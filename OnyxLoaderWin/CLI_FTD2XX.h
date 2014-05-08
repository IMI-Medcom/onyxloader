#ifndef CLI_FTD2XXX_H_
#define CLI_FTD2XXX_H_

//	const UInt32 FT_LIST_NUMBER_ONLY = 0x80000000;
//	const UInt32 FT_LIST_BY_INDEX	= 0x40000000;
//	const UInt32 FT_LIST_ALL			= 0x20000000;
//	const UInt32 FT_OPEN_BY_SERIAL_NUMBER = 1;
//	const UInt32 FT_OPEN_BY_DESCRIPTION    = 2;

//	const UInt32 FT_EVENT_RXCHAR		= 1;
//	const UInt32 FT_EVENT_MODEM_STATUS  = 2;

#define FT_PREFIX [System::Runtime::InteropServices::DllImport("FTD2XX.dll")]

enum {
    FT_OK,
    FT_INVALID_HANDLE,
    FT_DEVICE_NOT_FOUND,
    FT_DEVICE_NOT_OPENED,
    FT_IO_ERROR,
    FT_INSUFFICIENT_RESOURCES,
    FT_INVALID_PARAMETER,
    FT_INVALID_BAUD_RATE,
    FT_DEVICE_NOT_OPENED_FOR_ERASE,
    FT_DEVICE_NOT_OPENED_FOR_WRITE,
    FT_FAILED_TO_WRITE_DEVICE,
    FT_EEPROM_READ_FAILED,
    FT_EEPROM_WRITE_FAILED,
    FT_EEPROM_ERASE_FAILED,
    FT_EEPROM_NOT_PRESENT,
    FT_EEPROM_NOT_PROGRAMMED,
    FT_INVALID_ARGS,
    FT_NOT_SUPPORTED,
    FT_OTHER_ERROR
};

#define FT_SUCCESS(status) ((status) == FT_OK)

typedef void * FT_HANDLE;
typedef unsigned long DWORD;
typedef unsigned long FT_STATUS;
typedef void * LPVOID;
typedef void * PVOID;
typedef DWORD * LPDWORD;
typedef DWORD ULONG;
typedef unsigned short USHORT;
typedef unsigned char UCHAR;
typedef unsigned short WORD;
typedef WORD * LPWORD;
typedef unsigned char UCHAR;
typedef UCHAR * PUCHAR;
typedef char CHAR;
typedef CHAR * PCHAR;
typedef ULONG FT_DEVICE;
typedef void *HANDLE;
typedef int BOOL;
#define FALSE   0
#define TRUE    1

// as c++. net is a managed application and our ftd2xx.dll is unmanaged code you must declare the functions here explicitly
// to allow you to call them within the application. An include file and the .lib file simply wont work with c++.net. Its
// a similar problem in c#.
// see http://www.codeguru.com/Cpp/COM-Tech/complus/managed/article.php/c3947/ for more on this.
// I have only included these 4 functions in this to show you how to do this. For other functions that you require similar
// declerations will need to go here. 


//using namespace System::Runtime::InteropServices;
FT_PREFIX FT_STATUS FT_Open(int deviceNumber, FT_HANDLE * pHandle);
FT_PREFIX FT_STATUS FT_OpenEx(PVOID pArg1, DWORD Flags, FT_HANDLE *pHandle);
FT_PREFIX FT_STATUS FT_ListDevices(PVOID pArg1, PVOID pArg2, DWORD Flags);
//FT_PREFIX FT_STATUS FT_ListDevices(UInt32 pvArg1, void * pvArg2, UInt32 dwFlags);	// FT_ListDevcies by serial number or description by index only
FT_PREFIX FT_STATUS FT_Close(FT_HANDLE ftHandle);
FT_PREFIX FT_STATUS FT_Read(FT_HANDLE ftHandle, LPVOID lpBuffer, DWORD nBufferSize, LPDWORD lpBytesReturned);
FT_PREFIX FT_STATUS FT_Write(FT_HANDLE ftHandle, LPVOID lpBuffer, DWORD nBufferSize, LPDWORD lpBytesWritten);
FT_PREFIX FT_STATUS FT_SetBaudRate(FT_HANDLE ftHandle, ULONG BaudRate);
FT_PREFIX FT_STATUS FT_SetDivisor(FT_HANDLE ftHandle, USHORT Divisor);
FT_PREFIX FT_STATUS FT_SetDataCharacteristics(FT_HANDLE ftHandle, UCHAR WordLength, UCHAR StopBits, UCHAR Parity);
FT_PREFIX FT_STATUS FT_SetFlowControl(FT_HANDLE ftHandle, USHORT FlowControl, UCHAR XonChar, UCHAR XoffChar);
FT_PREFIX FT_STATUS FT_ResetDevice(FT_HANDLE ftHandle);
FT_PREFIX FT_STATUS FT_SetDtr(FT_HANDLE ftHandle);
FT_PREFIX FT_STATUS FT_ClrDtr(FT_HANDLE ftHandle);
FT_PREFIX FT_STATUS FT_SetRts(FT_HANDLE ftHandle);
FT_PREFIX FT_STATUS FT_ClrRts(FT_HANDLE ftHandle);
FT_PREFIX FT_STATUS FT_GetModemStatus(FT_HANDLE ftHandle, ULONG *pModemStatus);
FT_PREFIX FT_STATUS FT_SetChars(FT_HANDLE ftHandle, UCHAR EventChar, UCHAR EventCharEnabled, UCHAR ErrorChar, UCHAR ErrorCharEnabled);
FT_PREFIX FT_STATUS FT_Purge(FT_HANDLE ftHandle, ULONG Mask);
FT_PREFIX FT_STATUS FT_SetTimeouts(FT_HANDLE ftHandle, ULONG ReadTimeout, ULONG WriteTimeout);
FT_PREFIX FT_STATUS FT_GetQueueStatus(FT_HANDLE ftHandle, DWORD *dwRxBytes);
FT_PREFIX FT_STATUS FT_SetEventNotification(FT_HANDLE ftHandle, DWORD Mask, PVOID Param);
FT_PREFIX FT_STATUS FT_GetStatus(FT_HANDLE ftHandle, DWORD *dwRxBytes, DWORD *dwTxBytes, DWORD *dwEventDWord);
FT_PREFIX FT_STATUS FT_SetBreakOn(FT_HANDLE ftHandle);
FT_PREFIX FT_STATUS FT_SetBreakOff(FT_HANDLE ftHandle);
FT_PREFIX FT_STATUS FT_SetWaitMask(FT_HANDLE ftHandle, DWORD Mask);
FT_PREFIX FT_STATUS FT_WaitOnMask(FT_HANDLE ftHandle, DWORD *Mask);
FT_PREFIX FT_STATUS FT_GetEventStatus(FT_HANDLE ftHandle, DWORD *dwEventDWord);
FT_PREFIX FT_STATUS FT_ReadEE(FT_HANDLE ftHandle, DWORD dwWordOffset, LPWORD lpwValu);
FT_PREFIX FT_STATUS FT_WriteEE(FT_HANDLE ftHandle, DWORD dwWordOffset, WORD wValue);
FT_PREFIX FT_STATUS FT_EraseEE(FT_HANDLE ftHandle);
// Missed out the programming stuff +++ 

FT_PREFIX FT_STATUS  FT_EE_UASize(FT_HANDLE ftHandle, LPDWORD lpdwSize);
FT_PREFIX FT_STATUS  FT_EE_UAWrite(FT_HANDLE ftHandle, PUCHAR pucData, DWORD dwDataLen);
FT_PREFIX FT_STATUS  FT_EE_UARead(FT_HANDLE ftHandle, PUCHAR pucData, DWORD dwDataLen, LPDWORD lpdwBytesRead);
FT_PREFIX FT_STATUS  FT_SetLatencyTimer(FT_HANDLE ftHandle, UCHAR ucLatency);
FT_PREFIX FT_STATUS  FT_GetLatencyTimer(FT_HANDLE ftHandle, PUCHAR pucLatency);
FT_PREFIX FT_STATUS  FT_SetBitMode(FT_HANDLE ftHandle, UCHAR ucMask, UCHAR ucEnable);
FT_PREFIX FT_STATUS  FT_GetBitMode(FT_HANDLE ftHandle, PUCHAR pucMode);
FT_PREFIX FT_STATUS  FT_SetUSBParameters(FT_HANDLE ftHandle, ULONG ulInTransferSize, ULONG ulOutTransferSize);
FT_PREFIX FT_STATUS  FT_GetDeviceInfo(FT_HANDLE ftHandle, FT_DEVICE *lpftDevice, LPDWORD lpdwID, PCHAR SerialNumber, PCHAR Description, LPVOID Dummy);
FT_PREFIX FT_STATUS  FT_StopInTask(FT_HANDLE ftHandle);
FT_PREFIX FT_STATUS  FT_RestartInTask(FT_HANDLE ftHandle);
FT_PREFIX FT_STATUS  FT_SetResetPipeRetryCount(FT_HANDLE ftHandle, DWORD dwCount);
FT_PREFIX FT_STATUS  FT_ResetPort(FT_HANDLE ftHandle);


#define FT_SUCCESS(status) ((status) == FT_OK)

//
// FT_OpenEx Flags
//

#define FT_OPEN_BY_SERIAL_NUMBER    1
#define FT_OPEN_BY_DESCRIPTION      2
#define FT_OPEN_BY_LOCATION			4

//
// FT_ListDevices Flags (used in conjunction with FT_OpenEx Flags
//

#define FT_LIST_NUMBER_ONLY			0x80000000
#define FT_LIST_BY_INDEX			0x40000000
#define FT_LIST_ALL					0x20000000

#define FT_LIST_MASK (FT_LIST_NUMBER_ONLY|FT_LIST_BY_INDEX|FT_LIST_ALL)

//
// Baud Rates
//

#define FT_BAUD_300			300
#define FT_BAUD_600			600
#define FT_BAUD_1200		1200
#define FT_BAUD_2400		2400
#define FT_BAUD_4800		4800
#define FT_BAUD_9600		9600
#define FT_BAUD_14400		14400
#define FT_BAUD_19200		19200
#define FT_BAUD_38400		38400
#define FT_BAUD_57600		57600
#define FT_BAUD_115200		115200
#define FT_BAUD_230400		230400
#define FT_BAUD_460800		460800
#define FT_BAUD_921600		921600

//
// Word Lengths
//

#define FT_BITS_8			(UCHAR) 8
#define FT_BITS_7			(UCHAR) 7
#define FT_BITS_6			(UCHAR) 6
#define FT_BITS_5			(UCHAR) 5

//
// Stop Bits
//

#define FT_STOP_BITS_1		(UCHAR) 0
#define FT_STOP_BITS_1_5	(UCHAR) 1
#define FT_STOP_BITS_2		(UCHAR) 2

//
// Parity
//

#define FT_PARITY_NONE		(UCHAR) 0
#define FT_PARITY_ODD		(UCHAR) 1
#define FT_PARITY_EVEN		(UCHAR) 2
#define FT_PARITY_MARK		(UCHAR) 3
#define FT_PARITY_SPACE		(UCHAR) 4

//
// Flow Control
//

#define FT_FLOW_NONE        0x0000
#define FT_FLOW_RTS_CTS     0x0100
#define FT_FLOW_DTR_DSR     0x0200
#define FT_FLOW_XON_XOFF    0x0400

//
// Purge rx and tx buffers
//
#define FT_PURGE_RX         1
#define FT_PURGE_TX         2

//
// Events
//

typedef void(*PFT_EVENT_HANDLER)(DWORD, DWORD);

#define FT_EVENT_RXCHAR		    1
#define FT_EVENT_MODEM_STATUS   2

//
// Timeouts
//

#define FT_DEFAULT_RX_TIMEOUT   300
#define FT_DEFAULT_TX_TIMEOUT   300

//
// Device types
//

typedef ULONG	FT_DEVICE;

enum {
    FT_DEVICE_BM,
    FT_DEVICE_AM,
    FT_DEVICE_100AX,
    FT_DEVICE_UNKNOWN,
    FT_DEVICE_2232C,
    FT_DEVICE_232R
};


//
// structure to hold program data for FT_Program function
//
typedef struct ft_program_data {

    DWORD Signature1;			// Header - must be 0x00000000 
    DWORD Signature2;			// Header - must be 0xffffffff
    DWORD Version;				// Header - FT_PROGRAM_DATA version
    //          0 = original
    //          1 = FT2232C extensions

    WORD VendorId;				// 0x0403
    WORD ProductId;				// 0x6001
    char *Manufacturer;			// "FTDI"
    char *ManufacturerId;		// "FT"
    char *Description;			// "USB HS Serial Converter"
    char *SerialNumber;			// "FT000001" if fixed, or NULL
    WORD MaxPower;				// 0 < MaxPower <= 500
    WORD PnP;					// 0 = disabled, 1 = enabled
    WORD SelfPowered;			// 0 = bus powered, 1 = self powered
    WORD RemoteWakeup;			// 0 = not capable, 1 = capable
    //
    // Rev4 extensions
    //
    UCHAR Rev4;					// non-zero if Rev4 chip, zero otherwise
    UCHAR IsoIn;				// non-zero if in endpoint is isochronous
    UCHAR IsoOut;				// non-zero if out endpoint is isochronous
    UCHAR PullDownEnable;		// non-zero if pull down enabled
    UCHAR SerNumEnable;			// non-zero if serial number to be used
    UCHAR USBVersionEnable;		// non-zero if chip uses USBVersion
    WORD USBVersion;			// BCD (0x0200 => USB2)
    //
    // Rev 5 (FT2232C) extensions
    //
    UCHAR Rev5;					// non-zero if Rev5 chip, zero otherwise
    UCHAR IsoInA;				// non-zero if in endpoint is isochronous
    UCHAR IsoInB;				// non-zero if in endpoint is isochronous
    UCHAR IsoOutA;				// non-zero if out endpoint is isochronous
    UCHAR IsoOutB;				// non-zero if out endpoint is isochronous
    UCHAR PullDownEnable5;		// non-zero if pull down enabled
    UCHAR SerNumEnable5;		// non-zero if serial number to be used
    UCHAR USBVersionEnable5;	// non-zero if chip uses USBVersion
    WORD USBVersion5;			// BCD (0x0200 => USB2)
    UCHAR AIsHighCurrent;		// non-zero if interface is high current
    UCHAR BIsHighCurrent;		// non-zero if interface is high current
    UCHAR IFAIsFifo;			// non-zero if interface is 245 FIFO
    UCHAR IFAIsFifoTar;			// non-zero if interface is 245 FIFO CPU target
    UCHAR IFAIsFastSer;			// non-zero if interface is Fast serial
    UCHAR AIsVCP;				// non-zero if interface is to use VCP drivers
    UCHAR IFBIsFifo;			// non-zero if interface is 245 FIFO
    UCHAR IFBIsFifoTar;			// non-zero if interface is 245 FIFO CPU target
    UCHAR IFBIsFastSer;			// non-zero if interface is Fast serial
    UCHAR BIsVCP;				// non-zero if interface is to use VCP drivers

    /////////////////////////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////// added by RS ////////////////////////////////////////
    /////////////////////////////////////////////////////////////////////////////////////////////

    //
    // Rev 6 (FT232R) extensions
    //
    UCHAR UseExtOsc;            // Use External Oscillator
    UCHAR HighDriveIOs;         // High Drive I/Os
    UCHAR EndpointSize;         // Endpoint size
    UCHAR PullDownEnableR;      // non-zero if pull down enabled
    UCHAR SerNumEnableR;        // non-zero if serial number to be used
    UCHAR InvertTXD;            // non-zero if invert TXD
    UCHAR InvertRXD;            // non-zero if invert RXD
    UCHAR InvertRTS;            // non-zero if invert RTS
    UCHAR InvertCTS;            // non-zero if invert CTS
    UCHAR InvertDTR;            // non-zero if invert DTR
    UCHAR InvertDSR;            // non-zero if invert DSR
    UCHAR InvertDCD;            // non-zero if invert DCD
    UCHAR InvertRI;             // non-zero if invert RI
    UCHAR Cbus0;                // Cbus Mux control
    UCHAR Cbus1;                // Cbus Mux control
    UCHAR Cbus2;                // Cbus Mux control
    UCHAR Cbus3;                // Cbus Mux control
    UCHAR Cbus4;                // Cbus Mux control
    UCHAR RIsD2XX;              // non-zero if using D2XX driver
    //
    // Rev 7 (FT2232H) Extensions
    //
    UCHAR PullDownEnable7;      // non-zero if pull down enabled
    UCHAR SerNumEnable7;        // non-zero if serial number to be used
    UCHAR ALSlowSlew;           // non-zero if AL pins have slow slew
    UCHAR ALSchmittInput;       // non-zero if AL pins are Schmitt input
    UCHAR ALDriveCurrent;       // valid values are 4mA, 8mA, 12mA, 16mA
    UCHAR AHSlowSlew;           // non-zero if AH pins have slow slew
    UCHAR AHSchmittInput;       // non-zero if AH pins are Schmitt input
    UCHAR AHDriveCurrent;       // valid values are 4mA, 8mA, 12mA, 16mA
    UCHAR BLSlowSlew;           // non-zero if BL pins have slow slew
    UCHAR BLSchmittInput;       // non-zero if BL pins are Schmitt input
    UCHAR BLDriveCurrent;       // valid values are 4mA, 8mA, 12mA, 16mA
    UCHAR BHSlowSlew;           // non-zero if BH pins have slow slew
    UCHAR BHSchmittInput;       // non-zero if BH pins are Schmitt input
    UCHAR BHDriveCurrent;       // valid values are 4mA, 8mA, 12mA, 16mA
    UCHAR IFAIsFifo7;           // non-zero if interface is 245 FIFO
    UCHAR IFAIsFifoTar7;        // non-zero if interface is 245 FIFO CPU target
    UCHAR IFAIsFastSer7;        // non-zero if interface is Fast serial
    UCHAR AIsVCP7;              // non-zero if interface is to use VCP drivers
    UCHAR IFBIsFifo7;           // non-zero if interface is 245 FIFO
    UCHAR IFBIsFifoTar7;        // non-zero if interface is 245 FIFO CPU target
    UCHAR IFBIsFastSer7;        // non-zero if interface is Fast serial
    UCHAR BIsVCP7;              // non-zero if interface is to use VCP drivers
    UCHAR PowerSaveEnable;      // non-zero if using BCBUS7 to save power for self-powered designs
    //
    // Rev 8 (FT4232H) Extensions
    //
    UCHAR PullDownEnable8;      // non-zero if pull down enabled
    UCHAR SerNumEnable8;        // non-zero if serial number to be used
    UCHAR ASlowSlew;            // non-zero if A pins have slow slew
    UCHAR ASchmittInput;        // non-zero if A pins are Schmitt input
    UCHAR ADriveCurrent;        // valid values are 4mA, 8mA, 12mA, 16mA
    UCHAR BSlowSlew;            // non-zero if B pins have slow slew
    UCHAR BSchmittInput;        // non-zero if B pins are Schmitt input
    UCHAR BDriveCurrent;        // valid values are 4mA, 8mA, 12mA, 16mA
    UCHAR CSlowSlew;            // non-zero if C pins have slow slew
    UCHAR CSchmittInput;        // non-zero if C pins are Schmitt input
    UCHAR CDriveCurrent;        // valid values are 4mA, 8mA, 12mA, 16mA
    UCHAR DSlowSlew;            // non-zero if D pins have slow slew
    UCHAR DSchmittInput;        // non-zero if D pins are Schmitt input
    UCHAR DDriveCurrent;        // valid values are 4mA, 8mA, 12mA, 16mA
    UCHAR ARIIsTXDEN;           // non-zero if port A uses RI as RS485 TXDEN
    UCHAR BRIIsTXDEN;           // non-zero if port B uses RI as RS485 TXDEN
    UCHAR CRIIsTXDEN;           // non-zero if port C uses RI as RS485 TXDEN
    UCHAR DRIIsTXDEN;           // non-zero if port D uses RI as RS485 TXDEN
    UCHAR AIsVCP8;              // non-zero if interface is to use VCP drivers
    UCHAR BIsVCP8;              // non-zero if interface is to use VCP drivers
    UCHAR CIsVCP8;              // non-zero if interface is to use VCP drivers
    UCHAR DIsVCP8;              // non-zero if interface is to use VCP drivers
    //
    // Rev 9 (FT232H) Extensions
    //
    UCHAR PullDownEnableH;      // non-zero if pull down enabled
    UCHAR SerNumEnableH;        // non-zero if serial number to be used
    UCHAR ACSlowSlewH;          // non-zero if AC pins have slow slew
    UCHAR ACSchmittInputH;      // non-zero if AC pins are Schmitt input
    UCHAR ACDriveCurrentH;      // valid values are 4mA, 8mA, 12mA, 16mA
    UCHAR ADSlowSlewH;          // non-zero if AD pins have slow slew
    UCHAR ADSchmittInputH;      // non-zero if AD pins are Schmitt input
    UCHAR ADDriveCurrentH;      // valid values are 4mA, 8mA, 12mA, 16mA
    UCHAR Cbus0H;               // Cbus Mux control
    UCHAR Cbus1H;               // Cbus Mux control
    UCHAR Cbus2H;               // Cbus Mux control
    UCHAR Cbus3H;               // Cbus Mux control
    UCHAR Cbus4H;               // Cbus Mux control
    UCHAR Cbus5H;               // Cbus Mux control
    UCHAR Cbus6H;               // Cbus Mux control
    UCHAR Cbus7H;               // Cbus Mux control
    UCHAR Cbus8H;               // Cbus Mux control
    UCHAR Cbus9H;               // Cbus Mux control
    UCHAR IsFifoH;              // non-zero if interface is 245 FIFO
    UCHAR IsFifoTarH;           // non-zero if interface is 245 FIFO CPU target
    UCHAR IsFastSerH;           // non-zero if interface is Fast serial
    UCHAR IsFT1248H;            // non-zero if interface is FT1248
    UCHAR FT1248CpolH;          // FT1248 clock polarity - clock idle high (1) or clock idle low (0)
    UCHAR FT1248LsbH;           // FT1248 data is LSB (1) or MSB (0)
    UCHAR FT1248FlowControlH;   // FT1248 flow control enable
    UCHAR IsVCPH;               // non-zero if interface is to use VCP drivers
    UCHAR PowerSaveEnableH;     // non-zero if using ACBUS7 to save power for self-powered designs

} FT_PROGRAM_DATA, *PFT_PROGRAM_DATA;

//
// Win32 COMM API type functions
//
typedef struct _FTCOMSTAT {
    DWORD fCtsHold : 1;
    DWORD fDsrHold : 1;
    DWORD fRlsdHold : 1;
    DWORD fXoffHold : 1;
    DWORD fXoffSent : 1;
    DWORD fEof : 1;
    DWORD fTxim : 1;
    DWORD fReserved : 25;
    DWORD cbInQue;
    DWORD cbOutQue;
} FTCOMSTAT, *LPFTCOMSTAT;

typedef struct _FTDCB {
    DWORD DCBlength;      /* sizeof(FTDCB)                   */
    DWORD BaudRate;       /* Baudrate at which running       */
    DWORD fBinary : 1;     /* Binary Mode (skip EOF check)    */
    DWORD fParity : 1;     /* Enable parity checking          */
    DWORD fOutxCtsFlow : 1; /* CTS handshaking on output       */
    DWORD fOutxDsrFlow : 1; /* DSR handshaking on output       */
    DWORD fDtrControl : 2;  /* DTR Flow control                */
    DWORD fDsrSensitivity : 1; /* DSR Sensitivity              */
    DWORD fTXContinueOnXoff : 1; /* Continue TX when Xoff sent */
    DWORD fOutX : 1;       /* Enable output X-ON/X-OFF        */
    DWORD fInX : 1;        /* Enable input X-ON/X-OFF         */
    DWORD fErrorChar : 1;  /* Enable Err Replacement          */
    DWORD fNull : 1;       /* Enable Null stripping           */
    DWORD fRtsControl : 2;  /* Rts Flow control                */
    DWORD fAbortOnError : 1; /* Abort all reads and writes on Error */
    DWORD fDummy2 : 17;     /* Reserved                        */
    WORD wReserved;       /* Not currently used              */
    WORD XonLim;          /* Transmit X-ON threshold         */
    WORD XoffLim;         /* Transmit X-OFF threshold        */
    BYTE ByteSize;        /* Number of bits/byte, 4-8        */
    BYTE Parity;          /* 0-4=None,Odd,Even,Mark,Space    */
    BYTE StopBits;        /* 0,1,2 = 1, 1.5, 2               */
    char XonChar;         /* Tx and Rx X-ON character        */
    char XoffChar;        /* Tx and Rx X-OFF character       */
    char ErrorChar;       /* Error replacement char          */
    char EofChar;         /* End of Input character          */
    char EvtChar;         /* Received Event character        */
    WORD wReserved1;      /* Fill for now.                   */
} FTDCB, *LPFTDCB;

typedef struct _FTTIMEOUTS {
    DWORD ReadIntervalTimeout;          /* Maximum time between read chars. */
    DWORD ReadTotalTimeoutMultiplier;   /* Multiplier of characters.        */
    DWORD ReadTotalTimeoutConstant;     /* Constant in milliseconds.        */
    DWORD WriteTotalTimeoutMultiplier;  /* Multiplier of characters.        */
    DWORD WriteTotalTimeoutConstant;    /* Constant in milliseconds.        */
} FTTIMEOUTS, *LPFTTIMEOUTS;



FT_PREFIX FT_STATUS FT_EE_Program(FT_HANDLE ftHandle, PFT_PROGRAM_DATA pData);
FT_PREFIX FT_STATUS FT_EE_Read(FT_HANDLE ftHandle, PFT_PROGRAM_DATA pData);


#endif //CLI_FTD2XXX_H_