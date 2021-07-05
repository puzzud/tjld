	module src_puzl_msx_tilemap2_asm

	public _InitializeTilemap
	public _SetTileMapCellShape
	extern VdpSetWriteAddress
	extern GetVdpScreenPosition

	include "msx.asm"

	section data_user

	section bss_user

	section rodata_user

.CharacterShapeData
	db %11111111
	db %11111111
	db %11111111
	db %11111111
	db %11111111
	db %11111111
	db %11111111
	db %11111111

.CharacterColorData
	db $f0
	db $f0
	db $f0
	db $f0
	db $f0
	db $f0
	db $f0
	db $f0

	section code_user

._InitializeTilemap
	; Populate character shape.
	; TODO: Get base address from system for cross platform.
	ld hl,$0000+128*8
	call VdpSetWriteAddress
	
	ld hl,CharacterShapeData
	ld b,8
	otir
	
	; Populate character colors.
	; TODO: Get base address from system for cross platform.
	ld hl,$2000+128*8
	call VdpSetWriteAddress
	
	ld hl,CharacterColorData
	ld b,8
	otir
	
	; TODO: Initialize TileMapCollisionCodes.

	ret

.PrintCharacter
	; Put character on screen.
	call GetVdpScreenPosition
	call VdpSetWriteAddress
	
	; TODO: Make 128 a parameter.
	ld a,128
	out (c),a

	ret

;void SetTileMapCellShape(byte x, byte y, byte shapeCode)
._SetTileMapCellShape
	; TODO: Figure out parameters and stack management.
	ld bc,$0808 ; b holds x, c holds y.
	call PrintCharacter
	
	;pop hl
	;pop hl
	;pop hl
	
	ret
