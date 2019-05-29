#include <math.h>

#include "crc32.h"
#include "useful.h"

#include "l2c_imports.h"
#include "saltysd_dynamic.h"
#include "saltysd_helper.h"
#include "acmd_wrapper.h"
#include "const_value_table.h"
#include "taunt_toggles.h"
#include "raygun_printer.h"

#include "training_mods.h"

using namespace lib;
using namespace app::lua_bind;

u64 fighter_manager_addr;

bool is_operation_cpu(u64 module_accessor) {
	int entry_id = WorkModule::get_int(module_accessor, FIGHTER_INSTANCE_WORK_ID_INT_ENTRY_ID);
	u64 fighter_information = FighterManager::get_fighter_information(LOAD64(fighter_manager_addr), entry_id);

	return FighterInformation::is_operation_cpu(fighter_information);
}

bool is_in_hitstun(u64 module_accessor) {
	int status_kind = StatusModule::status_kind(module_accessor);
	return status_kind >= FIGHTER_STATUS_KIND_DAMAGE && status_kind <= FIGHTER_STATUS_KIND_DAMAGE_FALL;
}

void perform_jump(u64 module_accessor) {
	int jump_count = WorkModule::get_int(module_accessor, FIGHTER_INSTANCE_WORK_ID_INT_JUMP_COUNT);
	int max_jump_count = WorkModule::get_int(module_accessor, FIGHTER_INSTANCE_WORK_ID_INT_JUMP_COUNT_MAX);
	if (jump_count < max_jump_count) {
		if (StatusModule::situation_kind(module_accessor) == SITUATION_KIND_AIR) {
		    if (WorkModule::get_param_int(module_accessor, 0xB99CC3FBCLL, 0) == FIGHTER_JUMP_AERIAL_TYPE_NORMAL)
				StatusModule::change_status_request_from_script(module_accessor, FIGHTER_STATUS_KIND_JUMP_AERIAL, 1);
		    else
				StatusModule::change_status_request_from_script(module_accessor, FIGHTER_STATUS_KIND_FLY, 1);
		} else if (StatusModule::situation_kind(module_accessor) == SITUATION_KIND_GROUND) {
		    StatusModule::change_status_request_from_script(module_accessor, FIGHTER_STATUS_KIND_JUMP_SQUAT, 1);   
		}			
		WorkModule::inc_int(module_accessor, FIGHTER_INSTANCE_WORK_ID_INT_JUMP_COUNT);
	}
}

// Force option out of hitstun
u64 app::lua_bind::WorkModule::enable_transition_term_group_replace(u64 module_accessor, int transition_group) {
	if (is_training_mode() && is_operation_cpu(module_accessor)) {
		if (is_in_hitstun(module_accessor)) {
			if (TOGGLE_STATE == MASH_AIRDODGE) { // airdodge
				if (transition_group == FIGHTER_STATUS_TRANSITION_GROUP_CHK_AIR_ESCAPE)
					StatusModule::change_status_request_from_script(module_accessor, FIGHTER_STATUS_KIND_ESCAPE_AIR, 1);
			} else if (TOGGLE_STATE == MASH_JUMP) { // jump
				if (transition_group == FIGHTER_STATUS_TRANSITION_GROUP_CHK_AIR_JUMP_AERIAL)
					perform_jump(module_accessor);
				else if (transition_group == FIGHTER_STATUS_TRANSITION_GROUP_CHK_GROUND_JUMP)
					perform_jump(module_accessor);
			}
		}
	}

	// call original WorkModule::enable_transition_term_group_impl
	u64 work_module = load_module(module_accessor, 0x50);
	u64 (*enable_transition_term_group)(u64, u64) = (u64(*)(u64, u64))(load_module_impl(work_module, 0x140));

	return enable_transition_term_group(work_module, transition_group);
}

// Force DI
float app::lua_bind::WorkModule::get_float_replace(u64 module_accessor, int var) {
	if (is_training_mode() && is_operation_cpu(module_accessor)) {
		if (is_in_hitstun(module_accessor)) {
			if (DI_STATE != NONE) {
				float angle;
				
				if (DI_STATE == DI_RANDOM_IN_AWAY) { // randomly pick between 0 (right) and PI (left)
					angle = app::sv_math::rand(hash40("fighter"), 2) * M_PI;
				} else {
					angle = (DI_STATE - 1) * M_PI / 4.0;
				}

				// If facing left, reverse angle
				if (PostureModule::lr(module_accessor) != -1.0) {
					angle -= M_PI;
				}

				if (var == FIGHTER_STATUS_DAMAGE_WORK_FLOAT_VECOR_CORRECT_STICK_X)
					return cos(angle);

				if (var == FIGHTER_STATUS_DAMAGE_WORK_FLOAT_VECOR_CORRECT_STICK_Y)
					return sin(angle);
			}
		}
	}

	// call original WorkModule::get_float_impl
	u64 work_module = load_module(module_accessor, 0x50);
	float (*get_float)(u64, int) = (float (*)(u64, int))(load_module_impl(work_module, 0x58));

	return get_float(work_module, var);
}

void app::lua_bind::MotionModule::change_motion_replace(u64 module_accessor, u64 motion_kind, float start_frame, float frame_speed_mult, bool unk1, float unk2, bool unk3, bool unk4) {
	u64 curr_motion_kind = MotionModule::motion_kind(module_accessor);
	if ((curr_motion_kind == hash40("damage_air_1") ||
	     curr_motion_kind == hash40("damage_air_2") ||
	     curr_motion_kind == hash40("damage_air_3")) && motion_kind == hash40("fall")) {
		if (is_training_mode() && is_operation_cpu(module_accessor)) {
			if (TOGGLE_STATE == MASH_AIRDODGE) { // airdodge
				StatusModule::change_status_request_from_script(module_accessor, FIGHTER_STATUS_KIND_ESCAPE_AIR, 1);
			} else if (TOGGLE_STATE == MASH_JUMP) { // jump
				perform_jump(module_accessor);
			}
		}
	}

	// call original
	u64 motion_module = load_module(module_accessor, 0x88);
	void (*change_motion)(u64, u64, float, float, bool, float, bool, bool) =
		(void (*)(u64, u64, float, float, bool, float, bool, bool)) load_module_impl(motion_module, 0xD8);

	change_motion(motion_module, motion_kind, start_frame, frame_speed_mult, unk1, unk2, unk3, unk4);
}

void training_mods_main() {
	fighter_manager_addr = SaltySDCore_FindSymbol("_ZN3lib9SingletonIN3app14FighterManagerEE9instance_E");
	SaltySD_function_replace_sym(
		"_ZN3app8lua_bind45WorkModule__enable_transition_term_group_implEPNS_26BattleObjectModuleAccessorEi",
		(u64)&WorkModule::enable_transition_term_group_replace);
	SaltySD_function_replace_sym(
		"_ZN3app8lua_bind26WorkModule__get_float_implEPNS_26BattleObjectModuleAccessorEi",
		(u64)&WorkModule::get_float_replace);
	SaltySD_function_replace_sym(
		"_ZN3app8lua_bind32MotionModule__change_motion_implEPNS_26BattleObjectModuleAccessorEN3phx6Hash40Effbfbb",
		(u64)&MotionModule::change_motion_replace);
}
