#ifndef LEVEL_H
#define LEVEL_H

#define TARGET_SCREEN_TILE_WIDTH 32
#define TARGET_SCREEN_TILE_HEIGHT 24
#define TARGET_SCREEN_TILE_OFFSET_X (((SCREEN_WIDTH / TILE_WIDTH) - TARGET_SCREEN_TILE_WIDTH) / 2)
#define TARGET_SCREEN_TILE_OFFSET_Y 1
#define TARGET_SCREEN_BLOCK_WIDTH (TARGET_SCREEN_TILE_WIDTH / 2)
#define TARGET_SCREEN_BLOCK_HEIGHT (TARGET_SCREEN_TILE_HEIGHT / 2)

#define COLLISION_FLAG_LADDER COLLISION_FLAG_0

#define OBSTACLE_BLOCK 219
#define OBSTACLE_BLOCK_COLOR COLOR_WHITE

#define PICKUP_BLOCK '*'
#define PICKUP_BLOCK_COLOR COLOR_YELLOW

void LoadLevel(void);

#endif
