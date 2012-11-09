/*****************************************************************************
** WriteFlashThread.h
** Author: Kraku
*****************************************************************************/
#ifndef _WRITEFLASHTHREAD_H_
#define _WRITEFLASHTHREAD_H_
#include <QThread>
#include "AbstractPort.h"
#include "Logic.h"

class WriteFlashThread:public QThread
{
Q_OBJECT public:
  WriteFlashThread ()
  {
  }
  virtual void run ();
  FILE *file;
  char mbc;
  char algorythm;
  char dap;
  int page_count;
  bool end;
  AbstractPort *port;

  public slots:void canceled (void);
signals:
  void set_progress (int ile, int max);
  void error (int err);
};

#endif
