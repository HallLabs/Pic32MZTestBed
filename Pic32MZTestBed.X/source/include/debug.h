/*
File:   debug.h
Author: Taylor Robbins
Date:   08\29\2019
*/

#ifndef _DEBUG_H
#define _DEBUG_H

// +--------------------------------------------------------------+
// |                       Public Functions                       |
// +--------------------------------------------------------------+
void  DebugUartInit();
bool  DebugUartTxPut(u8 newByte);
bool  DebugUartTxPutBytes(const u8* dataPntr, u32 dataLength);
void  DebugPutByte(u8 newByte);
void  DebugUartWrite(const char* rawFileName, OutputLevel_t outputLevel, bool newLine, const char* string);
void  DebugUartPrint(const char* rawFileName, OutputLevel_t outputLevel, bool newLine, const char* formatStr, ...);
void  DebugUartFlush();
char* DebugUartReadLine();
u32   DebugUartRxLength();
char  DebugUartRxGet(u32 offset);
void  DebugUartUpdate();

// +--------------------------------------------------------------+
// |                        Public Macros                         |
// +--------------------------------------------------------------+
#define Write(string)             DebugUartWrite(__FILE__, OutputLevel_None, false, (string))
#define WriteLine(string)         DebugUartWrite(__FILE__, OutputLevel_None, true, (string))
#define Print(formatStr, ...)     DebugUartPrint(__FILE__, OutputLevel_None, false, (formatStr), ##__VA_ARGS__)
#define PrintLine(formatStr, ...) DebugUartPrint(__FILE__, OutputLevel_None, true, (formatStr), ##__VA_ARGS__)

#define WriteAt(outputLevel, string)             DebugUartWrite(__FILE__, outputLevel, false, (string))
#define WriteLineAt(outputLevel, string)         DebugUartWrite(__FILE__, outputLevel, true, (string))
#define PrintAt(outputLevel, formatStr, ...)     DebugUartPrint(__FILE__, outputLevel, false, (formatStr), ##__VA_ARGS__)
#define PrintLineAt(outputLevel, formatStr, ...) DebugUartPrint(__FILE__, outputLevel, true, (formatStr), ##__VA_ARGS__)

#if DEBUG_LEVEL_OUTPUT_ENABLED
	#define Write_D(string)             DebugUartWrite(__FILE__, OutputLevel_Debug, false, (string))
	#define WriteLine_D(string)         DebugUartWrite(__FILE__, OutputLevel_Debug, true, (string))
	#define Print_D(formatStr, ...)     DebugUartPrint(__FILE__, OutputLevel_Debug, false, (formatStr), ##__VA_ARGS__)
	#define PrintLine_D(formatStr, ...) DebugUartPrint(__FILE__, OutputLevel_Debug, true, (formatStr), ##__VA_ARGS__)
#else
	#define Write_D(string)             //Nothing
	#define WriteLine_D(string)         //Nothing
	#define Print_D(formatStr, ...)     //Nothing
	#define PrintLine_D(formatStr, ...) //Nothing
#endif

#if INFO_LEVEL_OUTPUT_ENABLED
	#define Write_I(string)             DebugUartWrite(__FILE__, OutputLevel_Info, false, (string))
	#define WriteLine_I(string)         DebugUartWrite(__FILE__, OutputLevel_Info, true, (string))
	#define Print_I(formatStr, ...)     DebugUartPrint(__FILE__, OutputLevel_Info, false, (formatStr), ##__VA_ARGS__)
	#define PrintLine_I(formatStr, ...) DebugUartPrint(__FILE__, OutputLevel_Info, true, (formatStr), ##__VA_ARGS__)
#else
	#define Write_I(string)             //Nothing
	#define WriteLine_I(string)         //Nothing
	#define Print_I(formatStr, ...)     //Nothing
	#define PrintLine_I(formatStr, ...) //Nothing
#endif

#if ERROR_LEVEL_OUTPUT_ENABLED
	#define Write_E(string)             DebugUartWrite(__FILE__, OutputLevel_Error, false, (string))
	#define WriteLine_E(string)         DebugUartWrite(__FILE__, OutputLevel_Error, true, (string))
	#define Print_E(formatStr, ...)     DebugUartPrint(__FILE__, OutputLevel_Error, false, (formatStr), ##__VA_ARGS__)
	#define PrintLine_E(formatStr, ...) DebugUartPrint(__FILE__, OutputLevel_Error, true, (formatStr), ##__VA_ARGS__)
#else
	#define Write_E(string)             //Nothing
	#define WriteLine_E(string)         //Nothing
	#define Print_E(formatStr, ...)     //Nothing
	#define PrintLine_E(formatStr, ...) //Nothing
#endif

#if NOTIFY_LEVEL_OUTPUT_ENABLED
	#define Write_N(string)             DebugUartWrite(__FILE__, OutputLevel_Notify, false, (string))
	#define WriteLine_N(string)         DebugUartWrite(__FILE__, OutputLevel_Notify, true, (string))
	#define Print_N(formatStr, ...)     DebugUartPrint(__FILE__, OutputLevel_Notify, false, (formatStr), ##__VA_ARGS__)
	#define PrintLine_N(formatStr, ...) DebugUartPrint(__FILE__, OutputLevel_Notify, true, (formatStr), ##__VA_ARGS__)
#else
	#define Write_N(string)             //Nothing
	#define WriteLine_N(string)         //Nothing
	#define Print_N(formatStr, ...)     //Nothing
	#define PrintLine_N(formatStr, ...) //Nothing
#endif

#if WARNING_LEVEL_OUTPUT_ENABLED
	#define Write_W(string)             DebugUartWrite(__FILE__, OutputLevel_Warning, false, (string))
	#define WriteLine_W(string)         DebugUartWrite(__FILE__, OutputLevel_Warning, true, (string))
	#define Print_W(formatStr, ...)     DebugUartPrint(__FILE__, OutputLevel_Warning, false, (formatStr), ##__VA_ARGS__)
	#define PrintLine_W(formatStr, ...) DebugUartPrint(__FILE__, OutputLevel_Warning, true, (formatStr), ##__VA_ARGS__)
#else
	#define Write_W(string)             //Nothing
	#define WriteLine_W(string)         //Nothing
	#define Print_W(formatStr, ...)     //Nothing
	#define PrintLine_W(formatStr, ...) //Nothing
#endif

#endif //  _DEBUG_H
