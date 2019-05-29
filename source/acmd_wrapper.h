#ifndef ACMD_WRAPPER_H
#define ACMD_WRAPPER_H

#include <switch.h>

#include <initializer_list>

#include "l2c_imports.h"
#include "useful.h"

u64 load_module(u64 module_accessor, u64 module_offset);

void* load_module_impl(u64 module, u64 function_offset);

namespace app::sv_math {
	int rand(u64, int) asm("_ZN3app7sv_math4randEN3phx6Hash40Ei") LINKABLE;
}

namespace app::sv_module_access {
	/**
	 * Used to cause a grab release.
	 * 
	 * This function should not be called directly. Instead, use ACMD.wrap() like so:
	 * ACMD.wrap(app::sv_module_access::grab, { arg1, arg2, arg3 });
	 * 
	 * arguments:
	 * - flag (int) -- usually MA_MSC_CMD_GRAB_CLEAR_ALL
	 */
	u64 grab(u64) asm("_ZN3app16sv_module_access4grabEP9lua_State") LINKABLE;
}

namespace app::sv_system {
	u64 battle_object(u64) asm("_ZN3app9sv_system13battle_objectEP9lua_State") LINKABLE;
	u64 battle_object_module_accessor(u64) asm("_ZN3app9sv_system29battle_object_module_accessorEP9lua_State") LINKABLE;
	u8 battle_object_category(u64) asm("_ZN3app9sv_system22battle_object_categoryEP9lua_State") LINKABLE;
	int battle_object_kind(u64) asm("_ZN3app9sv_system18battle_object_kindEP9lua_State") LINKABLE;
}

namespace app::lua_bind {
	namespace AttackModule {
		/**
		 * terminate a specific hitbox
		 * arguments:
		 * - module_accessor (u64) -- one of the fields of the ACMD struct
		 * - ID (int)
		 * - ??? (bool)
		 */
		void clear(u64, int, bool) asm("_ZN3app8lua_bind24AttackModule__clear_implEPNS_26BattleObjectModuleAccessorEib") LINKABLE;
		
		/**
		 * terminate all hitboxes
		 * arguments:
		 * - module_accessor (u64) -- one of the fields of the ACMD struct
		 */
		void clear_all(u64) asm("_ZN3app8lua_bind28AttackModule__clear_all_implEPNS_26BattleObjectModuleAccessorE") LINKABLE;
		
		
		void set_attack_height_all(u64, u64, bool) asm("_ZN3app8lua_bind40AttackModule__set_attack_height_all_implEPNS_26BattleObjectModuleAccessorENS_12AttackHeightEb") LINKABLE;
		void set_catch_only_all(u64, bool, bool) asm("_ZN3app8lua_bind37AttackModule__set_catch_only_all_implEPNS_26BattleObjectModuleAccessorEbb") LINKABLE;
	}

	namespace ControlModule {
		bool check_button_on(u64, int) asm("_ZN3app8lua_bind35ControlModule__check_button_on_implEPNS_26BattleObjectModuleAccessorEi") LINKABLE;
	}  

	namespace EffectModule {
		// boma, effect, joint, pos, rot, size, random_pos, random_rot, NO_SCALE?, attr?, unkint1, unkint2
		uint req_on_joint(u64, u64, u64, const Vector3f*, const Vector3f*, float a6, const Vector3f*, const Vector3f*, bool, uint, int, int) 
			asm("_ZN3app8lua_bind31EffectModule__req_on_joint_implEPNS_26BattleObjectModuleAccessorEN3phx6Hash40ES4_RKNS3_8Vector3fES7_fS7_S7_bjii") LINKABLE;
		void kill_kind(u64, u64, bool, bool) asm("_ZN3app8lua_bind28EffectModule__kill_kind_implEPNS_26BattleObjectModuleAccessorEN3phx6Hash40Ebb") LINKABLE;
	}

	namespace FighterManager {
		u64 get_fighter_information(u64, int) asm("_ZN3app8lua_bind44FighterManager__get_fighter_information_implEPNS_14FighterManagerENS_14FighterEntryIDE") LINKABLE;
	}

	namespace FighterInformation {
		bool is_operation_cpu(u64) asm("_ZN3app8lua_bind41FighterInformation__is_operation_cpu_implEPNS_18FighterInformationE") LINKABLE;
	}

	namespace GrabModule {
		void set_rebound(u64, bool) asm("_ZN3app8lua_bind28GrabModule__set_rebound_implEPNS_26BattleObjectModuleAccessorEb") LINKABLE;
	}

	namespace HitModule {
		void set_status_all(u64, int, int) asm("_ZN3app8lua_bind30HitModule__set_status_all_implEPNS_26BattleObjectModuleAccessorENS_9HitStatusEi") LINKABLE;
	}

	namespace MotionModule {
		float frame(u64) asm("_ZN3app8lua_bind24MotionModule__frame_implEPNS_26BattleObjectModuleAccessorE") LINKABLE;
		u64 motion_kind(u64) asm("_ZN3app8lua_bind30MotionModule__motion_kind_implEPNS_26BattleObjectModuleAccessorE") LINKABLE;
	}

	namespace PostureModule {
		float lr(u64) asm("_ZN3app8lua_bind22PostureModule__lr_implEPNS_26BattleObjectModuleAccessorE") LINKABLE;
		float pos_x(u64) asm("_ZN3app8lua_bind25PostureModule__pos_x_implEPNS_26BattleObjectModuleAccessorE") LINKABLE;
		float pos_y(u64) asm("_ZN3app8lua_bind25PostureModule__pos_y_implEPNS_26BattleObjectModuleAccessorE") LINKABLE;
		float set_pos(u64, const Vector3f*) asm("_ZN3app8lua_bind27PostureModule__set_pos_implEPNS_26BattleObjectModuleAccessorERKN3phx8Vector3fE") LINKABLE;
	}

	namespace StatusModule {
		u64 change_status_request_from_script(u64, int, bool) asm("_ZN3app8lua_bind52StatusModule__change_status_request_from_script_implEPNS_26BattleObjectModuleAccessorEib") LINKABLE;
		int status_kind(u64) asm("_ZN3app8lua_bind30StatusModule__status_kind_implEPNS_26BattleObjectModuleAccessorE") LINKABLE;
		int situation_kind(u64) asm("_ZN3app8lua_bind33StatusModule__situation_kind_implEPNS_26BattleObjectModuleAccessorE") LINKABLE;
	}

	namespace WorkModule {
		float get_float(u64, int) asm("_ZN3app8lua_bind26WorkModule__get_float_implEPNS_26BattleObjectModuleAccessorEi") LINKABLE;
		int get_int(u64, int) asm("_ZN3app8lua_bind24WorkModule__get_int_implEPNS_26BattleObjectModuleAccessorEi") LINKABLE;
		void inc_int(u64, int) asm("_ZN3app8lua_bind24WorkModule__inc_int_implEPNS_26BattleObjectModuleAccessorEi") LINKABLE;
		float get_param_float(u64, u64, u64) asm("_ZN3app8lua_bind32WorkModule__get_param_float_implEPNS_26BattleObjectModuleAccessorEmm") LINKABLE;
		int get_param_int(u64, u64, u64) asm("_ZN3app8lua_bind30WorkModule__get_param_int_implEPNS_26BattleObjectModuleAccessorEmm") LINKABLE;
		void on_flag(u64, int) asm("_ZN3app8lua_bind24WorkModule__on_flag_implEPNS_26BattleObjectModuleAccessorEi") LINKABLE;
		void off_flag(u64, int) asm("_ZN3app8lua_bind25WorkModule__off_flag_implEPNS_26BattleObjectModuleAccessorEi") LINKABLE;
		float set_float(u64, float, int) asm("_ZN3app8lua_bind26WorkModule__set_float_implEPNS_26BattleObjectModuleAccessorEfi") LINKABLE;
		int set_int(u64, int, int) asm("_ZN3app8lua_bind24WorkModule__set_int_implEPNS_26BattleObjectModuleAccessorEii") LINKABLE;
	}
}

struct ACMD {
	lib::L2CAgent* l2c_agent;
	u64 module_accessor;

	ACMD(lib::L2CAgent* agent);

	void frame(float f);
	void wait(float f);
	bool is_excute();
	
	void wrap(u64 (*acmd_func)(u64), std::initializer_list<lib::L2CValue> list);

	void ATTACK(
		u64 i1,   // ID
		u64 i2,   // Part
		u64 h1,   // Bone
		float f1, // Damage
		u64 i3,   // Angle
		u64 i4,   // KBG
		u64 i5,   // FKB
		u64 i6,   // BKB
		float f2, // Size
		float f3, // X
		float f4, // Y
		float f5, // Z
		          // X2
		          // Y2
		          // Z2
		float f6, // Hitlag
		float f7, // SDI
		u64 i7,   // Clang/Rebound
		u64 i8,   // Facing Restriction
		u64 i9,   // Fixed Weight
		u64 i10,  // Shield Damage
		float f8, // Trip Chance
		u64 i11,  // Rehit Rate
		u64 i12,  // Reflectable
		u64 i13,  // Absorbable
		u64 i14,  // Flinchless
		u64 i15,  // Disable Hitlag
		u64 i16,  // Direct
		u64 i17,  // Ground/Air
		u64 i18,  // Hit Bits
		u64 i19,  // Collision Bits
		u64 i20,  // Friendly Fire
		u64 h2,   // Effect
		u64 i21,  // SFX Level
		u64 i22,  // SFX Type
		u64 i23); // Move Type

	void ATTACK(
		u64 i1,   // ID
		u64 i2,   // Part
		u64 h1,   // Bone
		float f1, // Damage
		u64 i3,   // Angle
		u64 i4,   // KBG
		u64 i5,   // FKB
		u64 i6,   // BKB
		float f2, // Size
		float f3, // X
		float f4, // Y
		float f5, // Z
		float fX2,// X2
		float fY2,// Y2
		float fZ2,// Z2
		float f6, // Hitlag
		float f7, // SDI
		u64 i7,   // Clang/Rebound
		u64 i8,   // Facing Restriction
		u64 i9,   // Fixed Weight
		u64 i10,  // Shield Damage
		float f8, // Trip Chance
		u64 i11,  // Rehit Rate
		u64 i12,  // Reflectable
		u64 i13,  // Absorbable
		u64 i14,  // Flinchless
		u64 i15,  // Disable Hitlag
		u64 i16,  // Direct
		u64 i17,  // Ground/Air
		u64 i18,  // Hit Bits
		u64 i19,  // Collision Bits
		u64 i20,  // Friendly Fire
		u64 h2,   // Effect
		u64 i21,  // SFX Level
		u64 i22,  // SFX Type
		u64 i23); // Move Type	
};

#endif // ACMD_WRAPPER_H
