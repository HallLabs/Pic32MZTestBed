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
static bool btnWasDown2 = false;
static bool btnWasDown3 = false;

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
	// |        Blinky Example        |
	// +==============================+
	TEST_LED1_VALUE = ((TickCounterMs % 1000) >= 500) ? HIGH : LOW;
	TEST_LED2_VALUE = ((TickCounterMs % 400) >= 200) ? HIGH : LOW;
	TEST_LED3_VALUE = ((TickCounterSec % 6) >= 3) ? HIGH : LOW;
	
	// +==============================+
	// |        Button Example        |
	// +==============================+
	bool btn1 = (TEST_BTN1_VALUE == LOW);
	if (btn1 != btnWasDown1)
	{
		PrintLineAt(btn1 ? OutputLevel_Info : OutputLevel_Debug, "Button1 %s", btn1 ? "Pressed" : "Released");
		btnWasDown1 = btn1;
	}
	
	bool btn2 = (TEST_BTN2_VALUE == LOW);
	if (btn2 != btnWasDown2)
	{
		PrintLineAt(btn2 ? OutputLevel_Info : OutputLevel_Debug, "Button2 %s", btn2 ? "Pressed" : "Released");
		btnWasDown2 = btn2;
	}
	
	bool btn3 = (TEST_BTN3_VALUE == LOW);
	if (btn3 != btnWasDown3)
	{
		PrintLineAt(btn3 ? OutputLevel_Info : OutputLevel_Debug, "Button3 %s", btn3 ? "Pressed" : "Released");
		btnWasDown3 = btn3;
	}
	
	// +==============================+
	// |      Handle Debug Input      |
	// +==============================+
	char* newCommand = DebugUartReadLine();
	if (newCommand != nullptr)
	{
		HandleDebugCommand(newCommand);
	}
}