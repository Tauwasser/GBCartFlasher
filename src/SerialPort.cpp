/*****************************************************************************
** SerialPort.cpp
** Author: Kraku
*****************************************************************************/
#include "const.h"
#include "SerialPort.h"

extern "C"{
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
/*
 * Header files for RS232C support under Linux
 */
#include <unistd.h>
#include <termios.h>
#include <fcntl.h>
}

SerialPort::SerialPort()
	:AbstractPort()
	
{
	descriptor = -1;
	/* Descriptor -1 is invalid - something like NULL */
}


bool SerialPort::open_port(QString port_name)
{
  /* Open port in RW mode, don't check DCD line */
  descriptor = open (port_name.toAscii(), O_RDWR | O_NOCTTY | O_NDELAY);
  if (descriptor != -1)			/* is port opened */
  {
    opened = TRUE;
    struct termios ts;		/* structure describing port */
    tcgetattr (descriptor, &ts);	/* get actual state of port */

    /*
     * set line settings
     */
    cfsetispeed (&ts, (speed_t)B115200);
    cfsetospeed (&ts, (speed_t)B115200);
    ts.c_cflag |= (CLOCAL | CREAD);	/* important */
    ts.c_cflag &= ~PARENB;	/* no parity */
    ts.c_cflag &= ~CSTOPB;	/* 1 stop bit */
    ts.c_cflag &= ~CSIZE;
    ts.c_cflag |= CS8;		/* 8 data bits */

    /*
     * raw input/output 
     */
    ts.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG);
    ts.c_iflag &= ~(IXON | IXOFF | IXANY);
    ts.c_oflag &= ~OPOST;
    ts.c_iflag &= ~(INLCR | IGNCR | ICRNL);

    /*
     * set all info 
     */
    tcsetattr (descriptor, TCSANOW, &ts);
    fcntl (descriptor, F_SETFL, FNDELAY); /* don't wait for data while reading */
    return TRUE;
  }
  else
    return FALSE;
}

bool SerialPort::close_port ()
{
  if (descriptor == -1)
    return FALSE;
  else
  {
    close (descriptor);
    opened = FALSE;
    return TRUE;
  }
}

int SerialPort::send_packet(unsigned char packet[PACKETSIZE])
{
  return write (descriptor, packet, PACKETSIZE);
}


bool SerialPort::send_char (unsigned char character)
{
  if (write (descriptor, &character, 1))
    return TRUE;
  else
    return FALSE;
}

int SerialPort::receive_char (void)
{
  time_t tp = time (NULL);	/* save current time */
  unsigned char character;
  int read_count = 0;

  do
  {
    read_count = read (descriptor, &character, 1);	/* try to read char... */
  }
  while (read_count <= 0 && time (NULL) - tp < SLEEPTIME);

  /*
   * ...until done or SLEEPTIME is over
   */
  if (read_count == 0)
    return TIMEOUT;
  
  /* ACK and END are only accepted chars */
  if (character == ACK || character == END)
    return character;
  else
    return NAK;
}

int SerialPort::receive_packet (unsigned char *packet)
{
  time_t tp = time (NULL);
  int read_count;

  do
  {
    read_count = read (descriptor, packet, 1);
  }
  while (read_count <= 0 && time (NULL) - tp < SLEEPTIME);

  if (read_count <= 0)
    return TIMEOUT;
  else
  {
    if (packet[0] != DATA)	/* if it's not a packet, then treat it as char */
    {
      if (packet[0] == ACK)
	return ACK;
      else if (packet[0] == END)
	return END;
      else
	return NAK;
    }
    else			/* if packet, read rest of it */
    {
      int data_left = PACKETSIZE - 1;
      tp = time (NULL);

      do
      {

	/*
	 * read rest of data and put it in packet
	 */
	read_count =
	  read (descriptor, &packet[PACKETSIZE - data_left], data_left);
	if (read_count > 0)
	{
	  data_left -= read_count;
	  tp = time (NULL);	/* reset wait timer */
	}
      }
      while (data_left != 0 && time (NULL) - tp < SLEEPTIME);
      /*
       * read until packet is full or time ends 
       */
      if (data_left > 0)
	return TIMEOUT;		
      else
	return DATA;		/* DATA signals read of full packet */
    }
  }
}
