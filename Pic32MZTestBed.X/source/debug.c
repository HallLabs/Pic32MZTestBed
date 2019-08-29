/*
File:   debug.c
Author: Taylor Robbins
Date:   08\29\2019
Description: 
	** Handles doing debug output over UART5 as well as reading and buffering up debug input to be interpretted by the application
	** Both input and output are done using interrupts which allow it to do it's work somewhat asynchronously

	** The debug output is queued up in a FIFO which allows for the application to generate characters and continue on with other tasks
	** without having to wait for all of that data to complete sending over the UART. This also means that the output FIFO can overflow
	** and cause loss of information if too much data is sent too quickly. Use the DebugUartFlush function to prevent this from happening

	** Debug output can be sent with various "Output Levels". If DEBUG_OUTPUT_LEVEL_PREFIX is true then we send 1 byte prefixes for each
	** line that can be interpreted by the receiving program to do various things like change the text color of that line. These output
	** levels can also be enabled and disabled using the ####_LEVEL_OUTPUT_ENABLED defines.
	
	** Debug input is immediately echoed back to the computer when received so that user can see what they are typing. The \b (0x08) character
	** is interpretted as a back space and removes a character from the current input line. Once we receive a \n (0x0A) we consider the input
	** done and let the application know that an command is ready to be processed.
	
	** The new-line format can be controlled with DEBUG_WINDOWS_LINE_ENDINGS. If this is true then we will send a \r\n for every \n in the debug output
	** If it is false then we just send \n characters by themselves.
*/

#include "app.h"
#include "debug.h"

#include "micro.h"
#include "fifo.h"
#include "tick_timer.h"

// +--------------------------------------------------------------+
// |                     Private Definitions                      |
// +--------------------------------------------------------------+
#define DEBUG_BAUD_RATE     115200
#define DEBUG_BRG_CONFIG     ((MICRO_PERF_BUS2_FREQ / (DEBUG_BAUD_RATE*16)) - 1)

//Calculation for the baudrate error
// 200,000,000 / (1,843,200) = 108.50694 (truncates to 108)
// BaudRate = 200,000,000 / (16*108) = 115740.74 (Off by 540bps or 0.47%, bit width 8.64us instead of 8.6805us)

//NOTE: These are all the registers we need to read/write in this file. They have been
//      grouped here to make it easier to change which UART the debug input/output goes through.
#define DBG_UART_NAME       UART5
#define DBG_UART_PREFIX     U5
#define DBG_UART_BRG        U5BRG
#define DBG_UART_MODE       U5MODE
#define DBG_UART_MODEbits   U5MODEbits
#define DBG_UART_STA        U5STA
#define DBG_UART_STAbits    U5STAbits
#define DBG_UART_IPCRXIP    IPC45bits.U5RXIP
#define DBG_UART_IPCRXIS    IPC45bits.U5RXIS
#define DBG_UART_IPCTXIP    IPC45bits.U5TXIP
#define DBG_UART_IPCTXIS    IPC45bits.U5TXIS
#define DBG_UART_IPCERRIP   IPC44bits.U5EIP
#define DBG_UART_IPCERRIS   IPC44bits.U5EIS
#define DBG_UART_RXINTFLAG  IFS5bits.U5RXIF
#define DBG_UART_TXINTFLAG  IFS5bits.U5TXIF
#define DBG_UART_ERRINTFLAG IFS5bits.U5EIF
#define DBG_UART_RXINTEN    IEC5bits.U5RXIE
#define DBG_UART_TXINTEN    IEC5bits.U5TXIE
#define DBG_UART_ERRINTEN   IEC5bits.U5EIE
#define DBG_UART_RXREG      U5RXREG
#define DBG_UART_TXREG      U5TXREG
#define DBG_UART_RXVECTOR   _UART5_RX_VECTOR
#define DBG_UART_TXVECTOR   _UART5_TX_VECTOR
#define DBG_UART_ERRVECTOR  _UART5_FAULT_VECTOR
#define DBG_UART_BITPOS(REGNAME, BITNAME) (_U5##REGNAME##_##BITNAME##_POSITION)
#define DBG_UART_BITSET(REGNAME, BITNAME, value) ((value) << _U5##REGNAME##_##BITNAME##_POSITION)

// +--------------------------------------------------------------+
// |                       Private Globals                        |
// +--------------------------------------------------------------+
static struct
{
	volatile u32 head;
	volatile u32 tail;
	u8 buffer[DEBUG_INPUT_FIFO_LENGTH];
} DebugFifoRx;

static struct
{
	volatile u32 head;
	volatile u32 tail;
	u8 buffer[DEBUG_OUTPUT_FIFO_LENGTH];
} DebugFifoTx;

static bool justWroteNewLine = true;
static u8 readLineBuffer[DEBUG_INPUT_MAX_LENGTH+1];
static bool debugOverflow = false;

// +--------------------------------------------------------------+
// |                       Public Functions                       |
// +--------------------------------------------------------------+
void DebugUartInit()
{
	ClearStruct(DebugFifoRx);
	ClearStruct(DebugFifoTx);
	
	// +==============================+
	// |     UART5 Initialization     |
	// +==============================+
	{
		//NOTE: The UART5 peripheral has already been mapped to the desired pins in MicroInit
		
		DBG_UART_BRG = DEBUG_BRG_CONFIG; // Configure the baud rate generator.
		
		DBG_UART_MODE = (
			DBG_UART_BITSET(MODE, STSEL,  0)    | //Stop Bit Selection = 1 Bit (0)
			DBG_UART_BITSET(MODE, PDSEL,  0b00) | //Parity and Data Selection = 8-bit, no parity (0b00)
			DBG_UART_BITSET(MODE, BRGH,   0)    | //High Baud Rate = DISABLED (0)
			DBG_UART_BITSET(MODE, RXINV,  0)    | //Rx Polarity Inversion = Idle HIGH (0)
			DBG_UART_BITSET(MODE, ABAUD,  0)    | //Auto-Baud = DISABLED (0)
			DBG_UART_BITSET(MODE, LPBACK, 0)    | //Loopback Mode = DISABLED (0)
			DBG_UART_BITSET(MODE, WAKE,   0)    | //Wake-up on Start Bit Detect = DISABLED (0)
			DBG_UART_BITSET(MODE, UEN,    0b00) | //UART Enable Mode = U5RX and U5TX used, U5CTS and U5RTS not used (0b00)
			DBG_UART_BITSET(MODE, RTSMD,  0)    | //RTS Pin Mode = Flow Control Mode (0)
			DBG_UART_BITSET(MODE, IREN,   0)    | //IrDA Encoder/Decoder = DISABLED (0)
			DBG_UART_BITSET(MODE, SIDL,   0)    | //Stop in Idle Mode = Continue in Idle (0)
			DBG_UART_BITSET(MODE, ON,     1)      //UART Enabled = ENABLED (1)
		);
		
		DBG_UART_STA = (
			DBG_UART_BITSET(STA, URXDA,    0)    | //
			DBG_UART_BITSET(STA, OERR,     0)    | //Rx Overrun Flag = CLEARED (0)
			DBG_UART_BITSET(STA, FERR,     0)    | //
			DBG_UART_BITSET(STA, PERR,     0)    | //
			DBG_UART_BITSET(STA, RIDLE,    0)    | //
			DBG_UART_BITSET(STA, ADDEN,    0)    | //Address Character Detect = DISABLED (0)
			DBG_UART_BITSET(STA, URXISEL,  0b00) | //Rx Interrupt Mode = While Rx Buffer is NOT Empty (0b00)
			DBG_UART_BITSET(STA, TRMT,     0)    | //
			DBG_UART_BITSET(STA, UTXBF,    0)    | //
			DBG_UART_BITSET(STA, UTXEN,    1)    | //UART Transmit Enabled = ENABLED (1)
			DBG_UART_BITSET(STA, UTXBRK,   0)    | //Send Break Bit = CLEARED (0)
			DBG_UART_BITSET(STA, URXEN,    1)    | //UART Receive Enabled = ENABLED (1)
			DBG_UART_BITSET(STA, UTXINV,   0)    | //Tx Polarity Inversion = Idle HIGH (0)
			DBG_UART_BITSET(STA, UTXISEL,  0b10) | //Tx Interrupt Mode = While Tx Buffer is Empty (0b10)
			DBG_UART_BITSET(STA, ADDR,     0x00) | //Target Address = 0x00
			DBG_UART_BITSET(STA, ADM_EN,   0)    | //
			DBG_UART_BITSET(STA, URXISEL0, 0)    | //
			DBG_UART_BITSET(STA, URXISEL1, 0)    | //
			DBG_UART_BITSET(STA, UTXISEL0, 0)    | //
			DBG_UART_BITSET(STA, UTXISEL1, 0)    | //
			DBG_UART_BITSET(STA, UTXSEL,   0)      //
		);
		
		DBG_UART_IPCRXIP  = 1; DBG_UART_IPCRXIS  = 0; //Rx Priority 1.0
		DBG_UART_IPCTXIP  = 1; DBG_UART_IPCTXIS  = 0; //Tx Priority 1.0
		DBG_UART_IPCERRIP = 1; DBG_UART_IPCERRIS = 0; //Err Priority 1.0
		
		// Clear interrupt flags.
		DBG_UART_RXINTFLAG  = CLEARED;
		DBG_UART_TXINTFLAG  = CLEARED;
		DBG_UART_ERRINTFLAG = CLEARED;
		
		// Enable interrupts for Rx and errors.
		// Tx interrupt will be enabled when data is available to send.
		DBG_UART_RXINTEN  = ENABLED;
		DBG_UART_TXINTEN  = DISABLED;
		DBG_UART_ERRINTEN = ENABLED;
	}
}

bool DebugUartTxPut(u8 newByte)
{
	MicroDisableInterrupts();
	bool result = FifoPush(DebugFifoTx, newByte);
	DBG_UART_TXINTEN = ENABLED;
	MicroEnableInterrupts();
	
	return result;
}

bool DebugUartTxPutBytes(const u8* dataPntr, u32 dataLength)
{
	bool result = true;
	
	MicroDisableInterrupts();
	u32 bIndex;
	for (bIndex = 0; bIndex < dataLength; bIndex++)
	{
		if (!FifoPush(DebugFifoTx, dataPntr[bIndex])) { result = false; }
	}
	DBG_UART_TXINTEN = ENABLED;
	MicroEnableInterrupts();
	
	return result;
}

void DebugPutByte(u8 newByte)
{
	if (!debugOverflow && DebugOutputBackoff == 0)
	{
		bool success = DebugUartTxPut(newByte);
		if (!success)
		{
			debugOverflow = true;
		}
	}
}

void DebugUartWrite(const char* rawFileName, OutputLevel_t outputLevel, bool newLine, const char* string)
{
	u32 cIndex;
	for (cIndex = 0; string[cIndex] != '\0'; cIndex++)
	{
		if (string[cIndex] == '\n')
		{
			#if DEBUG_WINDOWS_LINE_ENDINGS
			DebugPutByte('\r');
			DebugPutByte('\n');
			#else
			DebugPutByte('\n');
			#endif
			justWroteNewLine = true;
		}
		else
		{
			if (justWroteNewLine)
			{
				#if DEBUG_OUTPUT_LEVEL_PREFIX
				if (outputLevel != OutputLevel_None)
				{
					DebugPutByte((u8)outputLevel);
				}
				#endif
				
				#if DEBUG_OUTPUT_FILE_NAMES
				if (rawFileName != nullptr)
				{
					const char* fileNamePntr = GetFileNamePart(rawFileName);
					u32 fileNameLength = (u32)strlen(fileNamePntr);
					if (fileNameLength > 0)
					{
						u32 cIndex2; for (cIndex2 = 0; cIndex2 < fileNameLength; cIndex2++)
						{
							DebugPutByte(fileNamePntr[cIndex2]);
						}
						DebugPutByte(':'); DebugPutByte(' ');
					}
				}
				#endif
			}
			
			DebugPutByte(string[cIndex]);
			justWroteNewLine = false;
		}
	}
	
	if (newLine)
	{
		#if DEBUG_WINDOWS_LINE_ENDINGS
		DebugPutByte('\r');
		DebugPutByte('\n');
		#else
		DebugPutByte('\n');
		#endif
		justWroteNewLine = true;
	}
}

void DebugUartPrint(const char* rawFileName, OutputLevel_t outputLevel, bool newLine, const char* formatStr, ...)
{
	char printBuffer[DEBUG_PRINT_BUFFER_SIZE];
	va_list args;
	
	va_start(args, formatStr);
	size_t length = vsnprintf(printBuffer, DEBUG_PRINT_BUFFER_SIZE, formatStr, args);
	va_end(args);
	
	if (length >= DEBUG_PRINT_BUFFER_SIZE)
	{
		DebugUartWrite(rawFileName, outputLevel, newLine, "[DEBUG PRINT BUFFER OVERFLOW]");
	}
	else if (length > 0)
	{
		printBuffer[length] = '\0';
		DebugUartWrite(rawFileName, outputLevel, newLine, printBuffer);
	}
}

void DebugUartFlush()
{
	if (FifoLength(DebugFifoTx) > 0 && DBG_UART_STAbits.TRMT == false)
	{
		while (FifoLength(DebugFifoTx) > 0 && DBG_UART_STAbits.TRMT == false) { MicroClrWDT(); }
		MicroDelay(1);
	}
}

char* DebugUartReadLine()
{
	u32 lineLength = 0;
	readLineBuffer[0] = '\0';
	
	while (lineLength < FifoLength(DebugFifoRx))
	{
		char nextByte = FifoGet(DebugFifoRx, lineLength);
		if (nextByte == '\n')
		{
			while (lineLength > 0)
			{
				FifoPop(DebugFifoRx);
				lineLength--;
			}
			FifoPop(DebugFifoRx);
			return &readLineBuffer[0];
		}
		else
		{
			if (lineLength < DEBUG_INPUT_MAX_LENGTH)
			{
				readLineBuffer[lineLength] = nextByte;
				readLineBuffer[lineLength+1] = '\0';
			}
			lineLength++;
		}
	}
	
	return nullptr;
}

u32 DebugUartRxLength()
{
	return FifoLength(DebugFifoRx);
}

char DebugUartRxGet(u32 offset)
{
	return (char)FifoGet(DebugFifoRx, offset);
}

void DebugUartUpdate()
{
	if (debugOverflow)
	{
		if (FifoLength(DebugFifoTx) == 0)
		{
			DebugUartTxPut('\n');
			DebugUartTxPut('=');
			DebugUartTxPut('=');
			DebugUartTxPut('=');
			DebugUartTxPut('=');
			DebugUartTxPut('\n');
			DebugUartTxPut('\n');
			DebugOutputBackoff = DEBUG_OVERFLOW_BACKOFF;
			debugOverflow = false;
		}
	}
}

// +--------------------------------------------------------------+
// |                    Debug UART Receive ISR                    |
// +--------------------------------------------------------------+
void __ISR(DBG_UART_RXVECTOR, ipl1AUTO) DebugUartRxIsr()
{
	u8 newByte;
	
	//While Rx Data Available
	while (DBG_UART_STAbits.URXDA)
	{
		bool parityError = (DBG_UART_STAbits.PERR != 0);
		bool framingError = (DBG_UART_STAbits.FERR != 0);
		newByte = DBG_UART_RXREG;
		
		if (!parityError && !framingError)
		{
			if ((newByte >= ' ' && newByte <= '~') || newByte == '\n' || newByte == '\t')
			{
				FifoPushHard(DebugFifoRx, newByte); //Push it on the FIFO to be processed later
				#if DEBUG_ECHO_INPUT_CHARACTERS
				FifoPush(DebugFifoTx, newByte);
				DBG_UART_TXINTEN = ENABLED;
				#endif
			}
			else if (newByte != '\b' && newByte != '\r')
			{
				#if DEBUG_ECHO_INPUT_CHARACTERS
				FifoPush(DebugFifoTx, '?');
				DBG_UART_TXINTEN = ENABLED;
				#endif
			}
		}
		else
		{
			#if DEBUG_ECHO_INPUT_CHARACTERS
			FifoPush(DebugFifoTx, '!');
			DBG_UART_TXINTEN = ENABLED;
			#endif
		}
	}
	
	DBG_UART_RXINTFLAG = CLEARED;
}

// +--------------------------------------------------------------+
// |                   Debug UART Transmit ISR                    |
// +--------------------------------------------------------------+
void __ISR(DBG_UART_TXVECTOR, ipl1AUTO) DebugUartTxIsr()
{
	//While data to send and Hardware FIFO is not full
	while (!DBG_UART_STAbits.UTXBF && FifoLength(DebugFifoTx) > 0)
	{
		u8 nextByte = FifoPop(DebugFifoTx);
		DBG_UART_TXREG = nextByte;
	}
	
	if (FifoLength(DebugFifoTx) == 0) { DBG_UART_TXINTEN = DISABLED; }
	DBG_UART_TXINTFLAG = CLEARED;
}

// +--------------------------------------------------------------+
// |                     Debug UART Error ISR                     |
// +--------------------------------------------------------------+
void __ISR(DBG_UART_ERRVECTOR, ipl1AUTO) DebugUartErrIsr()
{
	DBG_UART_STAbits.PERR = CLEARED;
	DBG_UART_STAbits.FERR = CLEARED;
	DBG_UART_STAbits.OERR = CLEARED;
	DBG_UART_ERRINTFLAG = CLEARED;
}
