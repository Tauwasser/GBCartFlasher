/*****************************************************************************
** Console.h - header of Console used to print infos from other
** modules
** Author: Kraku
*****************************************************************************/
#ifndef _CONSOLE_H_
#define _CONSOLE_H_

#include <QTextEdit>
#include <QString>
#include <QPalette>
#include <QColor>

class Console:public QTextEdit
{
public:
  Console (QWidget * parent):QTextEdit (parent)
  {
    this->setMinimumWidth (400);
    this->setReadOnly (TRUE);
    QPalette palette;
      palette.setColor (QPalette::Base, QColor (0, 255, 0));
      this->setPalette (palette);
    QFont font ("Courier");
      font.setPointSize (8);
      this->setFont (font);
  }

  void print (QString string)
  {
    append (string);
  }

  void line ()
  {
#ifdef Q_WS_X11
    print ("<br>");
#else
    print ("<hr><br>");
#endif
  }
};

#endif
