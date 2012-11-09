/*****************************************************************************
** SerialPort.h - SerialPort driver for Linux/Posix based systems
** Author: Kraku
*****************************************************************************/
#ifndef _SERIALPORT_H_
#define _SERIALPORT_H_
#include "AbstractPort.h"

class SerialPort:public AbstractPort
{
Q_OBJECT
  int descriptor;
  bool opened;
public:
  SerialPort ();
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
