#include <level.h>

#include <stdlib.h>

#include <puzl.h>

void FASTCALL GenerateHWall(byte x, byte y, byte width);
void FASTCALL GenerateVWall(byte x, byte y, byte height);

void LoadLevel(void)
{
	// TODO: Call out custom logic?
	SetBackgroundColor(COLOR_BLACK);

	GenerateHWall(0, 1, TILEMAP_WIDTH);
	GenerateVWall(0, 2, TILEMAP_HEIGHT - 2);
	GenerateVWall(TILEMAP_WIDTH - 1, 2, TILEMAP_HEIGHT - 2);
	GenerateHWall(0, TILEMAP_HEIGHT - 1, TILEMAP_WIDTH);

	SetTileMapCellShape(TILEMAP_WIDTH / 2, TILEMAP_HEIGHT - 2, PICKUP_BLOCK);
	SetTileMapCellColor(TILEMAP_WIDTH / 2, TILEMAP_HEIGHT - 2, PICKUP_BLOCK_COLOR);
}

void GenerateHWall(byte x, byte y, byte width)
{
	do
	{
		SetTileMapCellColor(x, y, OBSTACLE_BLOCK_COLOR);
		SetTileMapCellShape(x, y, OBSTACLE_BLOCK);
		SetTileMapCellCollisionCode(x, y, 1);

		++x;
	}
	while (--width > 0);
}

void GenerateVWall(byte x, byte y, byte height)
{
	do
	{
		SetTileMapCellColor(x, y, OBSTACLE_BLOCK_COLOR);
		SetTileMapCellShape(x, y, OBSTACLE_BLOCK);
		SetTileMapCellCollisionCode(x, y, 1);

		++y;
	}
	while (--height > 0);
}

void AddNewPickup(void)
{
	Vector2d randomTile;

	do
	{
		// TODO: Use of rand in cc65 C64 is warning about initialized data in BSS.
		randomTile.x = rand() % TILEMAP_WIDTH;
		randomTile.y = TILEMAP_HEIGHT - 2;
	}
	while (GetTileMapShapeCode(randomTile.x, randomTile.y) == OBSTACLE_BLOCK);

	SetTileMapCellShape(randomTile.x, randomTile.y, PICKUP_BLOCK);
	SetTileMapCellColor(randomTile.x, randomTile.y, PICKUP_BLOCK_COLOR);
}
