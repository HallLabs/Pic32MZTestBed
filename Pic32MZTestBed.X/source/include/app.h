/*
File:   app.h
Author: Taylor Robbins
Date:   08\29\2019
Description:
	** app.h is a special header file that serves as the first include for all .c files in the project.
	** Any definitions or types that need to be defined in header files are defined here.
*/

#ifndef _APP_H
#define _APP_H

// +--------------------------------------------------------------+
// |                       Common Includes                        |
// +--------------------------------------------------------------+
#include <xc.h>
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <sys/attribs.h> // Defines the __ISR macro.
#include <sys/kmem.h>    // Macros for converting between physical and virtual addresses.

// +--------------------------------------------------------------+
// |                      Common Definitions                      |
// +--------------------------------------------------------------+
#include "defines.h"

// +--------------------------------------------------------------+
// |                     Compile Time Options                     |
// +--------------------------------------------------------------+
#define PRODUCTION_MODE false

#define DEBUG_OUTPUT_ENABLED         true
#define DEBUG_LEVEL_OUTPUT_ENABLED   (true && DEBUG_OUTPUT_ENABLED)
#define INFO_LEVEL_OUTPUT_ENABLED    (true && DEBUG_OUTPUT_ENABLED)
#define ERROR_LEVEL_OUTPUT_ENABLED   (true && DEBUG_OUTPUT_ENABLED)
#define NOTIFY_LEVEL_OUTPUT_ENABLED  (true && DEBUG_OUTPUT_ENABLED)
#define WARNING_LEVEL_OUTPUT_ENABLED (true && DEBUG_OUTPUT_ENABLED)

#define DEBUG_WINDOWS_LINE_ENDINGS  false
#define DEBUG_OUTPUT_LEVEL_PREFIX   true
#define DEBUG_ECHO_INPUT_CHARACTERS true
#define DEBUG_OUTPUT_FILE_NAMES     false

#define DEBUG_OUTPUT_FIFO_LENGTH     2048 //chars
#define DEBUG_INPUT_FIFO_LENGTH      128 //chars
#define DEBUG_INPUT_MAX_LENGTH       64 //chars
#define DEBUG_PRINT_BUFFER_SIZE      512 //chars
#define DEBUG_OVERFLOW_BACKOFF       1000 //ms

// +--------------------------------------------------------------+
// |                   Public Structures/Types                    |
// +--------------------------------------------------------------+
#include "app_structs.h"

// +--------------------------------------------------------------+
// |                        Public Globals                        |
// +--------------------------------------------------------------+
extern Version_t Version;

// +--------------------------------------------------------------+
// |                       Public Functions                       |
// +--------------------------------------------------------------+
void AppInitialize();
void AppUpdate();

#endif //  _APP_H
