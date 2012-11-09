/*****************************************************************************
** AbstractPort.h
** Author: Kraku
*****************************************************************************/
#ifndef _ABSTRACTPORT_H_
#define _ABSTRACTPORT_H_
#include <QObject>
#include <QString>

class AbstractPort:public QObject
{
Q_OBJECT public:
  virtual bool open_port (QString port_name) = 0;
  virtual bool close_port () = 0;
  virtual int receive_char () = 0;
  virtual bool send_char (unsigned char character) = 0;
  virtual int receive_packet (unsigned char *packet) = 0;
  virtual int send_packet (unsigned char *packet) = 0;
    virtual ~AbstractPort ()
  {
  };
  virtual bool isOpen () = 0;
signals:
  void error (int err);

};

#endif
