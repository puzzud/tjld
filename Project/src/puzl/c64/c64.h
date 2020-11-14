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

#define VIC              BYTE_FROM_ADDRESS(0xd000)
#define VIC_SPR0_X       BYTE_FROM_ADDRESS(0xd000)
#define VIC_SPR0_Y       BYTE_FROM_ADDRESS(0xd001)
#define VIC_SPR1_X       BYTE_FROM_ADDRESS(0xd002)
#define VIC_SPR1_Y       BYTE_FROM_ADDRESS(0xd003)
#define VIC_SPR2_X       BYTE_FROM_ADDRESS(0xd004)
#define VIC_SPR2_Y       BYTE_FROM_ADDRESS(0xd005)
#define VIC_SPR3_X       BYTE_FROM_ADDRESS(0xd006)
#define VIC_SPR3_Y       BYTE_FROM_ADDRESS(0xd007)
#define VIC_SPR4_X       BYTE_FROM_ADDRESS(0xd008)
#define VIC_SPR4_Y       BYTE_FROM_ADDRESS(0xd009)
#define VIC_SPR5_X       BYTE_FROM_ADDRESS(0xd00a)
#define VIC_SPR5_Y       BYTE_FROM_ADDRESS(0xd00b)
#define VIC_SPR6_X       BYTE_FROM_ADDRESS(0xd00c)
#define VIC_SPR6_Y       BYTE_FROM_ADDRESS(0xd00d)
#define VIC_SPR7_X       BYTE_FROM_ADDRESS(0xd00e)
#define VIC_SPR7_Y       BYTE_FROM_ADDRESS(0xd00f)
#define VIC_SPR_HI_X     BYTE_FROM_ADDRESS(0xd010)
#define VIC_SPR_ENA      BYTE_FROM_ADDRESS(0xd015)
#define VIC_SPR_EXP_Y    BYTE_FROM_ADDRESS(0xd017)
#define VIC_SPR_EXP_X    BYTE_FROM_ADDRESS(0xd01d)
#define VIC_SPR_MCOLOR   BYTE_FROM_ADDRESS(0xd01c)
#define VIC_SPR_BG_PRIO  BYTE_FROM_ADDRESS(0xd01b)

#define VIC_SPR_SP_COL   BYTE_FROM_ADDRESS(0xd01e)
#define VIC_SPR_BG_COL   BYTE_FROM_ADDRESS(0xd01f)

#define VIC_SPR_MCOLOR0  BYTE_FROM_ADDRESS(0xd025)
#define VIC_SPR_MCOLOR1  BYTE_FROM_ADDRESS(0xd026)

#define VIC_SPR0_COLOR   BYTE_FROM_ADDRESS(0xd027)
#define VIC_SPR1_COLOR   BYTE_FROM_ADDRESS(0xd028)
#define VIC_SPR2_COLOR   BYTE_FROM_ADDRESS(0xd029)
#define VIC_SPR3_COLOR   BYTE_FROM_ADDRESS(0xd02a)
#define VIC_SPR4_COLOR   BYTE_FROM_ADDRESS(0xd02b)
#define VIC_SPR5_COLOR   BYTE_FROM_ADDRESS(0xd02c)
#define VIC_SPR6_COLOR   BYTE_FROM_ADDRESS(0xd02d)
#define VIC_SPR7_COLOR   BYTE_FROM_ADDRESS(0xd02e)

#define SPR_X_SCREEN_LEFT   24
#define SPR_Y_SCREEN_TOP    50

#define VIC_CTRL1        BYTE_FROM_ADDRESS(0xd011)
#define VIC_CTRL2        BYTE_FROM_ADDRESS(0xd016)

#define VIC_HLINE        BYTE_FROM_ADDRESS(0xd012)

#define VIC_VIDEO_ADR    BYTE_FROM_ADDRESS(0xd018)

#define VIC_IRR          BYTE_FROM_ADDRESS(0xd019) // Interrupt request register
#define VIC_IMR          BYTE_FROM_ADDRESS(0xd01a) // Interrupt mask register

#define VIC_BORDERCOLOR  BYTE_FROM_ADDRESS(0xd020)
#define VIC_BG_COLOR0    BYTE_FROM_ADDRESS(0xd021)
#define VIC_BG_COLOR1    BYTE_FROM_ADDRESS(0xd022)
#define VIC_BG_COLOR2    BYTE_FROM_ADDRESS(0xd023)
#define VIC_BG_COLOR3    BYTE_FROM_ADDRESS(0xd024)


// ---------------------------------------------------------------------------
// I/O: SID

#define SID              BYTE_FROM_ADDRESS(0xd400)
#define SID_S1Lo         BYTE_FROM_ADDRESS(0xd400)
#define SID_S1Hi         BYTE_FROM_ADDRESS(0xd401)
#define SID_PB1Lo        BYTE_FROM_ADDRESS(0xd402)
#define SID_PB1Hi        BYTE_FROM_ADDRESS(0xd403)
#define SID_Ctl1         BYTE_FROM_ADDRESS(0xd404)
#define SID_AD1          BYTE_FROM_ADDRESS(0xd405)
#define SID_SUR1         BYTE_FROM_ADDRESS(0xd406)

#define SID_S2Lo         BYTE_FROM_ADDRESS(0xd407)
#define SID_S2Hi         BYTE_FROM_ADDRESS(0xd408)
#define SID_PB2Lo        BYTE_FROM_ADDRESS(0xd409)
#define SID_PB2Hi        BYTE_FROM_ADDRESS(0xd40A)
#define SID_Ctl2         BYTE_FROM_ADDRESS(0xd40B)
#define SID_AD2          BYTE_FROM_ADDRESS(0xd40C)
#define SID_SUR2         BYTE_FROM_ADDRESS(0xd40D)

#define SID_S3Lo         BYTE_FROM_ADDRESS(0xd40E)
#define SID_S3Hi         BYTE_FROM_ADDRESS(0xd40F)
#define SID_PB3Lo        BYTE_FROM_ADDRESS(0xd410)
#define SID_PB3Hi        BYTE_FROM_ADDRESS(0xd411)
#define SID_Ctl3         BYTE_FROM_ADDRESS(0xd412)
#define SID_AD3          BYTE_FROM_ADDRESS(0xd413)
#define SID_SUR3         BYTE_FROM_ADDRESS(0xd414)

#define SID_FltLo        BYTE_FROM_ADDRESS(0xd415)
#define SID_FltHi        BYTE_FROM_ADDRESS(0xd416)
#define SID_FltCtl       BYTE_FROM_ADDRESS(0xd417)
#define SID_Amp          BYTE_FROM_ADDRESS(0xd418)
#define SID_ADConv1      BYTE_FROM_ADDRESS(0xd419)
#define SID_ADConv2      BYTE_FROM_ADDRESS(0xd41A)
#define SID_Noise        BYTE_FROM_ADDRESS(0xd41B)
#define SID_Read3        BYTE_FROM_ADDRESS(0xd41C)

// ---------------------------------------------------------------------------
// I/O: CIAs

#define CIA1             BYTE_FROM_ADDRESS(0xdC00)
#define CIA1_PRA         BYTE_FROM_ADDRESS(0xdC00)
#define CIA1_PRB         BYTE_FROM_ADDRESS(0xdC01)
#define CIA1_DDRA        BYTE_FROM_ADDRESS(0xdC02)
#define CIA1_DDRB        BYTE_FROM_ADDRESS(0xdC03)
#define CIA1_TOD10       BYTE_FROM_ADDRESS(0xdC08)
#define CIA1_TODSEC      BYTE_FROM_ADDRESS(0xdC09)
#define CIA1_TODMIN      BYTE_FROM_ADDRESS(0xdC0A)
#define CIA1_TODHR       BYTE_FROM_ADDRESS(0xdC0B)
#define CIA1_ICR         BYTE_FROM_ADDRESS(0xdC0D)
#define CIA1_CRA         BYTE_FROM_ADDRESS(0xdC0E)
#define CIA1_CRB         BYTE_FROM_ADDRESS(0xdC0F)

#define CIA2             BYTE_FROM_ADDRESS(0xdD00)
#define CIA2_PRA         BYTE_FROM_ADDRESS(0xdD00)
#define CIA2_PRB         BYTE_FROM_ADDRESS(0xdD01)
#define CIA2_DDRA        BYTE_FROM_ADDRESS(0xdD02)
#define CIA2_DDRB        BYTE_FROM_ADDRESS(0xdD03)
#define CIA2_TOD10       BYTE_FROM_ADDRESS(0xdD08)
#define CIA2_TODSEC      BYTE_FROM_ADDRESS(0xdD09)
#define CIA2_TODMIN      BYTE_FROM_ADDRESS(0xdD0A)
#define CIA2_TODHR       BYTE_FROM_ADDRESS(0xdD0B)
#define CIA2_ICR         BYTE_FROM_ADDRESS(0xdD0D)
#define CIA2_CRA         BYTE_FROM_ADDRESS(0xdD0E)
#define CIA2_CRB         BYTE_FROM_ADDRESS(0xdD0F)

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
#define SCREEN_CHAR                   BYTE_FROM_ADDRESS(0xcc00)
#define SCREEN_CHAR_SIZE              BYTE_FROM_ADDRESS(0x400)

//address of color ram
#define SCREEN_COLOR                  BYTE_FROM_ADDRESS(0xd800)

#define SPRITE_GRAPHICS_TARGET        BYTE_FROM_ADDRESS(0xd000)
#define CHARACTER_GRAPHICS_TARGET     BYTE_FROM_ADDRESS(0xf000)
  
#define NUMBER_OF_HARDWARE_SPRITES     8

//address of sprite pointers
#define SPRITE_POINTER_BASE            SCREEN_CHAR + SCREEN_CHAR_SIZE - NUMBER_OF_HARDWARE_SPRITES

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
