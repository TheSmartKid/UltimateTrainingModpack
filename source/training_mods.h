#ifndef TRAINING_MODS_H
#define TRAINING_MODS_H

#include "useful.h"

bool is_operation_cpu(u64);
bool is_in_hitstun(u64);
void perform_jump(u64);

namespace app::lua_bind::WorkModule {
	u64 enable_transition_term_group_replace(u64, int);
	float get_float_replace(u64, int);
}

namespace app::lua_bind::MotionModule {
	void change_motion_replace(u64, u64, float, float, bool, float, bool, bool);
}

void training_mods_main();

#endif // TRAINING_MODS_H
