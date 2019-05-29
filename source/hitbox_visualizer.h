#ifndef HITBOX_VISUALIZER_H
#define HITBOX_VISUALIZER_H

#include "useful.h"

#include "l2c_imports.h"

namespace app::lua_bind::AttackModule {
	void clear_all_replace(u64);
}

namespace app::sv_animcmd {
	void ATTACK_replace(u64);
}

void hitbox_vis_main();

#endif // HITBOX_VISUALIZER_H
