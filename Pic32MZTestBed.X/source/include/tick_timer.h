/*
File:   tick_timer.h
Author: Taylor Robbins
Date:   08\29\2019
*/

#ifndef _TICK_TIMER_H
#define _TICK_TIMER_H

// +--------------------------------------------------------------+
// |                        Public Globals                        |
// +--------------------------------------------------------------+
extern volatile u32 TickCounterMs;  //Loops in 49.7 days
extern volatile u32 TickCounterSec; //Loops in 136.19 years

// +==============================+
// |     Ms Countdown Timers      |
// +==============================+
extern volatile u32 DebugOutputBackoff;

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
void TickTimerInit();
u32 TimeSinceMs(u32 counterValueMs);
u32 TimeSinceSec(u32 counterValueSec);

#endif //  _TICK_TIMER_H
