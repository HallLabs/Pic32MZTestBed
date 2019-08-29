/*
File:   main.c
Author: Taylor Robbins
Date:   08\29\2019
Description: 
	** Holds the main entry point for the program which contains the main loop for the project.
	** It also calls all of the initialization functions
*/

#include "app.h"

#include "micro.h"
#include "debug.h"
#include "tick_timer.h"

// +--------------------------------------------------------------+
// |                       Main Entry Point                       |
// +--------------------------------------------------------------+
int main(void)
{
	// +==============================+
	// |        Initialization        |
	// +==============================+
	u8 resetCauses = MicroDetectResetCause();
	MicroDisableInterrupts();
	MicroInit();
	TickTimerInit();
	DebugUartInit();
	MicroEnableInterrupts();
	
	AppInitialize();
	
	// +==============================+
	// |          Main Loop           |
	// +==============================+
	while (FOREVER)
	{
		AppUpdate();
		DebugUartUpdate();
	}
	
	// Should never get here.
	return 0;
}
