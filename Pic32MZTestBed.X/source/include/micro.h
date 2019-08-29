/*
File:   micro.h
Author: Taylor Robbins
Date:   08\29\2019
*/

#ifndef _MICRO_H
#define _MICRO_H

// +--------------------------------------------------------------+
// |                      Public Definitions                      |
// +--------------------------------------------------------------+
#define MICRO_SYS_CLK_FREQ      200000000UL //200MHz
#define MICRO_PERF_BUS2_FREQ    (MICRO_SYS_CLK_FREQ / 2) //PB2DIV is set to 2x by default
#define MICRO_PERF_BUS3_FREQ    (MICRO_SYS_CLK_FREQ / 2) //PB3DIV is set to 2x by default
#define MICRO_ONE_MS_COUNT      ((MICRO_SYS_CLK_FREQ/2)/1000)

// +--------------------------------------------------------------+
// |                        Public Globals                        |
// +--------------------------------------------------------------+
extern u32 MicroDeviceID;
extern u8  MicroDeviceRevision;

// +--------------------------------------------------------------+
// |                        Public Macros                         |
// +--------------------------------------------------------------+
#define MicroClrWDT() { WDTCONbits.WDTCLRKEY = 0x5743; }

#define MicroEnableInterrupts()  asm volatile("ei")
#define MicroDisableInterrupts() asm volatile("di")

#define MicroRegUnlock() do                                  \
{                                                            \
	SYSKEY = 0x12345678; /*write invalid key to force lock*/ \
	SYSKEY = 0xAA996655; /*write Key1 to SYSKEY*/            \
	SYSKEY = 0x556699AA; /*write Key2 to SYSKEY*/            \
} while(0)

#define MicroRegLock() do \
{                         \
	SYSKEY = 0x00000000;  \
} while(0)

// +--------------------------------------------------------------+
// |                       Public Functions                       |
// +--------------------------------------------------------------+
void MicroInit();
u8   MicroDetectResetCause();
void MicroDelay(u32 delayMs);
void MicroReset();

// +--------------------------------------------------------------+
// |                          Pin Macros                          |
// +--------------------------------------------------------------+

//Output @TEST_LED1 H0 (Red)
#define TEST_LED1_VALUE LATHbits.LATH0
//Output @TEST_LED2 H1 (Yellow)
#define TEST_LED2_VALUE LATHbits.LATH1
//Output @TEST_LED3 H2 (Green)
#define TEST_LED3_VALUE LATHbits.LATH2

//Input @TEST_BTN1 B12 (S1)
#define TEST_BTN1_PULLUP CNPUBbits.CNPUB12
#define TEST_BTN1_VALUE  PORTBbits.   RB12
//Input @TEST_BTN2 B13 (S2)
#define TEST_BTN2_PULLUP CNPUBbits.CNPUB13
#define TEST_BTN2_VALUE  PORTBbits.   RB13
//Input @TEST_BTN3 B14 (S3)
#define TEST_BTN3_PULLUP CNPUBbits.CNPUB14
#define TEST_BTN3_VALUE  PORTBbits.   RB14

#endif //  _MICRO_H
