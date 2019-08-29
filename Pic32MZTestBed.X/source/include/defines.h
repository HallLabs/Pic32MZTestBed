/*
File:   defines.h
Author: Taylor Robbins
Date:   08\29\2019
*/

#ifndef _DEFINES_H
#define _DEFINES_H

#ifdef __DEBUG
#define DEBUG true
#else
#define DEBUG false
#endif

#define FOREVER                1

#define ENABLED                1
#define DISABLED               0

#define INPUT                  1
#define OUTPUT                 0

#define ANALOG                 1
#define DIGITAL                0

#define HIGH                   1
#define LOW                    0

#define RELEASED               1
#define PRESSED                0

#define FILLED                 1
#define CLEARED                0

#define LED_OFF                1
#define LED_ON                 0

#define kiloHertz              1000
#define kHz                    1000
#define MegaHertz              (1000*1000)
#define MHz                    (1000*1000)

#define DUMMY                  0xDD

#define NUM_MS_PER_SECOND 1000
#define NUM_MS_PER_MINUTE (60*NUM_MS_PER_SECOND)
#define NUM_MS_PER_HOUR   (60*NUM_MS_PER_MINUTE)
#define NUM_MS_PER_DAY    (24*NUM_MS_PER_HOUR)
#define NUM_MS_PER_YEAR   (365*NUM_MS_PER_DAY)

#define NUM_SEC_PER_MINUTE 60
#define NUM_SEC_PER_HOUR   (60*NUM_SEC_PER_MINUTE)
#define NUM_SEC_PER_DAY    (24*NUM_SEC_PER_HOUR)
#define NUM_SEC_PER_YEAR   (365*NUM_SEC_PER_DAY)

#define POW16_0          1 //0x00000001
#define POW16_1         16 //0x00000010
#define POW16_2        256 //0x00000100
#define POW16_3       4096 //0x00001000
#define POW16_4      65536 //0x00010000
#define POW16_5    1048576 //0x00100000
#define POW16_6   16777216 //0x01000000
#define POW16_7  268435456 //0x10000000
#define POW16_8 4294967296

#define POW2_0           1 //0x00000001
#define POW2_1           2 //0x00000002
#define POW2_2           4 //0x00000004
#define POW2_3           8 //0x00000008
#define POW2_4          16 //0x00000010
#define POW2_5          32 //0x00000020
#define POW2_6          64 //0x00000040
#define POW2_7         128 //0x00000080
#define POW2_8         256 //0x00000100
#define POW2_9         512 //0x00000200
#define POW2_10       1024 //0x00000400
#define POW2_11       2048 //0x00000800
#define POW2_12       4096 //0x00001000
#define POW2_13       8192 //0x00002000
#define POW2_14      16384 //0x00004000
#define POW2_15      32768 //0x00008000
#define POW2_16      65536 //0x00010000
#define POW2_17     131072 //0x00020000
#define POW2_18     262144 //0x00040000
#define POW2_19     524288 //0x00080000
#define POW2_20    1048576 //0x00100000
#define POW2_21    2097152 //0x00200000
#define POW2_22    4194304 //0x00400000
#define POW2_23    8388608 //0x00800000
#define POW2_24   16777216 //0x01000000
#define POW2_25   33554432 //0x02000000
#define POW2_26   67108864 //0x04000000
#define POW2_27  134217728 //0x08000000
#define POW2_28  268435456 //0x10000000
#define POW2_29  536870912 //0x20000000
#define POW2_30 1073741824 //0x40000000
#define POW2_31 2147483648 //0x80000000
#define POW2_32 4294967296

typedef uint8_t     byte_t;

typedef uint8_t     u8;
typedef uint16_t    u16;
typedef uint32_t    u32;
typedef uint64_t    u64;

typedef int8_t      i8;
typedef int16_t     i16;
typedef int32_t     i32;
typedef int64_t     i64;

typedef float       real32;
typedef float       r32;
typedef double      real64;
typedef double      r64;

#define nullptr     0

#define ATTR_PACKED __attribute__((packed))

// +------------------------------------------------------------------+
// |                          Public Macros                           |
// +------------------------------------------------------------------+
#define IsFlagSet(BitwiseField, Bit) (((BitwiseField) & (Bit)) != 0)
#define FlagSet(BitwiseField, Bit) (BitwiseField) |= (Bit)
#define FlagUnset(BitwiseField, Bit) (BitwiseField) &= ~(Bit)

#define LsbMask(numBits) ((1 << ((numBits)+1)) - 1)

#define Decrement(unsignedValue) do { if ((unsignedValue) > 0) { (unsignedValue)--; } } while(0)
#define DecrementAmount(unsignedValue, amount) do { if ((unsignedValue) > (amount)) { (unsignedValue) -= (amount); } else { (unsignedValue) = 0; } } while(0)
#define Increment(unsignedValue, maximumValue) do { if ((unsignedValue) < maximumValue) { (unsignedValue)++; } } while(0)
#define IncrementU8(unsignedValue) do { if ((unsignedValue) < 0xFF) { (unsignedValue)++; } } while(0)
#define IncrementU32(unsignedValue) do { if ((unsignedValue) < 0xFFFFFFFF) { (unsignedValue)++; } } while(0)

#define ReverseByteArray(array, size) do                 \
{                                                        \
	u8 _tempValue; u8 _bIndex;                           \
	for (_bIndex = 0; _bIndex < (size)/2; _bIndex++)     \
	{                                                    \
		_tempValue = (array)[(size)-1 - _bIndex];        \
		(array)[(size)-1 - _bIndex] = (array)[_bIndex];  \
		(array)[_bIndex] = _tempValue;                   \
	}                                                    \
} while(0)

#define ArrayCount(Array) (sizeof(Array) / sizeof((Array)[0]))

#define ClearArray(Array)      memset((Array), '\0', sizeof((Array)))
#define ClearStruct(Structure) memset(&(Structure), '\0', sizeof((Structure)))
#define ClearPointer(Pointer)  memset((Pointer), '\0', sizeof(*(Pointer)));

// +==============================+
// |         Assert Macro         |
// +==============================+
#if DEBUG
#define Assert_(Expression) if (!(Expression)) { __asm__ volatile (" sdbbp 0"); }
#else
#define Assert_(Expression) //Nothing
#endif

#if DEBUG
#define Assert(Expression) if (!(Expression))                 \
{                                                             \
	WriteLine_E(__FILE__ " Assert(" #Expression ") failed!"); \
	/*AppError(AppError_Assertion);*/                         \
	__asm__ volatile (" sdbbp 0");                            \
}
#elif 1
#define Assert(Expression) if (!(Expression)) { WriteLine_E(__FILE__ " Assert(" #Expression ") failed!"); }
#else
#define Assert(Expression) //Nothing
#endif

#endif //  _DEFINES_H
