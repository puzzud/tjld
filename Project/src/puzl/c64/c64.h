//
// C64 generic definitions.
// Matches Compute's Mapping the Commodore 64.
// Some stolen from Elite128.
//
#ifndef C64_H
#define C64_H

#define BYTE_FROM_ADDRESS(A)		((unsigned char*)A)
#define GET_MEMORY_BYTE(A)			*BYTE_FROM_ADDRESS(A)
#define SET_MEMORY_BYTE(A, V)		*BYTE_FROM_ADDRESS(A) = V;

// ---------------------------------------------------------------------------
// Screen size

#define XSIZE             40
#define YSIZE             25

// ---------------------------------------------------------------------------
#define D6510      0x0000
#define R6510      0x0001

// ---------------------------------------------------------------------------
// I/O: VIC

#define VIC        0xd000
#define SP0X       0xd000
#define SP0Y       0xd001
#define SP1X       0xd002
#define SP1Y       0xd003
#define SP2X       0xd004
#define SP2Y       0xd005
#define SP3X       0xd006
#define SP3Y       0xd007
#define SP4X       0xd008
#define SP4Y       0xd009
#define SP5X       0xd00a
#define SP5Y       0xd00b
#define SP6X       0xd00c
#define SP6Y       0xd00d
#define SP7X       0xd00e
#define SP7Y       0xd00f
#define MSIGX      0xd010
#define SCROLY     0xd011
#define RASTER     0xd012
#define LPENX      0xd013
#define LPENY      0xd014
#define SPENA      0xd015
#define SCROLX     0xd016
#define YXPAND     0xd017
#define VMCSB      0xd018
#define VICIRQ     0xd019 // Interrupt request register
#define IRQMASK    0xd01a // Interrupt mask register
#define SPBGPR     0xd01b
#define SPMC       0xd01c
#define XXPAND     0xd01d
#define SPSPCL     0xd01e
#define SPBGCL     0xd01f
#define EXTCOL     0xd020
#define BGCOL0     0xd021
#define BGCOL1     0xd022
#define BGCOL2     0xd023
#define BGCOL3     0xd024
#define SPMC0      0xd025
#define SPMC1      0xd026
#define SP0COL     0xd027
#define SP1COL     0xd028
#define SP2COL     0xd029
#define SP3COL     0xd02a
#define SP4COL     0xd02b
#define SP5COL     0xd02c
#define SP6COL     0xd02d
#define SP7COL     0xd02e

#define SPR_X_SCREEN_LEFT   24
#define SPR_Y_SCREEN_TOP    50

// ---------------------------------------------------------------------------
// I/O: SID

#define SID        0xd400
#define FRELO1     0xd400
#define FREHI1     0xd401
#define PWLO1      0xd402
#define PWHI1      0xd403
#define VCREG1     0xd404
#define ATDCY1     0xd405
#define SUREL1     0xd406

#define FRELO2     0xd407
#define FREHI2     0xd408
#define PWLO2      0xd409
#define PWHI2      0xd40a
#define VCREG2     0xd40b
#define ATDCY2     0xd40c
#define SUREL2     0xd40d

#define FRELO3     0xd40e
#define FREHI3     0xd40f
#define PWLO3      0xd410
#define PWHI3      0xd411
#define VCREG3     0xd412
#define ATDCY3     0xd413
#define SUREL3     0xd414

#define CUTLO      0xd415
#define CUTHI      0xd416
#define RESON      0xd417
#define SIGVOL     0xd418
#define POTX       0xd419
#define POTY       0xd41a
#define RANDOM     0xd41b
#define ENV3       0xd41c

// ---------------------------------------------------------------------------
// I/O: CIAs

#define CIA1       0xdc00
#define CIAPRA     0xdc00
#define CIAPRB     0xdc01
#define CIDDRA     0xdc02
#define CIDDRB     0xdc03
#define TIMALO     0xdc04
#define TIMAHI     0xdc05
#define TIMBLO     0xdc06
#define TIMBHI     0xdc07
#define TODTEN     0xdc08
#define TODSEC     0xdc09
#define TODMIN     0xdc0a
#define TODHRS     0xdc0b
#define CIASDR     0xdc0c
#define CIAICR     0xdc0d
#define CIACRA     0xdc0e
#define CIACRB     0xdc0f

#define CIA2       0xdd00
#define CI2PRA     0xdd00
#define CI2PRB     0xdd01
#define C2DDRA     0xdd02
#define C2DDRB     0xdd03
#define TI2ALO     0xdd04
#define TI2AHI     0xdd05
#define TI2BLO     0xdd06
#define TI2BHI     0xdd07
#define TO2TEN     0xdd08
#define TO2SEC     0xdd09
#define TO2MIN     0xdd0a
#define TO2HRS     0xdd0b
#define CI2SDR     0xdd0c
#define CI2ICR     0xdd0d
#define CI2CRA     0xdd0e
#define CI2CRB     0xdd0f

// ---------------------------------------------------------------------------
// Processor Port at $01

#define LORAM    0x01     // Enable the basic rom
#define HIRAM    0x02     // Enable the kernal rom
#define IOEN     0x04     // Enable I/O
#define CASSDATA   0x08     // Cassette data
#define CASSPLAY   0x10     // Cassette: Play
#define CASSMOT    0x20     // Cassette motor on
#define TP_FAST    0x80     // Switch Rossmoeller TurboProcess to fast mode

#define RAMONLY    0xF8     // (~(LORAM | HIRAM | IOEN)) & $FF


// -----------
// Color codes

#define COLOR_BLACK                    0
#define COLOR_WHITE                    1
#define COLOR_RED                      2
#define COLOR_CYAN                     3
#define COLOR_PURPLE                   4
#define COLOR_GREEN                    5
#define COLOR_BLUE                     6
#define COLOR_YELLOW                   7
#define COLOR_ORANGE                   8
#define COLOR_BROWN                    9
#define COLOR_PINK                     10
#define COLOR_DARK_GREY                11
#define COLOR_GREY                     12
#define COLOR_LIGHT_GREEN              13
#define COLOR_LIGHT_BLUE               14
#define COLOR_LIGHT_GREY               15

// ---------------------------------------------------------------------------
// Additional defines (not stolen from Elite128).

//address of the screen buffer
#define SCREEN_CHAR                   0xcc00
#define SCREEN_CHAR_SIZE              0x400

//address of color ram
#define SCREEN_COLOR                  0xd800

#define SPRITE_GRAPHICS_TARGET        0xd000
#define CHARACTER_GRAPHICS_TARGET     0xf000
  
#define NUMBER_OF_HARDWARE_SPRITES     8

//address of sprite pointers
//#define SPRITE_POINTER_BASE            SCREEN_CHAR + SCREEN_CHAR_SIZE - NUMBER_OF_HARDWARE_SPRITES
#define SPRITE_POINTER_BASE						 0xcc00 + 0x400 - 8

#define SCREEN_CHAR_WIDTH              40
#define SCREEN_CHAR_HEIGHT             25

#define SCREEN_BORDER_THICKNESS_X      24
#define SCREEN_BORDER_THICKNESS_Y      50

//sprite number constants
#define SPRITE_BASE                    64

#define JOYSTICK_UP             0x01
#define JOYSTICK_DOWN           0x02
#define JOYSTICK_LEFT           0x04
#define JOYSTICK_RIGHT          0x08
#define JOYSTICK_BUTTON0        0x10
#define JOYSTICK_BUTTON1        0x20
#define JOYSTICK_BUTTON2        0x40
#define JOYSTICK_BUTTON3        0x80

#define CONTROLLER_LEFT         JOYSTICK_LEFT
#define CONTROLLER_RIGHT        JOYSTICK_RIGHT
#define CONTROLLER_UP           JOYSTICK_UP
#define CONTROLLER_DOWN         JOYSTICK_DOWN
#define CONTROLLER_BUTTON0      JOYSTICK_BUTTON0
#define CONTROLLER_BUTTON1      JOYSTICK_BUTTON1
#define CONTROLLER_BUTTON2      JOYSTICK_BUTTON2
#define CONTROLLER_BUTTON3      JOYSTICK_BUTTON3

#endif
