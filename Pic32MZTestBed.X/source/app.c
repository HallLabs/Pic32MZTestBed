/*
File:   app.c
Author: Taylor Robbins
Date:   08\29\2019
Description: 
	** Holds the main app functions as well as all of the global variables and functions that are used throughout the project
*/

#include "app.h"

#include "version.h"
#include "micro.h"
#include "debug.h"
#include "tick_timer.h"
#include "debug_commands.h"
#include "helpers.h"

// +--------------------------------------------------------------+
// |                        Public Globals                        |
// +--------------------------------------------------------------+
Version_t Version = { VERSION_MAJOR, VERSION_MINOR, VERSION_BUILD };

// +--------------------------------------------------------------+
// |                       Private Globals                        |
// +--------------------------------------------------------------+
static bool btnWasDown1 = false;
static bool btnDebounceTriggered1 = false;
static bool btnWasDown2 = false;
static bool btnDebounceTriggered2 = false;
static bool btnWasDown3 = false;
static bool btnDebounceTriggered3 = false;

// +--------------------------------------------------------------+
// |                        Initialization                        |
// +--------------------------------------------------------------+
void AppInitialize()
{
	MicroDelay(100); //DElay to try and let things settle before trying to do debug output
	PrintLine("\n+==============================+");
	PrintLine("|  PIC32MZ Test Bed v%u.%u(%3u)  |", Version.major, Version.minor, Version.build);
	PrintLine("+==============================+");
	#if PRODUCTION_MODE
	WriteLineAt(OutputLevel_Error, "Production Mode!");
	#endif
	
	//TODO: Any initialization can be done here
}

// +--------------------------------------------------------------+
// |                            Update                            |
// +--------------------------------------------------------------+
void AppUpdate()
{
	// +==============================+
	// |        Button Example        |
	// +==============================+
	bool btnDown1 = (TEST_BTN1_VALUE == LOW);
	if (btnDown1 != btnWasDown1 && ButtonDebounceTimer1 == 0)
	{
		PrintLineAt(btnDown1 ? OutputLevel_Info : OutputLevel_Debug, "Button1 %s", btnDown1 ? "Pressed" : "Released");
		btnWasDown1 = btnDown1;
		ButtonDebounceTimer1 = BUTTON_DEBOUNCE_TIME;
	}
	
	bool btnDown2 = (TEST_BTN2_VALUE == LOW);
	if (btnDown2 != btnWasDown2 && ButtonDebounceTimer2 == 0)
	{
		PrintLineAt(btnDown2 ? OutputLevel_Info : OutputLevel_Debug, "Button2 %s", btnDown2 ? "Pressed" : "Released");
		btnWasDown2 = btnDown2;
		ButtonDebounceTimer2 = BUTTON_DEBOUNCE_TIME;
	}
	
	bool btnDown3 = (TEST_BTN3_VALUE == LOW);
	if (btnDown3 != btnWasDown3 && ButtonDebounceTimer3 == 0)
	{
		PrintLineAt(btnDown3 ? OutputLevel_Info : OutputLevel_Debug, "Button3 %s", btnDown3 ? "Pressed" : "Released");
		btnWasDown3 = btnDown3;
		ButtonDebounceTimer3 = BUTTON_DEBOUNCE_TIME;
	}
	
	// +==============================+
	// |        Blinky Example        |
	// +==============================+
	TEST_LED1_VALUE = ((TickCounterMs % 1000) >= 500) ? HIGH : LOW;
	TEST_LED2_VALUE = ((TickCounterMs % 400) >= 200) ? HIGH : LOW;
	TEST_LED3_VALUE = btnDown3 ? HIGH : LOW;
	
	// +==============================+
	// |      Handle Debug Input      |
	// +==============================+
	char* newCommand = DebugUartReadLine();
	if (newCommand != nullptr)
	{
		HandleDebugCommand(newCommand);
	}
}