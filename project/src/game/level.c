#include <level.h>

#include <puzl.h>

void FASTCALL GenerateBlock(byte x, byte y, byte shape, byte color, byte collision);
void FASTCALL GeneratePlatformBlock(byte x, byte y);
void FASTCALL GenerateLadderBlock(byte x, byte y);
void FASTCALL GenerateHWall(byte x, byte y, byte width);
void FASTCALL GenerateVWall(byte x, byte y, byte height);

void FASTCALL GeneratePickup(byte x, byte y);

void LoadLevel(void)
{
	SetBackgroundColor(COLOR_BLACK);

	GenerateHWall(0, TARGET_SCREEN_BLOCK_HEIGHT - 1, TARGET_SCREEN_BLOCK_WIDTH);
	GenerateVWall(0, 0, TARGET_SCREEN_BLOCK_HEIGHT - 1);
	GenerateVWall(TARGET_SCREEN_BLOCK_WIDTH - 1, 0, TARGET_SCREEN_BLOCK_HEIGHT - 1);

	GenerateHWall(8, 8, 6);
	GenerateLadderBlock(12, 8);
	GenerateLadderBlock(12, 9);
	GenerateLadderBlock(12, 10);

	GeneratePickup(16, 21);
	GeneratePickup(18, 15);
}

void FASTCALL GenerateBlock(byte x, byte y, byte shape, byte color, byte collision)
{
	byte blockX = (x * 2) + TARGET_SCREEN_TILE_OFFSET_X;
	byte blockY = (y * 2) + TARGET_SCREEN_TILE_OFFSET_Y;

	SetTileMapCellShape(blockX, blockY, shape);
	SetTileMapCellColor(blockX, blockY, color);
	SetTileMapCellCollisionCode(blockX, blockY, collision);

	++blockX;
	SetTileMapCellShape(blockX, blockY, shape);
	SetTileMapCellColor(blockX, blockY, color);
	SetTileMapCellCollisionCode(blockX, blockY, collision);

	++blockY;
	SetTileMapCellShape(blockX, blockY, shape);
	SetTileMapCellColor(blockX, blockY, color);
	SetTileMapCellCollisionCode(blockX, blockY, collision);

	--blockX;
	SetTileMapCellShape(blockX, blockY, shape);
	SetTileMapCellColor(blockX, blockY, color);
	SetTileMapCellCollisionCode(blockX, blockY, collision);
}

void FASTCALL GeneratePlatformBlock(byte x, byte y)
{
	GenerateBlock(x, y, OBSTACLE_BLOCK, OBSTACLE_BLOCK_COLOR, COLLISION_FLAG_OBSTACLE);
}

void GenerateLadderBlock(byte x, byte y)
{
	GenerateBlock(x, y, OBSTACLE_BLOCK, COLOR_PURPLE, COLLISION_FLAG_LADDER);
}

void GenerateHWall(byte x, byte y, byte width)
{
	do
	{
		GeneratePlatformBlock(x, y);

		++x;
	}
	while (--width > 0);
}

void GenerateVWall(byte x, byte y, byte height)
{
	do
	{
		GeneratePlatformBlock(x, y);

		++y;
	}
	while (--height > 0);
}

void GeneratePickup(byte x, byte y)
{
	byte blockX = x + TARGET_SCREEN_TILE_OFFSET_X;
	byte blockY = y + TARGET_SCREEN_TILE_OFFSET_Y;

	SetTileMapCellShape(blockX, blockY, PICKUP_BLOCK);
	SetTileMapCellColor(blockX, blockY, PICKUP_BLOCK_COLOR);
}
