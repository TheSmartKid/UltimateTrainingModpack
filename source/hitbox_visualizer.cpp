#include <math.h>

#include "useful.h"

#include "l2c_imports.h"
#include "acmd_imports.h"
#include "acmd_wrapper.h"
#include "saltysd_dynamic.h"
#include "saltysd_helper.h"
#include "const_value_table.h"
#include "taunt_toggles.h"

#include "hitbox_visualizer_helper.h"
#include "hitbox_visualizer.h"

using namespace lib;
using namespace app::lua_bind;
using namespace app::sv_animcmd;

void (*AttackModule_set_attack_lua_state)(u64, u64);

Vector3f ID_COLORS[8] = { // used to tint the hitbox effects -- make sure that at least one component is equal to 1.0
	{ 1.0f, 0.0f, 0.0f }, // #ff0000 (red)
	{ 1.0f, 0.4f, 0.0f }, // #ff9900 (orange)
	{ 0.8f, 1.0f, 0.0f }, // #ccff00 (yellow)
	{ 0.2f, 1.0f, 0.2f }, // #00ff33 (green)
	{ 0.0f, 0.8f, 1.0f }, // #00ccff (sky blue)
	{ 0.4f, 0.4f, 1.0f }, // #6666ff (blue)
	{ 0.8f, 0.0f, 1.0f }, // #cc00ff (purple)
	{ 1.0f, 0.2f, 0.8f }, // #ff33cc (pink)
};
int MAX_EFFECTS_PER_HITBOX = 16; // max # of circles drawn for an extended hitbox

// clear graphics every time we clear all hitboxes
void app::lua_bind::AttackModule::clear_all_replace(u64 module_accessor) {
	if (is_training_mode()) {
		// only if we're not shielding
		int status_kind = StatusModule::status_kind(module_accessor);
		if (!(status_kind >= FIGHTER_STATUS_KIND_GUARD_ON && status_kind <= FIGHTER_STATUS_KIND_GUARD_OFF)) {
			Hash40 shieldEffectHash = { .hash = 0xAFAE75F05LL };
			EffectModule::kill_kind(module_accessor, shieldEffectHash.hash, 0, 1);
		}
	}

	// call original AttackModule::clear_all_impl
	u64 attack_module = load_module(module_accessor, 0xA0);
	void (*clear_all)(u64) = (void(*)(u64))(load_module_impl(attack_module, 0x50));

	return clear_all(attack_module);
}

void generate_hitbox_effects(L2CAgent *l2c_agent, L2CValue *bone, L2CValue *size,
		L2CValue *x, L2CValue *y, L2CValue *z, L2CValue *x2, L2CValue *y2, L2CValue *z2,
		Vector3f *color) {
	L2CValue red(color->x);
	L2CValue green(color->y);
	L2CValue blue(color->z);

	float size_mult = 19.0f / 200.0f;
	Hash40 shield_effect_hash = { .hash = 0xAFAE75F05LL };

	L2CValue shieldEffect(shield_effect_hash.hash);
	L2CValue x_rot(0.0f);
	L2CValue y_rot(0.0f);
	L2CValue z_rot(0.0f);
	L2CValue terminate(true);
	L2CValue effect_size((float)size->raw_float * size_mult);

	L2CValue rate(8.0f);

	float x_dist, y_dist, z_dist;
	int n_effects;
	if (x2->type != L2C_void && y2->type != L2C_void && z2->type != L2C_void) { // extended hitbox
		x_dist = x2->raw_float - x->raw_float;
		y_dist = y2->raw_float - y->raw_float;
		z_dist = z2->raw_float - z->raw_float;
		float dist = sqrtf(x_dist * x_dist + y_dist * y_dist + z_dist * z_dist);
		n_effects = (int)ceilf(dist / (size->raw_float * 1.75f)) + 1; // just enough effects to form a continuous line
		if (n_effects < 2)
		    n_effects = 2;
		if (n_effects > MAX_EFFECTS_PER_HITBOX)
		    n_effects = MAX_EFFECTS_PER_HITBOX;
	} else { // non-extended hitbox
		x_dist = y_dist = z_dist = 0;
		n_effects = 1;
	}

	for (int i = 0; i < n_effects; i++) {
		float t = n_effects <= 1 ? 0 : (float)i / (n_effects - 1);
		L2CValue x_curr(x->raw_float + x_dist * t);
		L2CValue y_curr(y->raw_float + y_dist * t);
		L2CValue z_curr(z->raw_float + z_dist * t);

		ACMD acmd(l2c_agent);
		acmd.wrap(EFFECT_FOLLOW_NO_SCALE, { shieldEffect, *bone, x_curr, y_curr, z_curr, x_rot, y_rot, z_rot, effect_size, terminate });

		// set to hitbox ID color
		acmd.wrap(LAST_EFFECT_SET_COLOR, { red, green, blue });

		// speed up animation by rate to remove pulsing effect
		acmd.wrap(LAST_EFFECT_SET_RATE, { rate });
	}
}

void app::sv_animcmd::ATTACK_replace(u64 a1) {
	// instantiate our own L2CAgent with the given lua_State
	L2CAgent l2c_agent;
	l2c_agent.L2CAgent_constr(a1);

	// get all necessary hitbox params
	L2CValue id, bone, damage, angle, kbg, fkb, bkb, size, x, y, z, x2, y2, z2;
	l2c_agent.get_lua_stack(1, &id); // int
	l2c_agent.get_lua_stack(3, &bone); // hash40
	l2c_agent.get_lua_stack(4, &damage); // float
	l2c_agent.get_lua_stack(5, &angle); // int
	l2c_agent.get_lua_stack(6, &kbg); // int
	l2c_agent.get_lua_stack(7, &fkb); // int
	l2c_agent.get_lua_stack(8, &bkb); // int
	l2c_agent.get_lua_stack(9, &size); // float
	l2c_agent.get_lua_stack(10, &x); // float
	l2c_agent.get_lua_stack(11, &y); // float
	l2c_agent.get_lua_stack(12, &z); // float
	l2c_agent.get_lua_stack(13, &x2); // float or void
	l2c_agent.get_lua_stack(14, &y2); // float or void
	l2c_agent.get_lua_stack(15, &z2); // float or void

	// original code: parse lua stack and call AttackModule::set_attack()
	AttackModule_set_attack_lua_state(LOAD64(LOAD64(a1 - 8) + 416LL), a1);

	if (HITBOX_VIS != NONE && is_training_mode()) { // generate hitbox effect(s)
		float color_scale = 1.0f;
		if (HITBOX_VIS == VIS_DAMAGE) { // color intensity scales with damage
			color_scale = unlerp_bounded(1.0f, 18.0f, damage.raw_float);
		} else if (HITBOX_VIS == VIS_KB) { // color intensity scales with KB
			// calculate the expected KB a character with 95 weight will receive at 80% pre-hit
			float TARGET_PERCENT = 80.0f;
			int TARGET_WEIGHT = 95;
			float percent_component;
			if (fkb.raw > 0) {
				percent_component = (10.0f + fkb.raw) * 0.1f * (1.0f + fkb.raw * 0.5f);
			} else {
				percent_component = (TARGET_PERCENT + damage.raw_float) * 0.1f * (1.0f + damage.raw_float * 0.5f);
			}
			float weight_component = 200.0f / (TARGET_WEIGHT + 100);
			float kb = (percent_component * weight_component * 1.4f + 18.0f) * (kbg.raw * 0.01f) + bkb.raw;
			color_scale = unlerp_bounded(50.0f, 200.0f, kb);
		}
		float color_t = 0.6f + 0.4f * powf(color_scale, 0.5f); // non-linear scaling to magnify differences at lower values
		Vector3f color = color_lerp({ 1.0f, 1.0f, 1.0f }, ID_COLORS[id.raw % 8], color_t);
		generate_hitbox_effects(&l2c_agent, &bone, &size, &x, &y, &z, &x2, &y2, &z2, &color);
	}

	u64 v1, v2, i;
	v1 = a1;

	// original code: clear_lua_stack section
	v2 = LOAD64(v1 + 16);
	for (i = **(u64 **)(v1 + 32) + 16LL; v2 < i; v2 = LOAD64(v1 + 16)) {
		LOAD64(v1 + 16) = v2 + 16;
		*(__int32_t *)(v2 + 8) = 0;
	}
	LOAD64(v1 + 16) = i;
}

void hitbox_vis_main() {
	AttackModule_set_attack_lua_state = (void (*)(u64, u64))SaltySDCore_FindSymbol("_ZN3app10sv_animcmd6ATTACKEP9lua_State") + 0xD0 - 0x70;
	SaltySD_function_replace_sym(
		"_ZN3app10sv_animcmd6ATTACKEP9lua_State",
		(u64)&ATTACK_replace);
	SaltySD_function_replace_sym(
		"_ZN3app8lua_bind28AttackModule__clear_all_implEPNS_26BattleObjectModuleAccessorE",
		(u64)&AttackModule::clear_all_replace);
}
