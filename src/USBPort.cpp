/*****************************************************************************
** USBPort.cpp
** Author: Kraku
*****************************************************************************/
#include "const.h"
#include "USBPort.h"
#include "Settings.h"
//#include <ftdi.h>
#include <iostream>
#include <ftd2xx.h>
extern "C"
{
#include <time.h>
#include <stdio.h>
#include <stdlib.h>

}


USBPort::USBPort ():
AbstractPort ()
{
  //ftdi_init (&ftdic);
	//ftHandle = NULL;
	//iNumDevs = 0;
	//pcBufLD = cBufLD;
	
}


bool
USBPort::open_port (QString /*port_name*/)
{
	// NEW CODE
	////////////////////////
	/*ftStatus = FT_ListDevices(pcBufLD, &iNumDevs, FT_LIST_ALL | FT_OPEN_BY_SERIAL_NUMBER);
	
	if(ftStatus != FT_OK) {
		printf("Error: FT_ListDevices(%d)\n", ftStatus);
		return FALSE;
	}
	
	printf("Device Serial Number - %s\n", cBufLD);
	// SETUP
	if((ftStatus = FT_OpenEx(cBufLD, FT_OPEN_BY_SERIAL_NUMBER, &ftHandle)) != FT_OK){
		/* 
		 This can fail if the ftdi_sio driver is loaded
		 use lsmod to check this and rmmod ftdi_sio to remove
		 also rmmod usbserial
		 */
		/*printf("Error FT_OpenEx(), device\n");
		return FALSE;
	}*/
	
	if((ftStatus = FT_Open(0, &ftHandle)) != FT_OK){
		/* 
		 This can fail if the ftdi_sio driver is loaded
		 use lsmod to check this and rmmod ftdi_sio to remove
		 also rmmod usbserial
		 */
		printf("Error FT_Open(), device\n");
		return FALSE;
	}
	
	printf("Opened device \n");
	//printf("Opened device %s\n", cBufLD);
	
	// CHOOSE SPEED
	
	
	
  //if (ftdi_usb_open (&ftdic, 0x0403, 0x6001) < 0)
  //  return FALSE;
/* choose speed */
  if (Settings::speed == STANDARD)
    {
			if ((ftStatus = FT_SetBaudRate(ftHandle, 185000)) != FT_OK) {
				/* 
				 This can fail if the ftdi_sio driver is loaded
				 use lsmod to check this and rmmod ftdi_sio to remove
				 also rmmod usbserial
				 */
				printf("Error FT_OpenEx(%d) \n", ftStatus);
				return FALSE;
			}
    }
  else if (Settings::speed == LOW)
    {
			if ((ftStatus = FT_SetBaudRate(ftHandle, 125000)) != FT_OK) {
				/* 
				 This can fail if the ftdi_sio driver is loaded
				 use lsmod to check this and rmmod ftdi_sio to remove
				 also rmmod usbserial
				 */
				printf("Error FT_OpenEx(%d) \n", ftStatus);
				return FALSE;
			}
    //  if (ftdi_set_baudrate (&ftdic, 125000) < 0)
	//return FALSE;
    }
  else if (Settings::speed == HIGH)
    {
			if ((ftStatus = FT_SetBaudRate(ftHandle, 375000)) != FT_OK) {
				/* 
				 This can fail if the ftdi_sio driver is loaded
				 use lsmod to check this and rmmod ftdi_sio to remove
				 also rmmod usbserial
				 */
				printf("Error FT_OpenEx(%d) \n", ftStatus);
				return FALSE;
			}
    }
	
	if (FT_SetLatencyTimer(&ftHandle, 2) != FT_OK)
		return FALSE;

  //if (ftdi_set_latency_timer (&ftdic, 2) < 0)
    //return FALSE;
	if (FT_SetDataCharacteristics(&ftHandle, FT_BITS_8, FT_STOP_BITS_1, FT_PARITY_NONE) != FT_OK)
		return FALSE;
	
  //if (ftdi_set_line_property (&ftdic, BITS_8, STOP_BIT_1, NONE) < 0)
    //return FALSE;
  //if(FT_SetTimeouts(ftHandle,5000,0) != FT_OK)
  //      return FALSE;
  //if(ftdi_enable_bitbang(&ftdic,0xFF) < 0)
  //      return FALSE;

  return TRUE;			/* all ok */



}

bool
USBPort::close_port ()
{
	//NEW
	if (ftHandle != NULL)
	{
		FT_Close(ftHandle);
		ftHandle = NULL;
		printf("Closed Device \n");
	}
	//if (pcBufRead)
	//	free(pcBufRead);
  //ftdi_usb_close (&ftdic);
  return TRUE;
}

int
USBPort::send_packet (unsigned char packet[PACKETSIZE])
{
  DWORD bytesSent;
	ftStatus = FT_Write(ftHandle, packet, sizeof(packet), &bytesSent);
  //bytesSent = ftdi_write_data (&ftdic, packet, PACKETSIZE);
  return (int)bytesSent;

}


bool
USBPort::send_char (unsigned char character)
{
  DWORD bytesSent;
	ftStatus = FT_Write(ftHandle, &character, sizeof(character), &bytesSent);
  //bytesSent = ftdi_write_data (&ftdic, &character, 1);
  return bytesSent == 1;

}

int
USBPort::receive_char (void)
{
  time_t tp = time (NULL);
  unsigned char character;
  DWORD bytesReceived = 0;
  do
    {
			// NEW
			ftStatus = FT_Read(ftHandle, &character, 1, &bytesReceived);
      //bytesReceived = ftdi_read_data (&ftdic, &character, 1);
      if (bytesReceived != 0)
	break;
    }
  while (time (NULL) - tp < SLEEPTIME);

  if (bytesReceived == 0)
    return TIMEOUT;
  if (character == ACK || character == END)
    return character;
  else
    return NAK;

}

int
USBPort::receive_packet (unsigned char *packet)
{
  time_t tp = time (NULL);
  DWORD bytesReceived = 0, bytesLeft;
  do
    {
			ftStatus = FT_Read(ftHandle, packet, 1, &bytesReceived);
      //bytesReceived = ftdi_read_data (&ftdic, packet, 1);
    }
  while (time (NULL) - tp < SLEEPTIME && bytesReceived == 0);
  if (bytesReceived == 0)
    return TIMEOUT;
  else
    {
      if (packet[0] != DATA)
	{
	  if (packet[0] == ACK)
	    return ACK;
	  else if (packet[0] == END)
	    return END;
	  else
	    return NAK;
	}
      else
	{
		DWORD remaining = PACKETSIZE - 1;
	  //unsigned int remaining = PACKETSIZE - 1;
	  tp = time (NULL);
	  bytesReceived = 0;
	  do
	    {
	      bytesLeft = remaining;

				ftStatus = FT_Read(ftHandle, &packet[PACKETSIZE - remaining], bytesLeft, &bytesReceived);
	      //bytesReceived =
		//ftdi_read_data (&ftdic, &packet[PACKETSIZE - remaining],
				//bytesLeft);
	      remaining -= bytesReceived;
	      tp = time (NULL);



	    }
	  while (time (NULL) - tp < SLEEPTIME && remaining != 0);
	  if (remaining > 0)
	    return TIMEOUT;
	  else
	    return DATA;
	}
    }
}
