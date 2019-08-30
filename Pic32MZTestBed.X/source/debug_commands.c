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
		WriteLine_I("status : Prints out some information about the state of the unit");
		WriteLine_I("test : Used for random tests");
		WriteLine_I("reset : Reset the controller");
		WriteLine_I("buttons : Prints out the current state of the buttons");
		WriteLine_I("pin [number] [value] : Manually change one of the test pins to 1 (HIGH) or 0 (LOW) output value");
	}
	
	// +==============================+
	// |            status            |
	// +==============================+
	else if (strcmp(commandStr, "status") == 0)
	{
		PrintLine_N("PIC32MZ Test Bed v%u.%u(%u)", Version.major, Version.minor, Version.build);
		Write_I("Time: "); PrintFormattedMilliseconds(OutputLevel_Info, TickCounterMs); WriteLine_I("");
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
		bool btn2 = (TEST_BTN2_VALUE == LOW);
		bool btn3 = (TEST_BTN3_VALUE == LOW);
		PrintLineAt(btn1 ? OutputLevel_Info : OutputLevel_Debug, "Button 1: %s", btn1 ? "Pressed" : "Released");
		PrintLineAt(btn2 ? OutputLevel_Info : OutputLevel_Debug, "Button 2: %s", btn2 ? "Pressed" : "Released");
		PrintLineAt(btn3 ? OutputLevel_Info : OutputLevel_Debug, "Button 3: %s", btn3 ? "Pressed" : "Released");
	}
	
	// +==============================+
	// |     pin [number] [value]     |
	// +==============================+
	else if (commandLength >= 4 && strncmp(commandStr, "pin ", 4) == 0)
	{
		const char* parts[4];
		u32 partLengths[4];
		u32 numParts = SplitNtString(&commandStr[4], ' ', &parts[0], &partLengths[0], ArrayCount(parts));
		if (numParts != 2 || partLengths[0] == 0 || partLengths[1] == 0) { WriteLine_E("Usage: pin [number] [value]"); return; }
		i32 pinNumberI32 = 0;
		if (!TryParseInt32(parts[0], partLengths[0], &pinNumberI32) || pinNumberI32 < 1 || pinNumberI32 > 6) { PrintLine_E("Invalid pin number given \"%.*s\"", partLengths[0], parts[0]); return; }
		i32 valueI32 = 0;
		if (!TryParseInt32(parts[1], partLengths[1], &valueI32) || valueI32 < 0 || valueI32 > 1) { PrintLine_E("Invalid value given \"%.*s\"", partLengths[1], parts[1]); return; }
		
		PrintLine_I("Setting TEST_PIN%d %s", pinNumberI32, (valueI32 > 0) ? "HIGH" : "LOW");
		switch (pinNumberI32)
		{
			case 1: TEST_PIN1_VALUE = (valueI32 > 0) ? HIGH : LOW; break;
			case 2: TEST_PIN2_VALUE = (valueI32 > 0) ? HIGH : LOW; break;
			case 3: TEST_PIN3_VALUE = (valueI32 > 0) ? HIGH : LOW; break;
			case 4: TEST_PIN4_VALUE = (valueI32 > 0) ? HIGH : LOW; break;
			case 5: TEST_PIN5_VALUE = (valueI32 > 0) ? HIGH : LOW; break;
			case 6: TEST_PIN6_VALUE = (valueI32 > 0) ? HIGH : LOW; break;
			default: Assert(false); break;
		}
	}
	
	// +==============================+
	// |       Unknown Command        |
	// +==============================+
	else
	{
		WriteLine_E("Unknown Command");
	}
}
