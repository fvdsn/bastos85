#ifndef __GAME_H__
#define __GAME_H__

#define P_SHIP 1
#define P_WALL 2
#define P_BG   3
#define P_MISSILE 4
#define P_SHIP_TRAIL 5
#define P_SHIP_BOOST 6

#define SHIP_SPEED 350
#define SHIP_ACCEL 2000

void game_register_ship(void);

#endif

