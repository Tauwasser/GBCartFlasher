/*****************************************************************************
** ReadRamThread.h
** Author: Kraku
*****************************************************************************/
#ifndef _READRAMTHREAD_H_
#define _READRAMTHREAD_H_
#include <QThread>
#include "AbstractPort.h"
#include "Logic.h"

class ReadRamThread:public QThread
{
Q_OBJECT public:
  virtual void run ();
    ReadRamThread ()
  {
  }
  bool end;
  FILE *file;
  char _2k;
  char mbc;
  char algorythm;
  char dap;
  int page_count;
  AbstractPort *port;
  public slots:void canceled (void);

signals:
  void set_progress (int ile, int max);
  void error (int err);
};

#endif
