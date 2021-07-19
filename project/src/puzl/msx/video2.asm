	module src_puzl_msx_video2_asm

	public _InitializeVideo
	public VdpSetWriteAddress
	public CopyToVdp
	public FillVdp
	public GetVdpScreenPosition
	extern _InitializeTilemap
	extern _InitializeSprites

	include "msx.asm"

	section data_user

	section bss_user

	section rodata_user
	
.VdpScreenInitData
	db       %10,128+0 ; Mode register #0.
	db %01100000,128+1 ; Mode register #1.
	db     %0110,128+2 ; Name Table Base.
	db %10011111,128+3 ; Color Table Base.
	db      %000,128+4 ; Character Pattern?
	db  %0110110,128+5 ; Sprite Attribute Base.
	db      %111,128+6 ; Sprite Pattern.
	db       $f1,128+7 ; Text Color 1, Border Color.
.VdpScreenInitDataEnd

	section code_user

._InitializeVideo
	; Set up screen.
	;call INIT32
	
	; Set up screen.
	; TODO: Use BIOS calls?? NOTE: INIT32 is not sufficient alone.
	di

	ld c,VdpCommandPort
	ld b,VdpScreenInitDataEnd-VdpScreenInitData
	ld hl,VdpScreenInitData
	otir

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

; Inputs:
;  - l: Value to fill.
;  - de: Amount to fill.
.FillVdp
	ld c,VdpDataPort
.fillVpdLoop
	out (c),l
	dec de
	ld d,a
	or e
	jr nz,fillVpdLoop

	ret
