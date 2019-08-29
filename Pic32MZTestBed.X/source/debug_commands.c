/*
File:   debug_commands.c
Author: Taylor Robbins
Date:   08\29\2019
Description: 
	** Holds the HandleDebugCommand function
*/

#include "app.h"
#include "debug_commands.h"

#include "micro.h"
#include "debug.h"
#include "tick_timer.h"
#include "helpers.h"

// +--------------------------------------------------------------+
// |                       Public Functions                       |
// +--------------------------------------------------------------+
void HandleDebugCommand(const char* commandStr)
{
	Assert(commandStr != nullptr);
	u32 commandLength = (u32)strlen(commandStr);
	
	// +==============================+
	// |             help             |
	// +==============================+
	if (strcmp(commandStr, "help") == 0)
	{
		WriteLine_I("test : Used for random tests");
		WriteLine_I("reset : Reset the controller");
		WriteLine_I("buttons : Prints out the current state of the buttons");
	}
	
	// +==============================+
	// |             test             |
	// +==============================+
	else if (strcmp(commandStr, "test") == 0)
	{
		WriteLine_E("Nothing to test right now");
		//TODO: Do any tests you want with this command
	}
	
	// +==============================+
	// |            reset             |
	// +==============================+
	else if (strcmp(commandStr, "reset") == 0)
	{
		WriteLine_I("Resetting...");
		DebugUartFlush();
		MicroReset();
	}
	
	// +==============================+
	// |           buttons            |
	// +==============================+
	else if (strcmp(commandStr, "buttons") == 0)
	{
		bool btn1 = (TEST_BTN1_VALUE == LOW);
		bool btn2 = (TEST_BTN1_VALUE == LOW);
		bool btn3 = (TEST_BTN1_VALUE == LOW);
		PrintLineAt(btn1 ? OutputLevel_Info : OutputLevel_Debug, "Button 1: %s", btn1 ? "Pressed" : "Released");
		PrintLineAt(btn2 ? OutputLevel_Info : OutputLevel_Debug, "Button 2: %s", btn2 ? "Pressed" : "Released");
		PrintLineAt(btn3 ? OutputLevel_Info : OutputLevel_Debug, "Button 3: %s", btn3 ? "Pressed" : "Released");
	}
	
	// +==============================+
	// |       Unknown Command        |
	// +==============================+
	else
	{
		WriteLine_E("Unknown Command");
	}
}
