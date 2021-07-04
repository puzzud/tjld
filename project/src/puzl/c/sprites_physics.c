#include <puzl.h>

signed char SpriteVelocitiesX[NUMBER_OF_SPRITES];
signed char SpriteVelocitiesY[NUMBER_OF_SPRITES];

byte SpriteCollisionMasks[NUMBER_OF_SPRITES];
byte SpriteCollisions[NUMBER_OF_SPRITES];

void BaseInitializeSpritesPhysics(void);
void CheckSpriteCollision(ScreenPoint* tempSpritePosition);
void CalculateSpriteTileCorners(ScreenPoint* spritePosition, Vector2d* upperLeftSpriteTile, Vector2d* lowerRightSpriteTile);

void BaseInitializeSpritesPhysics(void)
{
	unsigned int index;

	memset(SpriteVelocitiesX, 0, sizeof(SpriteVelocitiesX));
	memset(SpriteVelocitiesY, 0, sizeof(SpriteVelocitiesY));

	for (index = 0; index < NUMBER_OF_SPRITES; ++index)
	{
		// Collision data.
		SpriteCollisionMasks[index] = 0;
		SpriteCollisions[index] = 0;
	}
}

void MoveSprite(void)
{
	Sprite* sprite = &Sprites[CurrentSpriteIndex];
	ScreenPoint* spritePosition = &sprite->position;

	ScreenPoint tempSpritePosition;

	tempSpritePosition.x = spritePosition->x + SpriteVelocitiesX[CurrentSpriteIndex];
	tempSpritePosition.y = spritePosition->y + SpriteVelocitiesY[CurrentSpriteIndex];
	
	if (SpriteCollisionMasks[CurrentSpriteIndex] != 0)
	{
		CheckSpriteCollision(&tempSpritePosition);
	}

	// NOTE: Needs to happen to ensure platform implementation hooks.
	SetSpritePosition(tempSpritePosition.x, tempSpritePosition.y);
}

INLINE void CalculateSpriteTileCorners(ScreenPoint* spritePosition, Vector2d* upperLeftSpriteTile, Vector2d* lowerRightSpriteTile)
{
	upperLeftSpriteTile->x = spritePosition->x / TILE_WIDTH;
	upperLeftSpriteTile->y = spritePosition->y / TILE_HEIGHT;

	// TODO: Need way to track sprite dimensions.
	lowerRightSpriteTile->x = (spritePosition->x + SPRITE_WIDTH - 1) / TILE_WIDTH;
	lowerRightSpriteTile->y = (spritePosition->y + SPRITE_HEIGHT - 1) / TILE_HEIGHT;	
}

//--------------------------------------------------------------------------
// Checks for collision map overlap with temporary sprite position.
// Adjusts this position to original X or Y position, depending on velocity.
// Assumes sprite dimensions of 16x16.
void CheckSpriteCollision(ScreenPoint* tempSpritePosition)
{
	Sprite* sprite = &Sprites[CurrentSpriteIndex];
	ScreenPoint* spritePosition = &sprite->position;
	Vector2d spriteVelocity;

	int x, y;

	Vector2d upperLeftSpriteTile;
	Vector2d lowerRightSpriteTile;

	spriteVelocity.x = SpriteVelocitiesX[CurrentSpriteIndex];
	spriteVelocity.y = SpriteVelocitiesY[CurrentSpriteIndex];

	CalculateSpriteTileCorners(tempSpritePosition, &upperLeftSpriteTile, &lowerRightSpriteTile);

	SpriteCollisions[CurrentSpriteIndex] = 0;

	if ((SpriteCollisionMasks[CurrentSpriteIndex] & COLLISION_FLAG_OBSTACLE) != 0)
	{
		if (spriteVelocity.y < 0)
		{
			// Upper left.
			// Upper right.
			if (((GetTileMapCellCollisionCode(upperLeftSpriteTile.x, upperLeftSpriteTile.y) & COLLISION_FLAG_OBSTACLE) != 0) ||
				(((GetTileMapCellCollisionCode(lowerRightSpriteTile.x, upperLeftSpriteTile.y) & COLLISION_FLAG_OBSTACLE) != 0)))
			{
				tempSpritePosition->y = spritePosition->y;
				CalculateSpriteTileCorners(tempSpritePosition, &upperLeftSpriteTile, &lowerRightSpriteTile);
				SpriteCollisions[CurrentSpriteIndex] = COLLISION_FLAG_OBSTACLE;
			}
		}
		else if (spriteVelocity.y > 0)
		{
			// Lower left.
			// Lower right.
			if (((GetTileMapCellCollisionCode(upperLeftSpriteTile.x, lowerRightSpriteTile.y) & COLLISION_FLAG_OBSTACLE) != 0) ||
				(((GetTileMapCellCollisionCode(lowerRightSpriteTile.x, lowerRightSpriteTile.y) & COLLISION_FLAG_OBSTACLE) != 0)))
			{
				tempSpritePosition->y = spritePosition->y;
				CalculateSpriteTileCorners(tempSpritePosition, &upperLeftSpriteTile, &lowerRightSpriteTile);
				SpriteCollisions[CurrentSpriteIndex] = COLLISION_FLAG_OBSTACLE;
			}
		}

		if (spriteVelocity.x < 0)
		{
			// Upper left.
			// Lower left.
			if (((GetTileMapCellCollisionCode(upperLeftSpriteTile.x, upperLeftSpriteTile.y) & COLLISION_FLAG_OBSTACLE) != 0) ||
				(((GetTileMapCellCollisionCode(upperLeftSpriteTile.x, lowerRightSpriteTile.y) & COLLISION_FLAG_OBSTACLE) != 0)))
			{
				tempSpritePosition->x = spritePosition->x;
				CalculateSpriteTileCorners(tempSpritePosition, &upperLeftSpriteTile, &lowerRightSpriteTile);
				SpriteCollisions[CurrentSpriteIndex] = COLLISION_FLAG_OBSTACLE;
			}
		}
		else if (spriteVelocity.x > 0)
		{
			// Upper right.
			// Lower right.
			if (((GetTileMapCellCollisionCode(lowerRightSpriteTile.x, upperLeftSpriteTile.y) & COLLISION_FLAG_OBSTACLE) != 0) ||
				(((GetTileMapCellCollisionCode(lowerRightSpriteTile.x, lowerRightSpriteTile.y) & COLLISION_FLAG_OBSTACLE) != 0)))
			{
				tempSpritePosition->x = spritePosition->x;
				CalculateSpriteTileCorners(tempSpritePosition, &upperLeftSpriteTile, &lowerRightSpriteTile);
				SpriteCollisions[CurrentSpriteIndex] = COLLISION_FLAG_OBSTACLE;
			}
		}

		// TODO: Should be updated, but apparently doesn't matter much.
		//CalculateSpriteTileCorners(tempSpritePosition, &upperLeftSpriteTile, &lowerRightSpriteTile);
	}

	if ((SpriteCollisionMasks[CurrentSpriteIndex] & ~COLLISION_FLAG_OBSTACLE) != 0)
	{
		// TODO: Should be updated, but apparently doesn't matter much.
		//CalculateSpriteTileCorners(tempSpritePosition, &upperLeftSpriteTile, &lowerRightSpriteTile);

		// Cycle through all overlapped tiles and imprint their collision flags on
		// this sprite's collisions.
		for (y = upperLeftSpriteTile.y; y <= lowerRightSpriteTile.y; ++y)
		{
			for (x = upperLeftSpriteTile.x; x <= lowerRightSpriteTile.x; ++x)
			{
				SpriteCollisions[CurrentSpriteIndex] |= GetTileMapCellCollisionCode(x, y);
			}	
		}
	}
}
