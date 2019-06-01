/*****************************************************************************
** About.cpp - Source file of About DialogBox containing info about program
** Author: Kraku
*****************************************************************************/
#include "About.h"
#include "about.xpm"
#include "const.h"

About::About (QWidget * parent):QDialog (parent)
{

  all = new QVBoxLayout (this);

  this->setWindowTitle (tr ("About"));

  QPixmap
  logo (about_xpm);
  image = new QLabel (this);
  image->setFixedSize (501, 218);
  image->setPixmap (logo);

  all->addWidget (image);
  box = new QGroupBox (tr ("GB Cart Flasher Project"), this);
  box->setFixedWidth (501);
  labels = new QVBoxLayout (box);
  name =
    new QLabel (tr ("GB Cart Flasher ") +
								VER + " for OSX\nTested on Snow Leopard (64=bit)", box);
	labels->addWidget (name);
	bazz = new QLabel ("Updated by <a href=http://www.youtube.com/imyourbazz>Bazz</a> - FTD2XX Drivers", box);
  bazz->setOpenExternalLinks(TRUE);
	labels->addWidget(bazz);
	
  copy = new QLabel ("\n\nCopyright (c) 2005-2007 Kraku & Chroost", box);
  labels->addWidget (copy);
  email =
    new
    QLabel (tr
	    ("e-mail: <a href=mailto:gbflasher@interia.pl>gbflasher@interia.pl</a>"),
	    box);
	email->setOpenExternalLinks(TRUE);
  labels->addWidget (email);
  translator = new QLabel(tr("Translation by: Chroost"),box);
  labels->addWidget(translator);
  desc1 = new QLabel (tr ("This program and device, called set, enables to:\n"
			  "- make game backups from owned cartridges and play them on PC,\n"
			  "- transfer game saves between cartridge and PC, save and modify them,\n"
			  "- test written by yourself games directly on console\n  (rewritable cartridge required)."),
		      box);
  labels->addWidget (desc1);
  warning = new QLabel (tr ("<b>WARNING!</b>"), box);
  labels->addWidget (warning);
  desc2 =
    new
    QLabel (tr("Using this set for purposes other than previously listed,\n"
	    "especially for making illegal copies of copyrighted games is\nstrictly prohibited!"), box);
  labels->addWidget (desc2);
  all->addWidget (box);
  ok_btn = new QPushButton (tr ("Close"), this);
  all->addWidget (ok_btn);
  connect (ok_btn, SIGNAL (clicked ()), this, SLOT (close ()));
}
