;
; MSX generic definitions.

; ---------------------------------------------------------------------------
; Screen size

defc SCREEN_WIDTH  = 256
defc SCREEN_HEIGHT = 192

; ---------------------------------------------------------------------------
; MSX system variables
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
