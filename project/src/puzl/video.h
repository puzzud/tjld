#ifndef VIDEO_H
#define VIDEO_H

#include <puzl.h>

#define CHARACTER_HEIGHT 8
#define NUMBER_OF_CHARACTERS 256

#define SPRITE_WIDTH 16
#define SPRITE_HEIGHT 16
#define NUMBER_OF_SPRITE_FRAMES 7 * 7
#define NUMBER_OF_SPRITES 8
#define NUMBER_OF_SPRITE_COLORS 3

typedef enum
{
	COLLISION_FLAG_0 = 0x01,
	COLLISION_FLAG_1 = 0x02,
	COLLISION_FLAG_2 = 0x04,
	COLLISION_FLAG_3 = 0x08,
	COLLISION_FLAG_4 = 0x10,
	COLLISION_FLAG_5 = 0x20,
	COLLISION_FLAG_6 = 0x40,
	COLLISION_FLAG_7 = 0x80
} CollisionFlags;

#define COLLISION_FLAG_OBSTACLE COLLISION_FLAG_7

typedef struct
{
	signed char x;
	signed char y;
} Vector2d;

typedef struct
{
	signed short x;
	signed short y;
} ScreenPoint;

typedef struct
{
	signed short x;
	signed short y;
	signed short width;
	signed short height;
} Rectangle;

extern byte PrintX;
extern byte PrintY;
extern byte PrintColor;

extern signed char SpriteVelocitiesX[NUMBER_OF_SPRITES];
extern signed char SpriteVelocitiesY[NUMBER_OF_SPRITES];

extern byte SpriteCollisionMasks[NUMBER_OF_SPRITES];
extern byte SpriteCollisions[NUMBER_OF_SPRITES];

void FASTCALL SetBackgroundColor(byte colorCode);

byte FASTCALL GetTileMapShapeCode(byte x, byte y);
byte FASTCALL GetTileMapColorCode(byte x, byte y);
byte FASTCALL GetTileMapCellCollisionCode(byte x, byte y);

void FASTCALL SetTileMapCellShape(byte x, byte y, byte shapeCode);
void FASTCALL SetTileMapCellColor(byte x, byte y, byte colorCode);
void FASTCALL SetTileMapCellCollisionCode(byte x, byte y, byte collisionCode);

extern void FASTCALL PrintText(const char* text, byte x, byte y);

void FASTCALL EnableSprite(byte spriteIndex, byte enable);
signed short FASTCALL GetSpritePositionX(byte spriteIndex);
signed short FASTCALL GetSpritePositionY(byte spriteIndex);
void FASTCALL SetSpritePosition(byte spriteIndex, signed short x, signed short y);
void FASTCALL MoveSprite(byte spriteIndex);
void FASTCALL SetSpriteFrameIndex(byte spriteIndex, byte frameIndex);
void FASTCALL SetSpriteColor(byte spriteIndex, byte colorCode);
void FASTCALL SetSpriteSeconaryColor(byte colorCode);
void FASTCALL SetSpriteTertiaryColor(byte colorCode);
void FASTCALL SetSpriteAnimationSet(byte spriteIndex, const byte** animationSet);
void FASTCALL PlaySpriteAnimation(byte spriteIndex, byte animationId, byte looping);
void FASTCALL StopSpriteAnimation(byte spriteIndex);

#endif
