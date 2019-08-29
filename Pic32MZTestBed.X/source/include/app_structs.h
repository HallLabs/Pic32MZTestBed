/*
File:   app_structs.h
Author: Taylor Robbins
Date:   08\29\2019
Description:
	** Contains a bunch of the common structures and types that are used throughout the application
	** This file is included by app.h which means that everybody includes this file automatically
*/

#ifndef _APP_STRUCTS_H
#define _APP_STRUCTS_H

typedef struct
{
	u8 major;
	u8 minor;
	u8 build;
} Version_t;

enum
{
	ResetCause_PowerOn       = 0x01,
	ResetCause_BrownOut      = 0x02,
	ResetCause_WakeFromIdle  = 0x04,
	ResetCause_WakeFromSleep = 0x08,
	ResetCause_WatchdogTimer = 0x10,
	ResetCause_DeadmanTimer  = 0x20,
	ResetCause_SoftwareReset = 0x40,
	ResetCause_ExternalReset = 0x80,
};

typedef enum
{
	OutputLevel_None    = 0x00,
	OutputLevel_Debug   = 0x01,
	OutputLevel_Info    = 0x02,
	OutputLevel_Error   = 0x03,
	OutputLevel_Notify  = 0x04,
	OutputLevel_Warning = 0x05,
} OutputLevel_t;

#endif //  _APP_STRUCTS_H
