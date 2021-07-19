;
; MSX generic definitions.

; ---------------------------------------------------------------------------
; Screen size

defc SCREEN_WIDTH  = 256
defc SCREEN_HEIGHT = 192

; ---------------------------------------------------------------------------
; MSX colors
defc MSX_COLOR_TRANSPARENT = 0
defc MSX_COLOR_BLACK = 1
defc MSX_COLOR_GREEN = 2
defc MSX_COLOR_LIGHT_GREEN = 3
defc MSX_COLOR_DARK_BLUE = 4
defc MSX_COLOR_LIGHT_BLUE = 5
defc MSX_COLOR_DARK_RED = 6
defc MSX_COLOR_SKY_BLUE = 7
defc MSX_COLOR_RED = 8
defc MSX_COLOR_BRIGHT_RED = 9
defc MSX_COLOR_YELLOW = 10
defc MSX_COLOR_LIGHT_YELLOW = 11
defc MSX_COLOR_DARK_GREEN = 12
defc MSX_COLOR_PURPLE = 13
defc MSX_COLOR_GREY = 14
defc MSX_COLOR_WHITE = 15

; ---------------------------------------------------------------------------
; MSX system variables
defc T32NAM = $f3bd ; Address of pattern name table
defc T32COL = $f3bf ; Address of colors table
defc T32CGP = $f3c1 ; Address of pattern generator table
defc T32ATR = $f3c3 ; Address of sprite attribute table
defc T32PAT = $f3c5 ; Address of sprite generator table

defc FORCLR = $f3e9 ; Foreground color
defc BAKCLR = $f3eA ; Background color
defc BDRCLR = $f3eB ; Border color

; ---------------------------------------------------------------------------
; MSX VDP port numbers.
defc VdpDataPort = $98
defc VdpCommandPort = $99

; ---------------------------------------------------------------------------
; MSX BIOS routine addresses
; TODO: Incomplete.
defc FILVRM = $0056
defc LDIRVM = $005c
defc CHGMOD = $005f
defc CHGCLR = $0062
defc INIT32 = $006f
defc SETTXT = $0078
defc SETGRP = $007e
defc CALPAT = $0084
defc CALATR = $0087
defc GSPSIZ = $008a
defc GRPPRT = $008d
defc CHSNS = $009c
defc CHGET = $009f
defc CHPUT = $00a2
defc BEEP = $00c0
defc CLS = $00c3
defc POSIT = $00c6
defc GTSTCK = $00d5
defc GTTRIG = $00d8
defc INIPLT = $0141
