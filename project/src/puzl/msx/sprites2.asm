	module src_puzl_msx_sprites2_asm

	public _InitializeSprites
	extern VdpSetWriteAddress
	extern CopyToVdp
	
	include "msx.asm"

	section data_user

	section bss_user

	section rodata_user

.SpriteData
	db %11111111
	db %11111111
	db %11111111
	db %11111111
	db %11111111
	db %11111111
	db %11111111
	db %11111111
.SpriteDataEnd

	section code_user

._InitializeSprites
	; Populate hardware sprite shape.
	; TODO: Get base address from system for cross platform.
	ld hl,$3800+0*8
	call VdpSetWriteAddress

	ld hl,SpriteData
	ld bc,SpriteDataEnd-SpriteData
	call CopyToVdp

	; Set hardware sprite 0 attributes.
	ld hl,$1b00+0*4
	call VdpSetWriteAddress
	
	ld c,VdpDataPort
	ld a,$ff
	out (c),a
	ld a,0
	out (c),a
	ld a,0
	out (c),a
	ld a,8
	out (c),a

	ret
