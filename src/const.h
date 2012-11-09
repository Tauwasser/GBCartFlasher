/*****************************************************************************
** const.h - definitions of constants used in program
** Author: Kraku
*****************************************************************************/
#ifndef _CONST_H_
#define _CONST_H_

/* control characters */
enum cchars
{ ACK = 0xAA, NAK = 0xF0, END = 0x0F, DATA = 0x55 };

#define SLEEPTIME 3		/* time to wait for connection in sec. */
#define DELTIME 60		/* time to wait for delete in sec. */
#define PACKETSIZE 72
#define FRAMESIZE 64
#define AUTOSIZE -1
#define PORTS_COUNT 4
#define VER "1.1"

#define USB 0
#define SERIAL 1

/* strings for version information */
#ifdef Q_WS_X11
#define SYSTEM "LINUX"
#define DEVELOPED "GCC 4.1.1 + QT 4.3.2"
#endif

#ifdef Q_WS_WIN
#define SYSTEM "WINDOWS"
#define DEVELOPED "Dev-C++ 4.9.9.2 + QT 4.3.2"
#endif

/* return values and errors */
enum error_t
{ TIMEOUT = -1, FILEERROR_O = -2, FILEERROR_W = -3, FILEERROR_R =
    -4, SEND_ERROR = -5,
  BAD_PACKET = -6, BAD_PARAMS = -7, PORT_ERROR = -8, WRONG_SIZE = -9
};

/* packet types */
#define CONFIG 0x00
#define NORMAL_DATA 0x01
#define LAST_DATA 0x02
#define ERASE 0x03
#define STATUS 0x04

#define RESERVED 0x00
#define NREAD_ID 0x00
#define READ_ID 0x01

/* operations */
#define RROM 0x00
#define RRAM 0x01
#define WROM 0x02
#define WRAM 0x03
#define EFLA 0x00
#define ERAM 0x01



enum alg_t
{ ALG16 = 0x00, ALG12 = 0x01 };
enum dap_t
{ LONGER = 0x00, DEFAULT = 0x01, DATAPOLL = 0x02, TOGGLE = 0x03 };
enum speed_type
{ LOW = 0x00, STANDARD = 0x01, HIGH = 0x02 };

/* MBC types */
enum mbc_t
{ MBCAUTO = 0x00, MBC1 = 0x01, MBC2 = 0x02, MBC3 = 0x03,
  ROMONLY = 0x04, MBC5 = 0x05, RUMBLE = 0x06
};

#endif
