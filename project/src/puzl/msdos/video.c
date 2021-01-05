#include <msdos/video.h>

#include <puzl.h>

byte PrintX;
byte PrintY;
byte PrintColor;

void SetBackgroundColor(byte colorCode)
{
	
}

byte FASTCALL GetTileMapShapeCode(byte x, byte y)
{
	return 0;
}

byte FASTCALL GetTileMapColorCode(byte x, byte y)
{
	return 0;
}

byte FASTCALL GetTileMapCellCollisionCode(byte x, byte y)
{
	return 0;
}

void FASTCALL SetTileMapCellShape(byte x, byte y, byte shapeCode)
{

}

void FASTCALL SetTileMapCellColor(byte x, byte y, byte colorCode)
{

}

void FASTCALL SetTileMapCellCollisionCode(byte x, byte y, byte collisionCode)
{

}

void FASTCALL PrintText(const char* text, byte x, byte y)
{

}

void FASTCALL EnableSprite(byte spriteIndex, byte enable)
{

}

signed short FASTCALL GetSpritePositionX(byte spriteIndex)
{
	return 0;
}

signed short FASTCALL GetSpritePositionY(byte spriteIndex)
{
	return 0;
}

void FASTCALL SetSpritePosition(byte spriteIndex, signed short x, signed short y)
{
	
}

void FASTCALL SetSpriteVelocity(byte spriteIndex, signed char x, signed char y)
{
	
}

void FASTCALL MoveSprite(byte spriteIndex)
{
	
}

void FASTCALL SetSpriteFrameIndex(byte spriteIndex, byte frameIndex)
{
	
}

void FASTCALL SetSpriteColor(byte spriteIndex, byte colorCode)
{
	
}

void FASTCALL SetSpriteSeconaryColor(byte colorCode)
{
	
}

void FASTCALL SetSpriteTertiaryColor(byte colorCode)
{
	
}

void FASTCALL SetSpriteAnimationSet(byte spriteIndex, const byte** animationSet)
{
	
}

void FASTCALL PlaySpriteAnimation(byte spriteIndex, byte animationId, byte looping)
{
	
}

void FASTCALL StopSpriteAnimation(byte spriteIndex)
{
	
}
