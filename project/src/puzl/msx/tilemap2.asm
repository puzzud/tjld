	module src_puzl_msx_tilemap2_asm

	public _InitializeTilemap
	public _SetTileMapCellShape
	public _PrintText
	extern VdpSetWriteAddress
	extern GetVdpScreenPosition
	extern CopyToVdp
	extern _PrintX
	extern _PrintY
	extern _CharacterSet

	include "msx.asm"

	section data_user

	section bss_user

	section rodata_user

	section code_user

._InitializeTilemap
	call CopyCharacterPatternsToVdp
	call CopyCharacterColorPatternsToVdp

	; TODO: Initialize TileMapCollisionCodes.

	ret

.CopyCharacterPatternsToVdp
	; TODO: Get base address from system for cross platform.
	ld de,$0000
	ld hl,_CharacterSet
	ld bc,256*8
	call LDIRVM

	ret

.CopyCharacterColorPatternsToVdp
	; TODO: Get base address from system for cross platform.
	ld hl,$2000
	ld bc,256*8
	ld a,$f0
	call FILVRM

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
	call PrintCharacter

	ret

;void PrintText(const char* text)
; Inputs:
;  - sp[2]: text address.
._PrintText
	; b: x position.
	ld hl,_PrintX
	ld b,(hl)
	; c: y position.
	ld hl,_PrintY
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
