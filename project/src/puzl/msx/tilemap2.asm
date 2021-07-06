	module src_puzl_msx_tilemap2_asm

	public _InitializeTilemap
	public _SetTileMapCellShape
	extern VdpSetWriteAddress
	extern GetVdpScreenPosition

	include "msx.asm"

	section data_user
.temp1
	defs 2

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

; Inputs:
;  - b: x position.
;  - c: y position.
;  - a: character shape code.
.PrintCharacter
	; Put character on screen.
	call GetVdpScreenPosition
	call VdpSetWriteAddress
	
	; TODO: Make 128 a parameter.
	; TODO: e to a not necessary?
	ld a,e
	out (c),a

	ret

;void SetTileMapCellShape(byte x, byte y, byte shapeCode)
; Inputs:
;  - sp[6]: x position.
;  - sp[4]: y position.
;  - sp[2]: shapeCode.
._SetTileMapCellShape
	; TODO: Redo this with __z88dk_sdccdecl or preserve stack entries
	; (in case of optimization requiring stack to be preserved for other operations).
	;ld ix,0
	;and a ; Reset carry.
	;add ix,sp

	;ld a,(ix+0) ; a has a byte of return address from call into this subroutine.
	;ld a,(ix+1) ; a has other byte of return address from call into this subroutine.
	
	;ld a,(ix+2) ; a has shapeCode.
	;ld e,a

	;ld a,(ix+4) ; a has y?
	;ld b,a
	
	;ld a,(ix+6) ; a has x?
	;ld c,a

	; Cache return address from stack.
	pop hl
	ld (temp1),hl

	pop hl
	;ld e,l

	pop hl
	ld c,l

	pop hl
	ld b,l

	; b holds x.
	; c holds y.
	ld e,128 ; TODO: Change to parameter after full character set is implemented.
	call PrintCharacter

	; Restore cached return address on stack, replace popped entries in stack (not really though).
	ld hl,(temp1)
	push hl
	push hl
	push hl
	push hl

	ret
