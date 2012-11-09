/*****************************************************************************
** SerialPortWin.cpp
** Author: Kraku
*****************************************************************************/
#include "const.h"
#include "AbstractPort.h"
#include "SerialPortWin.h"
#include "Settings.h"

extern "C"{
#include <time.h>
#include <stdio.h>
#include <stdlib.h>

#include <windows.h>
}

SerialPortWin::SerialPortWin()
	:AbstractPort()
	
{
	hCommDev=NULL;
}


bool SerialPortWin::open_port(QString port_name)
{
	hCommDev = CreateFile((WCHAR*)port_name.utf16(), GENERIC_READ | GENERIC_WRITE,
        0, NULL, OPEN_EXISTING, 0, NULL);
    if (hCommDev != INVALID_HANDLE_VALUE) /* check if port exists */
    {
        SetupComm(hCommDev,/* port handle */ 
                  PACKETSIZE,/* input queue size */
                  PACKETSIZE);/* output queue size */
        dcb.DCBlength = sizeof(dcb); /* size of structure */
        GetCommState(hCommDev, &dcb);

/* speed setup */
        if(Settings::speed == STANDARD || Settings::speed == HIGH){        
            dcb.BaudRate=CBR_115200;
        }else if(Settings::speed == LOW){
            dcb.BaudRate=CBR_57600;
        }

        dcb.Parity = NOPARITY; /* setup parity */
        dcb.StopBits = ONESTOPBIT; /* stop bits */
        dcb.ByteSize = 8; /* data bits count */
        dcb.fParity = FALSE; /* no parity check */
        SetCommState(hCommDev, &dcb);
        return TRUE;
    }
    else
        return FALSE; 

  
}

bool SerialPortWin::close_port ()
{
if((hCommDev == NULL) || (hCommDev == INVALID_HANDLE_VALUE))
        return FALSE;
    else
    {
        CloseHandle(hCommDev);
        return TRUE;
    }

}

int SerialPortWin::send_packet (unsigned char packet[PACKETSIZE])
{
   DWORD sendCount;
    
    WriteFile(hCommDev, packet, PACKETSIZE, &sendCount, NULL);
    return sendCount;


}


bool SerialPortWin::send_char (unsigned char character)
{
  	if(TransmitCommChar(hCommDev,character))
        return TRUE;
    else
        return FALSE;
 

}

int SerialPortWin::receive_char (void)
{
	time_t tp = time(NULL);
    unsigned char character;
    DWORD receivedCount=0;
    ClearCommError(hCommDev, &Errors, &Stat);
    do
    {
        if (Stat.cbInQue > 0)
            ReadFile(hCommDev, &character, 1, &receivedCount, NULL);
        if(receivedCount != 0)
            break;
        ClearCommError(hCommDev, &Errors, &Stat);
    }while(time(NULL) - tp < SLEEPTIME);

    if (receivedCount == 0)
        return TIMEOUT;
    if(character == ACK || character == END)
        return character;
    else 
        return NAK;        

     
}

int SerialPortWin::receive_packet (unsigned char *packet)
{
  time_t tp = time(NULL);
    DWORD receivedCount=0, bytesToReceive;
    ClearCommError(hCommDev, &Errors, &Stat);
    
    do
    {
        if (Stat.cbInQue > 0)
            ReadFile(hCommDev, &packet[0], 1, &receivedCount, NULL);
        ClearCommError(hCommDev, &Errors, &Stat);
    } while(time(NULL) - tp < SLEEPTIME && receivedCount == 0);
    if (receivedCount == 0)
        return TIMEOUT;
    else
    {
        if(packet[0] != DATA)
        {
            if(packet[0] == ACK)
                return ACK;
            else if(packet[0] == END)
                    return END;
                 else
                    return NAK;
        }
        else
        {
            unsigned int bytesLeft = PACKETSIZE-1;
            tp = time(NULL);
            receivedCount = 0;
            ClearCommError(hCommDev, &Errors, &Stat);
            do
            {
                if (Stat.cbInQue > 0)
                {
                    if (Stat.cbInQue > bytesLeft)
                        bytesToReceive = bytesLeft;
                    else
                        bytesToReceive = Stat.cbInQue;

                        ReadFile(hCommDev, &packet[PACKETSIZE-bytesLeft], bytesToReceive, &receivedCount, NULL);
                        bytesLeft -= receivedCount;
                        tp = time(NULL);
                }
                
                   
                ClearCommError(hCommDev, &Errors, &Stat);
            }while(time(NULL) - tp < SLEEPTIME && bytesLeft != 0); 
            if(bytesLeft > 0)
                return TIMEOUT;
            else
                return DATA;
        }
    }

}
