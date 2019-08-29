/*
File:   helpers.c
Author: Taylor Robbins
Date:   08\29\2019
Description: 
	** Holds a bunch of functions that help us do stuff throughout the application.
	** Basically it's a bunch of functions with no home anywhere else
*/

#include "app.h"
#include "helpers.h"

#include "micro.h"
#include "debug.h"
#include "tick_timer.h"

// +--------------------------------------------------------------+
// |                       Public Functions                       |
// +--------------------------------------------------------------+
void PrintFormattedMilliseconds(OutputLevel_t outputLevel, u32 numMs)
{
	u32 ms = numMs;
	u32 days    = (ms / NUM_MS_PER_DAY);    ms -= (days * NUM_MS_PER_DAY);
	u32 hours   = (ms / NUM_MS_PER_HOUR);   ms -= (hours * NUM_MS_PER_HOUR);
	u32 minutes = (ms / NUM_MS_PER_MINUTE); ms -= (minutes * NUM_MS_PER_MINUTE);
	u32 seconds = (ms / NUM_MS_PER_SECOND); ms -= (seconds * NUM_MS_PER_SECOND);
	
	if (days == 0)
	{
		if (hours == 0)
		{
			if (minutes == 0)
			{
				if (seconds == 0)
				{
					PrintAt(outputLevel, "%ums", ms);
				}
				else
				{
					PrintAt(outputLevel, "%us %ums", seconds, ms);
				}
			}
			else
			{
				PrintAt(outputLevel, "%um %us %ums", minutes, seconds, ms);
			}
		}
		else
		{
			PrintAt(outputLevel, "%uh %um %us %ums", hours, minutes, seconds, ms);
		}
	}
	else
	{
		PrintAt(outputLevel, "%ud %uh %um %us %ums", days, hours, minutes, seconds, ms);
	}
}

void PrintFormattedSeconds(OutputLevel_t outputLevel, u32 numSecs)
{
	u32 secs = numSecs;
	u32 days    = (secs / NUM_SEC_PER_DAY);    secs -= (days * NUM_SEC_PER_DAY);
	u32 hours   = (secs / NUM_SEC_PER_HOUR);   secs -= (hours * NUM_SEC_PER_HOUR);
	u32 minutes = (secs / NUM_SEC_PER_MINUTE); secs -= (minutes * NUM_SEC_PER_MINUTE);
	
	if (days == 0)
	{
		if (hours == 0)
		{
			if (minutes == 0)
			{
				PrintAt(outputLevel, "%us", secs);
			}
			else
			{
				PrintAt(outputLevel, "%um %us", minutes, secs);
			}
		}
		else
		{
			PrintAt(outputLevel, "%uh %um %us", hours, minutes, secs);
		}
	}
	else
	{
		PrintAt(outputLevel, "%ud %uh %um %us", days, hours, minutes, secs);
	}
}

char GetHexChar(u8 nibbleValue)
{
	if (nibbleValue < 0x0A)  { return ('0' + nibbleValue); }
	if (nibbleValue <= 0x0F) { return ('A' + (nibbleValue-0x0A)); }
	return '?';
}

//NOTE: This functions doesn't do any buffer overrun check on the output buffer.
// It's assumed the calling code knows the size of the resulting string (dataLength*2 + '\0')
void HexToString(const u8* dataPntr, u32 dataLength, char* outBuffer)
{
	Assert(dataPntr != nullptr);
	Assert(outBuffer != nullptr);
	u32 bIndex;
	for (bIndex = 0; bIndex < dataLength; bIndex++)
	{
		u8 dataByte = dataPntr[bIndex];
		outBuffer[(bIndex*2) + 0] = GetHexChar((dataByte>>4)&0x0F);
		outBuffer[(bIndex*2) + 1] = GetHexChar((dataByte>>0)&0x0F);
	}
	outBuffer[dataLength*2] = '\0';
}

bool IsHexChar(char c)
{
	if ((c >= '0' && c <= '9') ||
		(c >= 'A' && c <= 'F') ||
		(c >= 'a' && c <= 'f'))
	{
		return true;
	}
	else
	{
		return false;
	}
}

u8 HexCharValue(char c)
{
	if (c >= '0' && c <= '9') { return (u8)(c - '0'); }
	if (c >= 'A' && c <= 'F') { return (u8)(c - 'A') + 10; }
	if (c >= 'a' && c <= 'f') { return (u8)(c - 'a') + 10; }
	return 0;
}

bool TryParseHex8(const char* str, u32 numChars, u8* outValue)
{
	if (str == nullptr) { return false; }
	if (numChars == 0) { return false; }
	
	bool foundHexChar = false;
	u32 power = 1;
	u32 result = 0;
	
	u32 cIndex;
	for (cIndex = numChars; cIndex > 0; cIndex--)
	{
		char c = str[cIndex-1];
		
		if (IsHexChar(c))
		{
			if (power > POW16_1)
			{
				//u8 would overflow if we add this number
				return false;
			}
			u8 charValue = HexCharValue(c);
			result += charValue * power;
			foundHexChar = true;
			if (power < POW16_1) { power <<= 4; }
			else { power <<= 3; } //so we don't overflow u32 we only shift up 3 on the last nibble
		}
		else if (c == ' ' || c == '\t' || c == '\n' || c == '\r')
		{
			//Do nothing for whitespace
		}
		else
		{
			//Invalid character found in string
			return false;
		}
	}
	
	if (!foundHexChar)
	{
		//No numbers were found in the string
		return false;
	}
	
	if (outValue != nullptr) { *outValue = result; }
	return true;
}

bool TryParseHex32(const char* str, u32 numChars, u32* outValue)
{
	if (str == nullptr) { return false; }
	if (numChars == 0) { return false; }
	
	bool foundHexChar = false;
	u32 power = 1;
	u32 result = 0;
	
	u32 cIndex;
	for (cIndex = numChars; cIndex > 0; cIndex--)
	{
		char c = str[cIndex-1];
		
		if (IsHexChar(c))
		{
			if (power > POW16_7)
			{
				//u32 would overflow if we add this number
				return false;
			}
			u8 charValue = HexCharValue(c);
			result += charValue * power;
			foundHexChar = true;
			if (power < POW16_7) { power <<= 4; }
			else { power <<= 3; } //so we don't overflow u32 we only shift up 3 on the last nibble
		}
		else if (c == ' ' || c == '\t' || c == '\n' || c == '\r')
		{
			//Do nothing for whitespace
		}
		else
		{
			//Invalid character found in string
			return false;
		}
	}
	
	if (!foundHexChar)
	{
		//No numbers were found in the string
		return false;
	}
	
	if (outValue != nullptr) { *outValue = result; }
	return true;
}

bool TryParseInt32(const char* str, u32 numCharacters, i32* outValue)
{
	if (numCharacters == 0) 
	{
		Assert(false);
	}
	
	u32 aggregateValue = 0;
	bool isNegative = false;
	bool foundFirstNumeric = false;
	bool endLoop = false;

	u32 strLength = (numCharacters == 0 ? strlen(str) : numCharacters);

	u32 i;
	for (i = 0; i < strLength && !endLoop; i++)
	{
		switch (str[i])
		{
		case '\0':
		{
			endLoop = true;

		} break;

		case ' ':
		{
			if (foundFirstNumeric)
			{
				//space after numerical values indicates the end of the number
				endLoop = true;
			}
			else
			{
				//ignore the leading whitespace
			}

		} break;

		case '-':
		{
			if (foundFirstNumeric)
			{
				//a '-' sign after numerics is invalid
				return false;
			}
			else
			{
				isNegative = true;
			}

		} break;

		case '+':
		{
			if (foundFirstNumeric)
			{
				//a '+' sign after numerics is invalid
				return false;
			}
			else if (isNegative)
			{
				//we already found a '-' and now a + so the string is invalid
				return false;
			}
			else
			{
				//no effect since isNegative defaults to false
				isNegative = false;
			}

		} break;

		case '0': // 0011 0000
		case '1': // 0011 0001
		case '2': // 0011 0010
		case '3': // 0011 0011
		case '4': // 0011 0100
		case '5': // 0011 0101
		case '6': // 0011 0110
		case '7': // 0011 0111
		case '8': // 0011 1000
		case '9': // 0011 1001
		{
			foundFirstNumeric = true;
			
			u32 oldAggregate = aggregateValue;

			//if values came before they need to be shifted up one place
			aggregateValue *= 10;

								  //make sure the high nibble is correct
			//Assert((str[i] & 0xF0) == 0b00110000);
			//our number equivelant should lie in the low nibble
			u8 numericValue = (str[i] & 0x0F);

			aggregateValue += numericValue;
			
			//if our new value is less then the one we had before then
			//the value has wrapped.
			if (aggregateValue < oldAggregate)
			{
				return false;
			}

			if (aggregateValue & (1 << 31))
			{
				//the number has reached past the maximum value of a signed int32
				return false;
			}

		} break;

		default:
		{
			//any other characters denote an invalid integer
			return false;

		} break;
		};
	}

	if (!foundFirstNumeric)
	{
		//no numerics where found. That's an invalid integer
		return false;
	}

	if (isNegative)
	{
		*outValue = -(i32)aggregateValue;
	}
	else
	{
		*outValue = (i32)aggregateValue;
	}
	return true;
}

bool TryParseHexBytes(const char* str, u32 numCharacters, u8* bufferOut, u32 bufferLength, u32* numBytesOut)
{
	Assert(str != nullptr);
	Assert(bufferOut != nullptr || bufferLength == 0);
	
	if (numBytesOut != nullptr) { *numBytesOut = 0; }
	u32 numBytes = 0;
	u32 cIndex = 0;
	while (cIndex+1 < numCharacters)
	{
		char c1 = str[cIndex];
		char c2 = str[cIndex+1];
		if (IsHexChar(c1) && IsHexChar(c2))
		{
			u8 newByte;
			TryParseHex8(&str[cIndex], 2, &newByte);
			if (numBytes < bufferLength && bufferOut != nullptr)
			{
				bufferOut[numBytes] = newByte;
			}
			numBytes++;
			if (numBytesOut != nullptr) { *numBytesOut = numBytes; }
			cIndex += 2;
		}
		else if (c1 != ' ' && c1 != '\t')
		{
			return false; //invalid character found
		}
		else if (c2 != ' ' && c2 != '\t')
		{
			return false; //invalid character found
		}
		else
		{
			cIndex++;
		}
	}
	
	if (numBytes > bufferLength)
	{
		return false; //Too many bytes for buffer
	}
	
	return true;
}

u32 SplitString(const char* str, u32 strLength, char splitChar, const char** partsBuffer, u32* lengthsBuffer, u32 maxParts)
{
	u32 cIndex = 0;
	u32 partStart = 0;
	u32 numParts = 0;
	for (cIndex = 0; cIndex <= strLength; cIndex++)
	{
		if (cIndex == strLength || str[cIndex] == splitChar)
		{
			if (partsBuffer != nullptr && numParts < maxParts) { partsBuffer[numParts] = &str[partStart]; }
			if (lengthsBuffer != nullptr && numParts < maxParts) { lengthsBuffer[numParts] = cIndex - partStart; }
			numParts++;
			partStart = cIndex+1;
		}
	}
	return numParts;
}

u32 SplitNtString(const char* nullTermString, char splitChar, const char** partsBuffer, u32* lengthsBuffer, u32 maxParts)
{
	return SplitString(nullTermString, (u32)strlen(nullTermString), splitChar, partsBuffer, lengthsBuffer, maxParts);
}

const char* GetFileNamePart(const char* filePath)
{
	if (filePath == nullptr) { return filePath; }
	const char* result = filePath;
	u32 cIndex; for (cIndex = 0; filePath[cIndex] != '\0'; cIndex++)
	{
		if (filePath[cIndex] == '\\' || filePath[cIndex] == '/') { result = &filePath[cIndex+1]; }
	}
	return result;
}

