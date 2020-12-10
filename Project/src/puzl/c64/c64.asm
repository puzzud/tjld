;
; C64 generic definitions.
; Matches Compute's Mapping the Commodore 64.
; Some stolen from Elite128.
;

; ---------------------------------------------------------------------------
; Screen size

XSIZE           = 40
YSIZE           = 25

; ---------------------------------------------------------------------------
D6510     = $0000
R6510     = $0001

; ---------------------------------------------------------------------------
; I/O: VIC

VIC       = $d000
SP0X      = $d000
SP0Y      = $d001
SP1X      = $d002
SP1Y      = $d003
SP2X      = $d004
SP2Y      = $d005
SP3X      = $d006
SP3Y      = $d007
SP4X      = $d008
SP4Y      = $d009
SP5X      = $d00a
SP5Y      = $d00b
SP6X      = $d00c
SP6Y      = $d00d
SP7X      = $d00e
SP7Y      = $d00f
MSIGX     = $d010
SCROLY    = $d011
RASTER    = $d012
LPENX     = $d013
LPENY     = $d014
SPENA     = $d015
SCROLX    = $d016
YXPAND    = $d017
VMCSB     = $d018
VICIRQ    = $d019 ; Interrupt request register
IRQMASK   = $d01a ; Interrupt mask register
SPBGPR    = $d01b
SPMC      = $d01c
XXPAND    = $d01d
SPSPCL    = $d01e
SPBGCL    = $d01f
EXTCOL    = $d020
BGCOL0    = $d021
BGCOL1    = $d022
BGCOL2    = $d023
BGCOL3    = $d024
SPMC0     = $d025
SPMC1     = $d026
SP0COL    = $d027
SP1COL    = $d028
SP2COL    = $d029
SP3COL    = $d02a
SP4COL    = $d02b
SP5COL    = $d02c
SP6COL    = $d02d
SP7COL    = $d02e

; 128 stuff:
VIC_KBD_128     = $d02f        ; Extended kbd bits (visible in 64 mode)
VIC_CLK_128     = $d030 ; Clock rate register (visible in 64 mode)

SPR_X_SCREEN_LEFT = 24
SPR_Y_SCREEN_TOP  = 50

; ---------------------------------------------------------------------------
; I/O: SID

SID       = $d400
FRELO1    = $d400
FREHI1    = $d401
PWLO1     = $d402
PWHI1     = $d403
VCREG1    = $d404
ATDCY1    = $d405
SUREL1    = $d406

FRELO2    = $d407
FREHI2    = $d408
PWLO2     = $d409
PWHI2     = $d40a
VCREG2    = $d40b
ATDCY2    = $d40c
SUREL2    = $d40d

FRELO3    = $d40e
FREHI3    = $d40f
PWLO3     = $d410
PWHI3     = $d411
VCREG3    = $d412
ATDCY3    = $d413
SUREL3    = $d414

CUTLO     = $d415
CUTHI     = $d416
RESON     = $d417
SIGVOL    = $d418
POTX      = $d419
POTY      = $d41a
RANDOM    = $d41b
ENV3      = $d41c

; ---------------------------------------------------------------------------
; I/O: VDC (128 only)

VDC_INDEX       = $d600
VDC_DATA        = $d601

; ---------------------------------------------------------------------------
; I/O: CIAs

CIA1      = $dc00
CIAPRA    = $dc00
CIAPRB    = $dc01
CIDDRA    = $dc02
CIDDRB    = $dc03
TIMALO    = $dc04
TIMAHI    = $dc05
TIMBLO    = $dc06
TIMBHI    = $dc07
TODTEN    = $dc08
TODSEC    = $dc09
TODMIN    = $dc0a
TODHRS    = $dc0b
CIASDR    = $dc0c
CIAICR    = $dc0d
CIACRA    = $dc0e
CIACRB    = $dc0f

CIA2      = $dd00
CI2PRA    = $dd00
CI2PRB    = $dd01
C2DDRA    = $dd02
C2DDRB    = $dd03
TI2ALO    = $dd04
TI2AHI    = $dd05
TI2BLO    = $dd06
TI2BHI    = $dd07
TO2TEN    = $dd08
TO2SEC    = $dd09
TO2MIN    = $dd0a
TO2HRS    = $dd0b
CI2SDR    = $dd0c
CI2ICR    = $dd0d
CI2CRA    = $dd0e
CI2CRB    = $dd0f

; ---------------------------------------------------------------------------
; Super CPU

SCPU_VIC_Bank1  = $d075
SCPU_Slow       = $d07A
SCPU_Fast       = $d07B
SCPU_EnableRegs = $d07E
SCPU_DisableRegs= $d07F
SCPU_Detect     = $d0BC


; ---------------------------------------------------------------------------
; Processor Port at $01

LORAM   = $01     ; Enable the basic rom
HIRAM   = $02     ; Enable the kernal rom
IOEN    = $04     ; Enable I/O
CASSDATA  = $08     ; Cassette data
CASSPLAY  = $10     ; Cassette: Play
CASSMOT   = $20     ; Cassette motor on
TP_FAST   = $80     ; Switch Rossmoeller TurboProcess to fast mode

RAMONLY   = $F8     ; (~(LORAM | HIRAM | IOEN)) & $FF


; -----------
; Color codes

COLOR_BLACK                  = 0
COLOR_WHITE                  = 1
COLOR_RED                    = 2
COLOR_CYAN                   = 3
COLOR_PURPLE                 = 4
COLOR_GREEN                  = 5
COLOR_BLUE                   = 6
COLOR_YELLOW                 = 7
COLOR_ORANGE                 = 8
COLOR_BROWN                  = 9
COLOR_PINK                   = 10
COLOR_DARK_GREY              = 11
COLOR_GREY                   = 12
COLOR_LIGHT_GREEN            = 13
COLOR_LIGHT_BLUE             = 14
COLOR_LIGHT_GREY             = 15

; ---------------------------------------------------------------------------
; Additional defines (not stolen from Elite128).

;address of the screen buffer
SCREEN_CHAR                  = $cc00
SCREEN_CHAR_SIZE             = $400

;address of color ram
SCREEN_COLOR                 = $d800

SPRITE_GRAPHICS_TARGET       = $d000
CHARACTER_GRAPHICS_TARGET    = $f000
  
NUMBER_OF_HARDWARE_SPRITES   = 8
NUMBER_OF_SPRITES_DIV_4      = 49 / 4 ; TODO: This is kind of application specific. Extract?

;address of sprite pointers
SPRITE_POINTER_BASE          = SCREEN_CHAR + SCREEN_CHAR_SIZE - NUMBER_OF_HARDWARE_SPRITES

SCREEN_CHAR_WIDTH            = 40
SCREEN_CHAR_HEIGHT           = 25

SCREEN_BORDER_THICKNESS_X    = 24
SCREEN_BORDER_THICKNESS_Y    = 50

;sprite number constants
SPRITE_BASE                  = 64

JOYSTICK_UP             = $01
JOYSTICK_DOWN           = $02
JOYSTICK_LEFT           = $04
JOYSTICK_RIGHT          = $08
JOYSTICK_BUTTON0        = $10
JOYSTICK_BUTTON1        = $20
JOYSTICK_BUTTON2        = $40
JOYSTICK_BUTTON3        = $80

CONTROLLER_LEFT         = JOYSTICK_LEFT
CONTROLLER_RIGHT        = JOYSTICK_RIGHT
CONTROLLER_UP           = JOYSTICK_UP
CONTROLLER_DOWN         = JOYSTICK_DOWN
CONTROLLER_BUTTON0      = JOYSTICK_BUTTON0
CONTROLLER_BUTTON1      = JOYSTICK_BUTTON1
CONTROLLER_BUTTON2      = JOYSTICK_BUTTON2
CONTROLLER_BUTTON3      = JOYSTICK_BUTTON3

NUMBER_OF_VOICES        = 3
