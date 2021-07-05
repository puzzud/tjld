	MODULE src_puzl_msx_video2_asm

	INCLUDE "msx.asm"

	SECTION data_user

	SECTION code_user

._InitializeVideo
	call _InitializeTilemap
	call _InitializeSprites
	ret

	PUBLIC _InitializeVideo
	EXTERN _InitializeTilemap
	EXTERN _InitializeSprites
