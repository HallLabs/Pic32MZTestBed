/*
File:   tick_timer.c
Author: Taylor Robbins
Date:   08\29\2019
Description: 
	** Uses Timer9 as a tick timer to keep track of time with a 1ms accuracy
*/

#include "app.h"
#include "tick_timer.h"

// +--------------------------------------------------------------+
// |                     Private Definitions                      |
// +--------------------------------------------------------------+
#define TICK_TIMER_ISR_PERIOD   1
#define TICK_TIMER_PERIOD_TWEAK 6
#define TICK_TIMER_PR_VALUE     3132//((TICK_TIMER_ISR_PERIOD * (MICRO_PERF_BUS_FREQ / 32L) / 1000L) - TICK_TIMER_PERIOD_TWEAK - 1)

// +--------------------------------------------------------------+
// |                        Public Globals                        |
// +--------------------------------------------------------------+
volatile u32 TickCounterMs  = 0; //Loops in 49.7 days
volatile u32 TickCounterSec = 0; //Loops in 136.19 years

// +==============================+
// |     Ms Countdown Timers      |
// +==============================+
volatile u32 DebugOutputBackoff = 0;
volatile u32 ButtonDebounceTimer1 = 0;
volatile u32 ButtonDebounceTimer2 = 0;
volatile u32 ButtonDebounceTimer3 = 0;

// +==============================+
// |      Ms Countup Timers       |
// +==============================+
//TODO: Add Ms Countup timers here

// +==============================+
// |     Sec Countdown Timers     |
// +==============================+
//TODO: Add Sec Countdown timers here

// +==============================+
// |      Sec Countup Timers      |
// +==============================+
//TODO: Add Sec Countup timers here

// +--------------------------------------------------------------+
// |                       Public Functions                       |
// +--------------------------------------------------------------+
void TickTimerInit()
{
	//+===============================+
	//|         Timer9 Init           |
	//+===============================+
	T9CONbits.SIDL  = 0; // Continue in idle mode.
	T9CONbits.TCKPS = 0b101; // Pre-scaler of 32. If this is changed, change the PR9 calculation.
	PR9 = TICK_TIMER_PR_VALUE; //Write the period register
	IPC10bits.T9IP  = 2; IPC10bits.T9IS = 0; //Int priority 2.0
	
	IFS1bits.T9IF = CLEARED; //Clear ISR flag
	IEC1bits.T9IE = ENABLED; //Enable ISR
	T9CONbits.ON  = ENABLED; //Enable timer
}

u32 TimeSinceMs(u32 counterValueMs)
{
	if (TickCounterMs >= counterValueMs)
	{
		return (TickCounterMs - counterValueMs);
	}
	else
	{
		return (UINT32_MAX - counterValueMs) + TickCounterMs;
	}
}
u32 TimeSinceSec(u32 counterValueSec)
{
	if (TickCounterSec >= counterValueSec)
	{
		return (TickCounterSec - counterValueSec);
	}
	else
	{
		return (UINT32_MAX - counterValueSec) + TickCounterSec;
	}
}

// +--------------------------------------------------------------+
// |                        Tick Timer ISR                        |
// +--------------------------------------------------------------+
static volatile u16 secCounter = 0;
void __ISR(_TIMER_9_VECTOR, ipl2AUTO) TickTimerIsr(void)
{
	// +==============================+
	// |      Millisecond Timers      |
	// +==============================+
	TickCounterMs++;
	Decrement(DebugOutputBackoff);
	Decrement(ButtonDebounceTimer1);
	Decrement(ButtonDebounceTimer2);
	Decrement(ButtonDebounceTimer3);
	
	//TODO: Add Ms Countup timers here
	
	secCounter++;
	if (secCounter >= 1000)
	{
		secCounter -= 1000;
		
		// +==============================+
		// |        Second Timers         |
		// +==============================+
		TickCounterSec++;
		
		//TODO: Add Sec Countdown timers here
		
		//TODO: Add Sec Countup timers here
	}
	
	IFS1CLR = _IFS1_T9IF_MASK;
}
