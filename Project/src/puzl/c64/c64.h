//
// C64 generic definitions.
// Stolen from Elite128.
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
// I/O: VIC

#define VIC              0xd000
#define VIC_SPR0_X       0xd000
#define VIC_SPR0_Y       0xd001
#define VIC_SPR1_X       0xd002
#define VIC_SPR1_Y       0xd003
#define VIC_SPR2_X       0xd004
#define VIC_SPR2_Y       0xd005
#define VIC_SPR3_X       0xd006
#define VIC_SPR3_Y       0xd007
#define VIC_SPR4_X       0xd008
#define VIC_SPR4_Y       0xd009
#define VIC_SPR5_X       0xd00a
#define VIC_SPR5_Y       0xd00b
#define VIC_SPR6_X       0xd00c
#define VIC_SPR6_Y       0xd00d
#define VIC_SPR7_X       0xd00e
#define VIC_SPR7_Y       0xd00f
#define VIC_SPR_HI_X     0xd010
#define VIC_SPR_ENA      0xd015
#define VIC_SPR_EXP_Y    0xd017
#define VIC_SPR_EXP_X    0xd01d
#define VIC_SPR_MCOLOR   0xd01c
#define VIC_SPR_BG_PRIO  0xd01b

#define VIC_SPR_SP_COL   0xd01e
#define VIC_SPR_BG_COL   0xd01f

#define VIC_SPR_MCOLOR0  0xd025
#define VIC_SPR_MCOLOR1  0xd026

#define VIC_SPR0_COLOR   0xd027
#define VIC_SPR1_COLOR   0xd028
#define VIC_SPR2_COLOR   0xd029
#define VIC_SPR3_COLOR   0xd02a
#define VIC_SPR4_COLOR   0xd02b
#define VIC_SPR5_COLOR   0xd02c
#define VIC_SPR6_COLOR   0xd02d
#define VIC_SPR7_COLOR   0xd02e

#define SPR_X_SCREEN_LEFT   24
#define SPR_Y_SCREEN_TOP    50

#define VIC_CTRL1        0xd011
#define VIC_CTRL2        0xd016

#define VIC_HLINE        0xd012

#define VIC_VIDEO_ADR    0xd018

#define VIC_IRR          0xd019 // Interrupt request register
#define VIC_IMR          0xd01a // Interrupt mask register

#define VIC_BORDERCOLOR  0xd020
#define VIC_BG_COLOR0    0xd021
#define VIC_BG_COLOR1    0xd022
#define VIC_BG_COLOR2    0xd023
#define VIC_BG_COLOR3    0xd024


// ---------------------------------------------------------------------------
// I/O: SID

#define SID              0xd400
#define SID_S1Lo         0xd400
#define SID_S1Hi         0xd401
#define SID_PB1Lo        0xd402
#define SID_PB1Hi        0xd403
#define SID_Ctl1         0xd404
#define SID_AD1          0xd405
#define SID_SUR1         0xd406

#define SID_S2Lo         0xd407
#define SID_S2Hi         0xd408
#define SID_PB2Lo        0xd409
#define SID_PB2Hi        0xd40a
#define SID_Ctl2         0xd40b
#define SID_AD2          0xd40c
#define SID_SUR2         0xd40d

#define SID_S3Lo         0xd40e
#define SID_S3Hi         0xd40f
#define SID_PB3Lo        0xd410
#define SID_PB3Hi        0xd411
#define SID_Ctl3         0xd412
#define SID_AD3          0xd413
#define SID_SUR3         0xd414

#define SID_FltLo        0xd415
#define SID_FltHi        0xd416
#define SID_FltCtl       0xd417
#define SID_Amp          0xd418
#define SID_ADConv1      0xd419
#define SID_ADConv2      0xd41a
#define SID_Noise        0xd41b
#define SID_Read3        0xd41c

// ---------------------------------------------------------------------------
// I/O: CIAs

#define CIA1             0xdc00
#define CIA1_PRA         0xdc00
#define CIA1_PRB         0xdc01
#define CIA1_DDRA        0xdc02
#define CIA1_DDRB        0xdc03
#define CIA1_TOD10       0xdc08
#define CIA1_TODSEC      0xdc09
#define CIA1_TODMIN      0xdc0a
#define CIA1_TODHR       0xdc0b
#define CIA1_ICR         0xdc0d
#define CIA1_CRA         0xdc0e
#define CIA1_CRB         0xdc0f

#define CIA2             0xdd00
#define CIA2_PRA         0xdd00
#define CIA2_PRB         0xdd01
#define CIA2_DDRA        0xdd02
#define CIA2_DDRB        0xdd03
#define CIA2_TOD10       0xdd08
#define CIA2_TODSEC      0xdd09
#define CIA2_TODMIN      0xdd0a
#define CIA2_TODHR       0xdd0b
#define CIA2_ICR         0xdd0d
#define CIA2_CRA         0xdd0e
#define CIA2_CRB         0xdd0f

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
