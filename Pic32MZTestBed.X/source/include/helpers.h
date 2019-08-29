/*
File:   helpers.h
Author: Taylor Robbins
Date:   08\29\2019
*/

#ifndef _HELPERS_H
#define _HELPERS_H

// +--------------------------------------------------------------+
// |                       Public Functions                       |
// +--------------------------------------------------------------+
void PrintFormattedMilliseconds(OutputLevel_t outputLevel, u32 numMs);
void PrintFormattedSeconds(OutputLevel_t outputLevel, u32 numSecs);
char GetHexChar(u8 nibbleValue);
void HexToString(const u8* dataPntr, u32 dataLength, char* outBuffer);
bool IsHexChar(char c);
u8 HexCharValue(char c);
bool TryParseHex8(const char* str, u32 numChars, u8* outValue);
bool TryParseHex32(const char* str, u32 numChars, u32* outValue);
bool TryParseInt32(const char* str, u32 numCharacters, i32* outValue);
bool TryParseHexBytes(const char* str, u32 numCharacters, u8* bufferOut, u32 bufferLength, u32* numBytesOut);
u32 SplitString(const char* str, u32 strLength, char splitChar, const char** partsBuffer, u32* lengthsBuffer, u32 maxParts);
u32 SplitNtString(const char* nullTermString, char splitChar, const char** partsBuffer, u32* lengthsBuffer, u32 maxParts);
const char* GetFileNamePart(const char* filePath);

#endif //  _HELPERS_H
