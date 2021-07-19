	module src_puzl_msx_video2_asm

	public _InitializeVideo
	public VdpSetWriteAddress
	public GetVdpScreenPosition
	extern _InitializeTilemap
	extern _InitializeSprites

	include "msx.asm"

	section data_user

	section bss_user

	section rodata_user

	section code_user

._InitializeVideo
	; Set up screen.
	di
	
	ld a,$1 ; TODO: Change to color constant.
	ld (BDRCLR),a
	ld (BAKCLR),a
	call INIT32
	;call INIPLT
	
	call _InitializeTilemap
	call _InitializeSprites

	ei

	ret

; TODO: Is there a BIOS call for this?
.GetVdpScreenPosition
	ld h,c
	xor a
	
	srl h
	rr a
	srl h
	rr a
	srl h
	rr a
	
	or b
	ld l,a
	
	ld a,h
	or $18
	ld h,a

	ret

; Inputs:
;  - hl: address.
; Outputs:
;  - none
; Notes:
;  - Next call to VdpDataPort gives the correct port
;  - to use for this address.
.VdpSetWriteAddress
	ld a,l
	out (VdpCommandPort),a

	ld a,h
	or %01000000
	out (VdpCommandPort),a
	
	ret

; Inputs:
;  - hl: start address of data.
;  - bc: length of data.
.CopyToVdp
	push bc
		ld c,VdpDataPort
		outi
	pop bc
	dec bc
	ld a,b
	or c
	jr nz,CopyToVdp

	ret
