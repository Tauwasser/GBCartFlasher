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
      palette.setColor (QPalette::Base, QColor (184, 230, 249));
      this->setPalette (palette);
    QFont font ("Courier");
      font.setPointSize (14);
      this->setFont (font);
		this->setTextColor (QColor(0, 0, 0));
		//this->setFontWeight(QFont::DemiBold);
  }

  void print (QString string)
  {
    append (string);
  }

  void line ()
  {
#ifdef Q_WS_MAC
    print ("<br>");
#else
    print ("<hr><br>");
#endif
  }
};

#endif
