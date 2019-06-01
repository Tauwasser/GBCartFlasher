/*****************************************************************************
** USBPort.h - USB driver for Linux based on libftdi
** Author: Kraku
*****************************************************************************/
#ifndef _USBPORT_H_
#define _USBPORT_H_
#include "AbstractPort.h"
//#include <ftdi.h>
#include <ftd2xx.h>
#include <time.h>


class USBPort:public AbstractPort
{
Q_OBJECT
  //struct ftdi_context ftdic;
  FILE *file;
	
	// new version
	//////////////////////
	FT_HANDLE ftHandle; // = NULL;
	//char * 	pcBufLD;
	//char cBufLD[64];	// device serial name
	FT_STATUS	ftStatus;	// get name
	//DWORD	dwRxSize = 0;
	//DWORD 	dwBytesWritten, dwBytesRead;
	//int	iNumDevs;

  bool opened;
public:
  USBPort ();
  bool open_port (QString port_name);
  bool close_port ();
  int receive_char ();
  bool send_char (unsigned char character);
  int receive_packet (unsigned char *packet);
  int send_packet (unsigned char *packet);
  bool isOpen ()
  {
    return opened;
  }
signals:
  void error (int err);
};

#endif
