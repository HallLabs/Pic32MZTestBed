/*
File:   fifo.h
Author: Taylor Robbins
Date:   08\29\2019
*/

#ifndef _FIFO_H
#define _FIFO_H

// +--------------------------------------------------------------+
// |                       Public Functions                       |
// +--------------------------------------------------------------+
u32 FifoLength_(u32 head, u32 tail, u32 bufferLength);
u32 FifoSpace_(u32 head, u32 tail, u32 bufferLength);
u8 FifoGet_(u32 head, u32 tail, const u8* buffer, u32 bufferLength, u32 offset);
u8 FifoPop_(volatile u32* head, volatile u32* tail, u8* buffer, u32 bufferLength);
bool FifoPush_(volatile u32* head, volatile u32* tail, u8* buffer, u32 bufferLength, u8 newByte, bool overwrite);
bool FifoPushBytes_(volatile u32* head, volatile u32* tail, u8* buffer, u32 bufferLength, const u8* bytesPntr, u32 numBytes, bool overwrite);
u8* FifoGetBytePntr_(u32 head, u32 tail, u8* buffer, u32 bufferLength, u32 offset);

// +--------------------------------------------------------------+
// |                        Public Macros                         |
// +--------------------------------------------------------------+
#define FifoLength(FifoName)                             FifoLength_((FifoName).head, (FifoName).tail, sizeof((FifoName).buffer))
#define FifoSpace(FifoName)                              FifoSpace_((FifoName).head, (FifoName).tail, sizeof((FifoName).buffer))
#define FifoGet(FifoName, offset)                        FifoGet_((FifoName).head, (FifoName).tail, &(FifoName).buffer[0], sizeof((FifoName).buffer), (offset))
#define FifoPop(FifoName)                                FifoPop_(&(FifoName).head, &(FifoName).tail, &(FifoName).buffer[0], sizeof((FifoName).buffer))
#define FifoPush(FifoName, newByte)                      FifoPush_(&(FifoName).head, &(FifoName).tail, &(FifoName).buffer[0], sizeof((FifoName).buffer), (newByte), false)
#define FifoPushHard(FifoName, newByte)                  FifoPush_(&(FifoName).head, &(FifoName).tail, &(FifoName).buffer[0], sizeof((FifoName).buffer), (newByte), true)
#define FifoPushBytes(FifoName, bytesPntr, numBytes)     FifoPushBytes_(&(FifoName).head, &(FifoName).tail, &(FifoName).buffer[0], sizeof((FifoName).buffer), bytesPntr, numBytes, false)
#define FifoPushBytesHard(FifoName, bytesPntr, numBytes) FifoPushBytes_(&(FifoName).head, &(FifoName).tail, &(FifoName).buffer[0], sizeof((FifoName).buffer), bytesPntr, numBytes, true)
#define FifoGetBytePntr(FifoName, offset)                FifoGetBytePntr_((FifoName).head, (FifoName).tail, &(FifoName).buffer[0], sizeof((FifoName).buffer), offset)

#endif //  _FIFO_H
