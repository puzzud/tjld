	module src_puzl_msx_tilemap2_asm

	public _InitializeTilemap
	public _SetTileMapCellShape
	public _PrintText
	extern VdpSetWriteAddress
	extern GetVdpScreenPosition
	extern CopyToVdp
	extern FillVdp
	extern _PrintX
	extern _PrintY
	extern _CharacterSet

	include "msx.asm"

	defc CHARACTER_CODE = 'A'

	section data_user

	section bss_user

	section rodata_user

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
	call CopyCharacterPatternsToVdp
	
	; Populate character colors.
	;call CopyCharacterColorPatternsToVdp

	ld hl,$2000+CHARACTER_CODE*8
	call VdpSetWriteAddress
	
	ld c,VdpDataPort
	ld hl,CharacterColorData
	ld b,8
	otir
	
	; TODO: Initialize TileMapCollisionCodes.

	ret

.CopyCharacterPatternsToVdp
	ld c,0
.copyCharactersLoop
	push bc
	call CopyCharacterPatternToVdp
	pop bc
	dec c
	ret z
	jp copyCharactersLoop

.CopyCharacterPatternToVdp
	ld h,0
	ld l,c
	and a
	add hl,hl
	add hl,hl
	add hl,hl ; x8
	push hl

	add hl,$0000
	call VdpSetWriteAddress
	
	ld c,VdpDataPort
	and a
	pop hl
	add hl,_CharacterSet
	ld b,8
	otir
	
	ret

.CopyCharacterColorPatternsToVdp
	; TODO: Get base address from system for cross platform.
	ld hl,$2000
	call VdpSetWriteAddress

	ld l,$f0
	ld de,256*8
	call FillVdp

	ret

; Inputs:
;  - b: x position.
;  - c: y position.
;  - a: character shape code.
.PrintCharacter
	; Put character on screen.
	call GetVdpScreenPosition
	call VdpSetWriteAddress
	
	ld c,VdpDataPort
	out (c),e

	ret

;void SetTileMapCellShape(byte x, byte y, byte shapeCode)
; Inputs:
;  - sp[6]: x position.
;  - sp[4]: y position.
;  - sp[2]: shapeCode.
._SetTileMapCellShape
	; TODO: Redo this with __z88dk_sdccdecl.
	; (in case of optimization requiring stack to be preserved for other operations).
	ld ix,0
	and a ; Reset carry.
	add ix,sp
	
	ld a,(ix+2) ; a has shapeCode.
	ld e,a

	ld a,(ix+4) ; a has y.
	ld c,a
	
	ld a,(ix+6) ; a has x.
	ld b,a

	; b holds x.
	; c holds y.
	ld e,CHARACTER_CODE ; TODO: Change to parameter after full character set is implemented.
	call PrintCharacter

	ret

;void PrintText(const char* text)
; Inputs:
;  - sp[2]: text address.
._PrintText
	; b: x position.
	ld hl,(_PrintX)
	ld b,(hl)
	; c: y position.
	ld hl,(_PrintY)
	ld c,(hl)
	call GetVdpScreenPosition
	call VdpSetWriteAddress
	
	ld ix,0
	and a ; Reset carry.
	add ix,sp
	
	ld a,(ix+2)
	ld l,a
	ld a,(ix+3)
	ld h,a

	ld c,VdpDataPort
.printTextLoop
	ld a,(hl)
	ret z
	out (c),a
	and a
	inc hl
	jp printTextLoop

	ret
