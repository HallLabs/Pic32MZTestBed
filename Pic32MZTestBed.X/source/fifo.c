/*
File:   fifo.c
Author: Taylor Robbins
Date:   08\29\2019
Description: 
	** Holds functions and macros that allow easy manipulation of looping First-In First-Out (FIFO) arrays
*/

#include "app.h"
#include "fifo.h"

// +--------------------------------------------------------------+
// |                       Public Functions                       |
// +--------------------------------------------------------------+
u32 FifoLength_(u32 head, u32 tail, u32 bufferLength)
{
	if (head >= tail)
	{
		return head - tail;
	}
	else
	{
		return (bufferLength - tail) + head;
	}
}

u32 FifoSpace_(u32 head, u32 tail, u32 bufferLength)
{
	if (head >= tail)
	{
		return bufferLength - head + tail-1;
	}
	else
	{
		return tail-1 - head;
	}
}

u8 FifoGet_(u32 head, u32 tail, const u8* buffer, u32 bufferLength, u32 offset)
{
	Assert_(offset < FifoLength_(head, tail, bufferLength));
	u32 pos = ((tail + offset) % bufferLength);
	return buffer[pos];
}

u8 FifoPop_(volatile u32* head, volatile u32* tail, u8* buffer, u32 bufferLength)
{
	Assert_(*head != *tail);
	u8 result = buffer[*tail];
	if (*head != *tail) { *tail = ((*tail + 1) % bufferLength); }
	return result;
}

bool FifoPush_(volatile u32* head, volatile u32* tail, u8* buffer, u32 bufferLength, u8 newByte, bool overwrite)
{
	bool result = true;
	u32 newHead = ((*head + 1) % bufferLength);
	if (newHead == *tail)
	{
		if (overwrite) { *tail = ((*tail + 1) % bufferLength); result = false; }
		else { return false; }
	}
	buffer[*head] = newByte;
	*head = newHead;
	return result;
}

bool FifoPushBytes_(volatile u32* head, volatile u32* tail, u8* buffer, u32 bufferLength, const u8* bytesPntr, u32 numBytes, bool overwrite)
{
	bool result = true;
	u32 bIndex; for (bIndex = 0; bIndex < numBytes; bIndex++)
	{
		if (!FifoPush_(head, tail, buffer, bufferLength, bytesPntr[bIndex], overwrite)) { result = false; }
	}
	return result;
}

u8* FifoGetBytePntr_(u32 head, u32 tail, u8* buffer, u32 bufferLength, u32 offset)
{
	Assert_(offset < FifoLength_(head, tail, bufferLength));
	u32 pos = ((tail + offset) % bufferLength);
	return &buffer[pos];
}
