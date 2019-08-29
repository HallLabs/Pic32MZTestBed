/*
File:   micro.c
Author: Taylor Robbins
Date:   08\29\2019
Description: 
	** Holds a bunch of functions that help us configure and use various microchip features.
	** MicroInit contains the code for configuring all the pins and remapping peripherals at startup
	** micro.h contains all of the defines we use to interact with GPIO pins
	** This file also contains the #pragma config macros which define the way the chip is configured on startup
*/

#include "app.h"
#include "micro.h"

// +--------------------------------------------------------------+
// |                        Public Globals                        |
// +--------------------------------------------------------------+
u32 MicroDeviceID       = 0xFFFFFFFF;
u8  MicroDeviceRevision = 0xFF;

// +--------------------------------------------------------------+
// |                       Public Functions                       |
// +--------------------------------------------------------------+
void MicroInit()
{
	MicroDeviceID = DEVIDbits.DEVID;
	MicroDeviceRevision = DEVIDbits.VER;
	
	// +==============================+
	// |       IO Configuration       |
	// +==============================+
	{
		// All N/C pins are configured to analog inputs
		ANSELA = 0b00000000000000000000000000000000;
		ANSELB = 0b00000000000000000000000000000000;
		ANSELC = 0b00000000000000000000000000000000;
		ANSELD = 0b00000000000000000000000000000000;
		ANSELE = 0b00000000000000000000000000000000;
		ANSELF = 0b00000000000000000000000000000000;
		ANSELG = 0b00000000000000000000000000000000;
		ANSELH = 0b00000000000000000000000000000000;
		ANSELJ = 0b00000000000000000000000000000000;
		// ANSELK = 0b00000000000000000000000000000000; //no ANSELK register I guess
		
		TRISA = (
			(INPUT  << _TRISA_TRISA0_POSITION)  | // N/C
			(INPUT  << _TRISA_TRISA1_POSITION)  | // N/C
			(INPUT  << _TRISA_TRISA2_POSITION)  | // N/C
			(INPUT  << _TRISA_TRISA3_POSITION)  | // N/C
			(INPUT  << _TRISA_TRISA4_POSITION)  | // N/C
			(INPUT  << _TRISA_TRISA5_POSITION)  | // N/C
			(INPUT  << _TRISA_TRISA6_POSITION)  | // N/C
			(INPUT  << _TRISA_TRISA7_POSITION)  | // N/C
			(INPUT  << _TRISA_TRISA9_POSITION)  | // N/C
			(INPUT  << _TRISA_TRISA10_POSITION) | // N/C
			(INPUT  << _TRISA_TRISA14_POSITION) | // N/C
			(INPUT  << _TRISA_TRISA15_POSITION)   // N/C
		);
		
		TRISB = (
			(INPUT  << _TRISB_TRISB0_POSITION)  | // @ICSP_CLOCK
			(INPUT  << _TRISB_TRISB1_POSITION)  | // @ICSP_DATA
			(INPUT  << _TRISB_TRISB2_POSITION)  | // N/C
			(INPUT  << _TRISB_TRISB3_POSITION)  | // N/C
			(INPUT  << _TRISB_TRISB4_POSITION)  | // N/C
			(INPUT  << _TRISB_TRISB5_POSITION)  | // N/C
			(INPUT  << _TRISB_TRISB6_POSITION)  | // N/C
			(INPUT  << _TRISB_TRISB7_POSITION)  | // N/C
			(INPUT  << _TRISB_TRISB8_POSITION)  | // N/C
			(INPUT  << _TRISB_TRISB9_POSITION)  | // N/C
			(INPUT  << _TRISB_TRISB10_POSITION) | // N/C
			(INPUT  << _TRISB_TRISB11_POSITION) | // N/C
			(INPUT  << _TRISB_TRISB12_POSITION) | // @TEST_BTN1 (SW1)
			(INPUT  << _TRISB_TRISB13_POSITION) | // @TEST_BTN2 (SW2)
			(INPUT  << _TRISB_TRISB14_POSITION) | // @TEST_BTN3 (SW3)
			(INPUT  << _TRISB_TRISB15_POSITION)   // N/C
		);
		
		TRISC = (
			(INPUT  << _TRISC_TRISC1_POSITION)  | // N/C
			(INPUT  << _TRISC_TRISC2_POSITION)  | // N/C
			(INPUT  << _TRISC_TRISC3_POSITION)  | // N/C
			(INPUT  << _TRISC_TRISC4_POSITION)  | // N/C
			(INPUT  << _TRISC_TRISC12_POSITION) | // N/C
			(INPUT  << _TRISC_TRISC13_POSITION) | // N/C
			(INPUT  << _TRISC_TRISC14_POSITION) | // N/C
			(INPUT  << _TRISC_TRISC15_POSITION)   // N/C
		);
		
		TRISD = (
			(INPUT  << _TRISD_TRISD0_POSITION)  | // N/C
			(INPUT  << _TRISD_TRISD1_POSITION)  | // N/C
			(INPUT  << _TRISD_TRISD2_POSITION)  | // N/C
			(INPUT  << _TRISD_TRISD3_POSITION)  | // N/C
			(INPUT  << _TRISD_TRISD4_POSITION)  | // N/C
			(INPUT  << _TRISD_TRISD5_POSITION)  | // N/C
			(INPUT  << _TRISD_TRISD6_POSITION)  | // N/C
			(INPUT  << _TRISD_TRISD7_POSITION)  | // N/C
			(INPUT  << _TRISD_TRISD9_POSITION)  | // N/C
			(INPUT  << _TRISD_TRISD10_POSITION) | // N/C
			(INPUT  << _TRISD_TRISD11_POSITION) | // N/C
			(INPUT  << _TRISD_TRISD12_POSITION) | // N/C
			(INPUT  << _TRISD_TRISD13_POSITION) | // N/C
			(INPUT  << _TRISD_TRISD14_POSITION) | // N/C
			(INPUT  << _TRISD_TRISD15_POSITION)   // N/C
		);
		
		TRISE = (
			(INPUT  << _TRISE_TRISE0_POSITION)  | // N/C
			(INPUT  << _TRISE_TRISE1_POSITION)  | // N/C
			(INPUT  << _TRISE_TRISE2_POSITION)  | // N/C
			(INPUT  << _TRISE_TRISE3_POSITION)  | // N/C
			(INPUT  << _TRISE_TRISE4_POSITION)  | // N/C
			(INPUT  << _TRISE_TRISE5_POSITION)  | // N/C
			(INPUT  << _TRISE_TRISE6_POSITION)  | // N/C
			(INPUT  << _TRISE_TRISE7_POSITION)  | // N/C
			(INPUT  << _TRISE_TRISE8_POSITION)  | // N/C
			(INPUT  << _TRISE_TRISE9_POSITION)    // N/C
		);
		
		TRISF = (
			(INPUT  << _TRISF_TRISF0_POSITION)  | // N/C
			(INPUT  << _TRISF_TRISF1_POSITION)  | // N/C
			(INPUT  << _TRISF_TRISF2_POSITION)  | // N/C
			(INPUT  << _TRISF_TRISF3_POSITION)  | // N/C
			(INPUT  << _TRISF_TRISF4_POSITION)  | // @DEBUG_UART_RX (U5RX)
			(OUTPUT << _TRISF_TRISF5_POSITION)  | // @DEBUG_UART_TX (U5TX)
			(INPUT  << _TRISF_TRISF8_POSITION)  | // N/C
			(INPUT  << _TRISF_TRISF12_POSITION) | // N/C
			(INPUT  << _TRISF_TRISF13_POSITION)   // N/C
		);
		
		TRISG = (
			(INPUT  << _TRISG_TRISG0_POSITION)  | // N/C
			(INPUT  << _TRISG_TRISG1_POSITION)  | // N/C
			(INPUT  << _TRISG_TRISG6_POSITION)  | // N/C
			(INPUT  << _TRISG_TRISG7_POSITION)  | // N/C
			(INPUT  << _TRISG_TRISG8_POSITION)  | // N/C
			(INPUT  << _TRISG_TRISG9_POSITION)  | // N/C
			(INPUT  << _TRISG_TRISG12_POSITION) | // N/C
			(INPUT  << _TRISG_TRISG13_POSITION) | // N/C
			(INPUT  << _TRISG_TRISG14_POSITION) | // N/C
			(INPUT  << _TRISG_TRISG15_POSITION)   // N/C
		);
		
		TRISH = (
			(OUTPUT << _TRISH_TRISH0_POSITION)  | // @TEST_LED1 (Red)
			(OUTPUT << _TRISH_TRISH1_POSITION)  | // @TEST_LED2 (Yellow)
			(OUTPUT << _TRISH_TRISH2_POSITION)  | // @TEST_LED3 (Green)
			(INPUT  << _TRISH_TRISH3_POSITION)  | // N/C
			(INPUT  << _TRISH_TRISH4_POSITION)  | // N/C
			(INPUT  << _TRISH_TRISH5_POSITION)  | // N/C
			(INPUT  << _TRISH_TRISH6_POSITION)  | // N/C
			(INPUT  << _TRISH_TRISH7_POSITION)  | // N/C
			(INPUT  << _TRISH_TRISH8_POSITION)  | // N/C
			(INPUT  << _TRISH_TRISH9_POSITION)  | // N/C
			(INPUT  << _TRISH_TRISH10_POSITION) | // N/C
			(INPUT  << _TRISH_TRISH11_POSITION) | // N/C
			(INPUT  << _TRISH_TRISH12_POSITION) | // N/C
			(INPUT  << _TRISH_TRISH13_POSITION) | // N/C
			(INPUT  << _TRISH_TRISH14_POSITION) | // N/C
			(INPUT  << _TRISH_TRISH15_POSITION)   // N/C
		);
		
		TRISJ = (
			(INPUT  << _TRISJ_TRISJ0_POSITION)  | // N/C
			(INPUT  << _TRISJ_TRISJ1_POSITION)  | // N/C
			(INPUT  << _TRISJ_TRISJ2_POSITION)  | // N/C
			(INPUT  << _TRISJ_TRISJ3_POSITION)  | // N/C
			(INPUT  << _TRISJ_TRISJ4_POSITION)  | // N/C
			(INPUT  << _TRISJ_TRISJ5_POSITION)  | // N/C
			(INPUT  << _TRISJ_TRISJ6_POSITION)  | // N/C
			(INPUT  << _TRISJ_TRISJ7_POSITION)  | // N/C
			(INPUT  << _TRISJ_TRISJ8_POSITION)  | // N/C
			(INPUT  << _TRISJ_TRISJ9_POSITION)  | // N/C
			(INPUT  << _TRISJ_TRISJ10_POSITION) | // N/C
			(INPUT  << _TRISJ_TRISJ11_POSITION) | // N/C
			(INPUT  << _TRISJ_TRISJ12_POSITION) | // N/C
			(INPUT  << _TRISJ_TRISJ13_POSITION) | // N/C
			(INPUT  << _TRISJ_TRISJ14_POSITION) | // N/C
			(INPUT  << _TRISJ_TRISJ15_POSITION)   // N/C
		);
		
		TRISK = (
			(INPUT  << _TRISK_TRISK0_POSITION)  | // N/C
			(INPUT  << _TRISK_TRISK1_POSITION)  | // N/C
			(INPUT  << _TRISK_TRISK2_POSITION)  | // N/C
			(INPUT  << _TRISK_TRISK3_POSITION)  | // N/C
			(INPUT  << _TRISK_TRISK4_POSITION)  | // N/C
			(INPUT  << _TRISK_TRISK5_POSITION)  | // N/C
			(INPUT  << _TRISK_TRISK6_POSITION)  | // N/C
			(INPUT  << _TRISK_TRISK7_POSITION)    // N/C
		);
		
		TEST_LED1_VALUE = HIGH;
		TEST_LED2_VALUE = HIGH;
		TEST_LED3_VALUE = HIGH;
		
		TEST_BTN1_PULLUP = ENABLED;
		TEST_BTN2_PULLUP = ENABLED;
		TEST_BTN3_PULLUP = ENABLED;
	}
	
	// +==============================+
	// |    Peripheral Pin Mapping    |
	// +==============================+
	{
		//UART5 Remap
		U5RXRbits.U5RXR = 0b0010; // U5RX mapped to RF4
		RPF5Rbits.RPF5R = 0b0011; // RF5 mapped to U5TX
	}
	
	// Enable Multi-Vector Interrupt Mode
	INTCONbits.MVEC = ENABLED;
}

u8 MicroDetectResetCause()
{
	u8 result = 0x00;
	if (RCONbits.POR) { result |= ResetCause_PowerOn; RCONbits.POR = CLEARED; }
	if (RCONbits.BOR) { if (!IsFlagSet(result, ResetCause_PowerOn)) { result |= ResetCause_BrownOut; } RCONbits.BOR = CLEARED; }
	if (RCONbits.IDLE)  { result |= ResetCause_WakeFromIdle; }
	if (RCONbits.SLEEP) { result |= ResetCause_WakeFromSleep; }
	if (RCONbits.WDTO)  { result |= ResetCause_WatchdogTimer; RCONbits.WDTO = CLEARED; }
	if (RCONbits.DMTO)  { result |= ResetCause_DeadmanTimer; }
	if (RCONbits.SWR)   { result |= ResetCause_SoftwareReset; }
	if (RCONbits.EXTR)  { result |= ResetCause_ExternalReset; }
	// if (RCONbits.CMR) { result |= ResetCause_ConfigurationMismatch; } //NOTE: We don't record this one since we only have 8 bits
	return result;
}

void MicroDelay(u32 delayMs)
{
	_CP0_SET_COUNT(0);
	while (_CP0_GET_COUNT() < delayMs*MICRO_ONE_MS_COUNT) { Nop(); }
}

void MicroReset()
{
	MicroDisableInterrupts();
	MicroRegUnlock();
	RSWRSTbits.SWRST = 1; // Set SWRST bit to arm reset.
	(void)RSWRST; // Read RSWRST register to trigger reset.
	while(FOREVER) { Nop(); } // Prevent any unwanted code execution until reset occurs.
}

// +--------------------------------------------------------------+
// |       Default Interrupt and General Exception Handler        |
// +--------------------------------------------------------------+
/* Code identifying the cause of the exception (CP0 Cause register). */
static u32 excep_code;
/* Address of instruction that caused the exception. */
static u32 excep_addr;
/* Pointer to the string describing the cause of the exception. */
static char* cause_str;
/* Array identifying the cause (indexed by _exception_code). */
static char* cause[] =
{
	"Interrupt",
	"Undefined",
	"Undefined",
	"Undefined",
	"Load/fetch address error",
	"Store address error",
	"Instruction bus error",
	"Data bus error",
	"Syscall",
	"Breakpoint",
	"Reserved instruction",
	"Coprocessor unusable",
	"Arithmetic overflow",
	"Trap",
	"Reserved",
	"Reserved",
	"Reserved",
	"Reserved",
	"Reserved"
};

void __attribute__((__interrupt__)) _DefaultInterrupt()
{
	// Mask off Mask of the ExcCode Field from the Cause Register.
	// Refer to the MIPs Software User's manual.
	excep_code = (_CP0_GET_CAUSE() & 0x0000007C) >> 2;
	excep_addr = _CP0_GET_EPC();
	cause_str  = cause[excep_code];
	// Assert(false); //TODO: Uncomment me!
	MicroReset();
}

void _general_exception_handler()
{
	// Mask off Mask of the ExcCode Field from the Cause Register.
	// Refer to the MIPs Software User's manual.
	excep_code = (_CP0_GET_CAUSE() & 0x0000007C) >> 2;
	excep_addr = _CP0_GET_EPC();
	cause_str  = cause[excep_code];
	// Assert(false); //TODO: Uncomment me!
	MicroReset();
}

//+---------------------------------------------------------------+
//|                      Device Configuration                     |
//+---------------------------------------------------------------+
// +==============================+
// |           DEVCFG0            |
// +==============================+
#pragma config DEBUG =      OFF
#pragma config JTAGEN =     OFF
#pragma config ICESEL =     ICS_PGx2 //NOTE: Our boards normally use ICS_PGx1
#pragma config TRCEN =      OFF
#pragma config BOOTISA =    MIPS32
#pragma config FECCCON =    OFF_UNLOCKED
#pragma config FSLEEP =     OFF
#pragma config DBGPER =     PG_ALL
#pragma config SMCLR =      MCLR_NORM
#pragma config SOSCGAIN =   GAIN_2X
#pragma config SOSCBOOST =  ON
#pragma config POSCGAIN =   GAIN_2X
#pragma config POSCBOOST =  ON
#pragma config EJTAGBEN =   NORMAL
#ifdef __DEBUG
#pragma config CP =         OFF
#else
#pragma config CP =         ON
#endif

// +==============================+
// |           DEVCFG1            |
// +==============================+
#pragma config FNOSC =      SPLL
#pragma config DMTINTV =    WIN_127_128
#pragma config FSOSCEN =    OFF
#pragma config IESO =       OFF
#pragma config POSCMOD =    OFF
#pragma config OSCIOFNC =   OFF
#pragma config FCKSM =      CSECME
#pragma config WDTPS =      PS4096
#pragma config WDTSPGM =    STOP
#pragma config FWDTEN =     OFF
#pragma config WINDIS =     NORMAL
#pragma config FWDTWINSZ =  WINSZ_25
#pragma config DMTCNT =     DMT31
#pragma config FDMTEN =     OFF

// +==============================+
// |           DEVCFG2            |
// +==============================+
#pragma config FPLLIDIV =   DIV_1
#pragma config FPLLRNG =    RANGE_5_10_MHZ
#pragma config FPLLICLK =   PLL_FRC
#if 0 //24MHz clock
#pragma config FPLLMULT =   MUL_12
#pragma config FPLLODIV =   DIV_4
#else //200MHz clock
#pragma config FPLLMULT =   MUL_50
#pragma config FPLLODIV =   DIV_2
#endif
#pragma config UPLLFSEL =   FREQ_24MHZ

// +==============================+
// |           DEVCFG3            |
// +==============================+
#pragma config USERID =     0xffff
#pragma config FMIIEN =     ON
#pragma config FETHIO =     ON
#pragma config PGL1WAY =    ON
#pragma config PMDL1WAY =   ON
#pragma config IOL1WAY =    ON
#pragma config FUSBIDIO =   ON

// +==============================+
// |           BF1SEQ0            |
// +==============================+
#pragma config TSEQ =       0x0000
#pragma config CSEQ =       0xffff
