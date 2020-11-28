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

typedef struct
{
	signed char x;
	signed char y;
} Direction;

typedef struct
{
	byte x;
	byte y;
} TilePoint;

typedef struct
{
	signed short x;
	signed short y;
} SpritePoint;

extern byte PrintX;
extern byte PrintY;
extern byte PrintColor;

void FASTCALL SetBackgroundColor(byte colorCode);

byte FASTCALL GetTileMapShapeCode(byte x, byte y);
byte FASTCALL GetTileMapColorCode(byte x, byte y);
void FASTCALL SetTileMapCellShape(byte x, byte y, byte shapeCode);
void FASTCALL SetTileMapCellColor(byte x, byte y, byte colorCode);

extern void FASTCALL PrintText(const char* text, byte x, byte y);

void FASTCALL EnableSprite(byte spriteIndex, byte enable);
signed short FASTCALL GetSpritePositionX(byte spriteIndex);
signed short FASTCALL GetSpritePositionY(byte spriteIndex);
void FASTCALL SetSpritePosition(byte spriteIndex, signed short x, signed short y);
void FASTCALL SetSpriteVelocity(byte spriteIndex, signed char x, signed char y);
void FASTCALL MoveSprite(byte spriteIndex);
void FASTCALL SetSpriteFrameIndex(byte spriteIndex, byte frameIndex);
void FASTCALL SetSpriteColor(byte spriteIndex, byte colorCode);
void FASTCALL SetSpriteSeconaryColor(byte colorCode);
void FASTCALL SetSpriteTertiaryColor(byte colorCode);

#endif
