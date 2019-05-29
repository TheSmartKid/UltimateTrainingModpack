#ifndef TAUNT_TOGGLES_H
#define TAUNT_TOGGLES_H

#define NONE 0

// Up Taunt
// controls whether or not hitbox visualization is enabled
#define NUM_VIS_STATES 3
extern int HITBOX_VIS;
#define VIS_DAMAGE 1
#define VIS_KB 2

// Side Taunt
// controls CPU DI behavior
// 0, 0.785398, 1.570796, 2.356194, -3.14159, -2.356194,  -1.570796, -0.785398
// 0, pi/4,     pi/2,     3pi/4,    pi,       5pi/4,      3pi/2,     7pi/4
#define NUM_DI_STATES 10
extern int DI_STATE;
#define DI_RANDOM_IN_AWAY 9

// Down Taunt
// controls CPU mash behavior
#define NUM_TOGGLE_STATES 3
extern int TOGGLE_STATE;
#define MASH_AIRDODGE 1
#define MASH_JUMP 2

#endif // TAUNT_TOGGLES_H
