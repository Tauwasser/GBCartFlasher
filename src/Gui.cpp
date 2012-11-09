/*****************************************************************************
** Gui.cpp - main class of GUI (Main Window)
** Author: Kraku
*****************************************************************************/
#include <QFileDialog>
#include <QPixmap>
#include <QMessageBox>
#include <QThread>
#include "Gui.h"
#include "Settings.h"
#include "Logic.h"
#include "About.h"



#ifdef Q_WS_X11
#include "SerialPort.h"
#include "USBPort.h"
#endif

#ifdef Q_WS_WIN
#include "USBPortWin.h"
#include "SerialPortWin.h"
#endif

#include "const.h"
#include "flasher.xpm"
#include "icon.xpm"

int
  Gui::port_type = SERIAL;

Gui::Gui (QWidget * parent):QWidget (parent)
{
  QThread::currentThread ()->setPriority (QThread::NormalPriority);
  path = ".";			//current startup dir
  this->setWindowIcon (QIcon (QPixmap (icon)));
  this->setWindowTitle (tr ("GB Cart Flasher version ") + VER);
  grid = new QGridLayout (this);
  left = new QVBoxLayout ();
  right = new QVBoxLayout ();
  center = new QVBoxLayout ();
  down = new QHBoxLayout ();

  image = new QLabel (this);

  image->setFixedSize (200, 162);
  settings = new Settings (this);
  left->addWidget (settings);
  left->addWidget (image);
  left->addStretch (1);
  grid->addLayout (left, 0, 0);
  console = new Console (this);

  QPixmap Logo (gameboy);

  image->setPixmap (Logo);

  right->addWidget (console);
  progress = new QProgressBar (this);
  down->addWidget (progress);
  cancel_btn = new QPushButton (tr ("Cancel"), this);
  cancel_btn->setEnabled (FALSE);
  down->addWidget (cancel_btn);
  right->addLayout (down);
  grid->addLayout (right, 0, 2);
  status_btn = new QPushButton (tr ("Cart Info"), this);
  rflash_btn = new QPushButton (tr ("Read FLASH"), this);
  wflash_btn = new QPushButton (tr ("Write FLASH"), this);
  rram_btn = new QPushButton (tr ("Read RAM"), this);
  wram_btn = new QPushButton (tr ("Write RAM"), this);
  eflash_btn = new QPushButton (tr ("Erase FLASH"), this);
  eram_btn = new QPushButton (tr ("Erase RAM"), this);
  about_btn = new QPushButton (tr ("About"), this);

  center->addWidget (status_btn, Qt::AlignTop);
  center->addWidget (rflash_btn);
  center->addWidget (wflash_btn);
  center->addWidget (rram_btn);
  center->addWidget (wram_btn);
  center->addWidget (eflash_btn);
  center->addWidget (eram_btn);
  center->addSpacing (20);
  center->addWidget (about_btn);
  center->addStretch (1);
  grid->addLayout (center, 0, 1);

  thread_WFLA = new WriteFlashThread;
  thread_RFLA = new ReadFlashThread;
  thread_E = new EraseThread;
  thread_RRAM = new ReadRamThread;
  thread_WRAM = new WriteRamThread;



  connect (cancel_btn, SIGNAL (clicked ()), thread_RFLA, SLOT (canceled ()));
  connect (cancel_btn, SIGNAL (clicked ()), thread_WFLA, SLOT (canceled ()));
  connect (cancel_btn, SIGNAL (clicked ()), thread_RRAM, SLOT (canceled ()));
  connect (cancel_btn, SIGNAL (clicked ()), thread_WRAM, SLOT (canceled ()));

  connect (cancel_btn, SIGNAL (clicked ()), thread_E, SLOT (canceled ()));
  connect (wflash_btn, SIGNAL (clicked ()), this, SLOT (write_flash ()));
  connect (rflash_btn, SIGNAL (clicked ()), this, SLOT (read_flash ()));
  connect (status_btn, SIGNAL (clicked ()), this, SLOT (show_info ()));
  connect (eflash_btn, SIGNAL (clicked ()), this, SLOT (erase_flash ()));
  connect (rram_btn, SIGNAL (clicked ()), this, SLOT (read_ram ()));
  connect (wram_btn, SIGNAL (clicked ()), this, SLOT (write_ram ()));
  connect (eram_btn, SIGNAL (clicked ()), this, SLOT (erase_ram ()));
  connect (about_btn, SIGNAL (clicked ()), this, SLOT (about ()));

  connect (thread_WFLA, SIGNAL (set_progress (int, int)), this,
	   SLOT (setProgress (int, int)));
  connect (thread_RFLA, SIGNAL (set_progress (int, int)), this,
	   SLOT (setProgress (int, int)));
  connect (thread_E, SIGNAL (set_progress (int, int)), this,
	   SLOT (setProgress (int, int)));
  connect (thread_RRAM, SIGNAL (set_progress (int, int)), this,
	   SLOT (setProgress (int, int)));
  connect (thread_WRAM, SIGNAL (set_progress (int, int)), this,
	   SLOT (setProgress (int, int)));


  connect (thread_RFLA, SIGNAL (error (int)), this, SLOT (print_error (int)));
  connect (thread_WFLA, SIGNAL (error (int)), this, SLOT (print_error (int)));
  connect (thread_RRAM, SIGNAL (error (int)), this, SLOT (print_error (int)));
  connect (thread_WRAM, SIGNAL (error (int)), this, SLOT (print_error (int)));
  connect (thread_E, SIGNAL (error (int)), this, SLOT (print_error (int)));

  connect (settings, SIGNAL (refresh_ram_buttons (void)), this,
	   SLOT (setRamButtons (void)));
  setProgress (0, 1);
  console->print (tr ("GB Cart Flasher version ") + VER + tr (" started."));
#ifdef Q_WS_WIN
/* device detection is avilable only on Windows */
  if (Settings::commanual == FALSE)
    {
      console->print (tr ("Detecting device..."));
    }
#endif
}

AbstractPort *
Gui::create_port (void)
{
  switch (port_type)
    {
    case USB:
#ifdef Q_WS_WIN
      return new USBPortWin;
#endif
#ifdef Q_WS_X11
      return new USBPort;
#endif
    case SERIAL:
#ifdef Q_WS_WIN
      return new SerialPortWin;
#endif
#ifdef Q_WS_X11
      return new SerialPort;
#endif
      break;
    }
  return NULL;
}

void
Gui::startup_info (void)
{
  status_t status;
  int which_port = -1;		//none at beggining


  if (Settings::commanual == FALSE)
    {
      port_type = USB;
      AbstractPort *port = create_port ();
      if (Logic::read_status (port, "USB", NREAD_ID, 0x00, 0x00, &status) ==
	  TRUE)
	{
	  QString tmp;
	  console->print (tr ("Device connected to: USB"));
	  settings->setCom (4);	//4 is index of usb in combobox
	  tmp =
	    tmp.sprintf (" %d%d.%d%d", status.ver_11, status.ver_12,
			 status.ver_21, status.ver_22);
	  console->print (tr ("Device firmware version:") + tmp);
	  console->line ();
	  return;
	}

      port_type = SERIAL;
      port = create_port ();
      for (int i = 0; i < PORTS_COUNT; i++)
	{
	  if (Logic::
	      read_status (port, settings->getCom(i).toAscii(), NREAD_ID, 0x00, 0x00,
			   &status) == TRUE)
	    {
	      which_port = i;
	      break;
	    }
	}
      if (which_port == -1)
	{
	  console->print (tr ("Device not found!"));
	  console->print (tr ("Check COM port connection."));
	}
      else
	{
	  QString tmp;
	  console->print (tr ("Device connected to: ") +
			  settings->getCom (which_port));
	  settings->setCom (which_port);
	  tmp =
	    tmp.sprintf (" %d%d.%d%d", status.ver_11, status.ver_12,
			 status.ver_21, status.ver_22);
	  console->print (tr ("Device firmware version:") + tmp);
	}
    }
  console->line ();

}

void
Gui::show_info ()
{
  status_t status;
  QString tmp;
  AbstractPort *port = create_port ();
  int return_code = Logic::read_status (port, settings->getCom().toAscii(), READ_ID,
					settings->getMbc (),
					Settings::algorythm, &status);

  if (return_code == TRUE)	/* no error */
    {

      console->print (tr ("--Device information--"));
      tmp =
	tmp.sprintf (" %d%d.%d%d", status.ver_11, status.ver_12,
		     status.ver_21, status.ver_22);
      console->print (tr ("Device firmware version:") + tmp);
      console->print ("\n" + tr ("--Cartridge information--"));
      tmp = tmp.sprintf (" 0x%x", status.manufacturer_id);
      console->print (tr ("FLASH memory manufacturer ID:") + tmp);
      tmp = tmp.sprintf (" %s", status.manufacturer);
      console->print (tr ("FLASH memory manufacturer name:") + tmp);
      tmp = tmp.sprintf (" 0x%x", status.chip_id);
      console->print (tr ("FLASH memory chip ID:") + tmp);

      if (Settings::showbbl == TRUE)
	{
	  if (status.bbl == 1)
	    tmp = tr ("Locked!");
	  else
	    tmp = tr ("Unlocked");
	  console->print (tr ("Boot Block Status: ") + tmp);
	}

      if (status.logo_correct == 1)
	{
	  console->print ("\n" + tr ("--ROM/FLASH content information--"));
	  console->print (tr ("Game logo signature is correct."));
	  tmp = tmp.sprintf (" %s", status.game_name);
	  console->print (tr ("Game title:") + tmp);
	  if (status.cgb == 1)
	    tmp = tr ("YES");
	  else
	    tmp = tr ("NO");
	  console->print (tr ("Designed for Color GB: ") + tmp);
	  if (status.sgb == 1)
	    tmp = tr ("YES");
	  else
	    tmp = tr ("NO");
	  console->print (tr ("Designed for Super GB: ") + tmp);
	  tmp = tmp.sprintf (" %s", status.typ);
	  console->print (tr ("Cartridge type:") + tmp);
	  tmp = tmp.sprintf (" %s", status.rom_size);
	  console->print (tr ("ROM size:") + tmp);
	  tmp = tmp.sprintf (" %s", status.ram_size);
	  console->print (tr ("RAM size:") + tmp);
	  tmp = tmp.sprintf (" 0x%x", status.crc16);
	  console->print (tr ("Checksum:") + tmp);
	  console->line ();
	}
      else
	{
	  console->print (tr ("Game logo signature is incorrect."));
	  console->
	    print (tr ("Cartridge is blank, damaged or not connected."));
	  console->line ();
	}
    }
  else
    print_error (return_code);

}


void
Gui::read_flash (void)
{
  file_name =
    QFileDialog::getSaveFileName (this, tr ("Write FLASH to..."), path,
				  tr ("GB Rom Dumps (*.gb *.gbc *.sgb)"));
  path = Logic::get_path (file_name);
  if (file_name != "")
    {
      thread_RFLA->port = create_port ();
      if (thread_RFLA->port->open_port (settings->getCom().toAscii()) == FALSE)
	{
	  print_error (PORT_ERROR);
	  return;
	}
      if (!file_name.contains (".gb", Qt::CaseInsensitive)
	  && !file_name.contains (".gbc", Qt::CaseInsensitive)
	  && !file_name.contains (".sgb", Qt::CaseInsensitive))
	file_name = file_name + ".gb";

      thread_RFLA->file = fopen (file_name.toAscii (), "wb");
      thread_RFLA->mbc = settings->getMbc ();
      thread_RFLA->page_count = settings->getFlash () / 16;
      thread_RFLA->dap = Settings::dap;
      thread_RFLA->algorythm = Settings::algorythm;

      setEnabledButtons (FALSE);
      thread_RFLA->start (Settings::priority);

      console->print (tr ("Reading data from FLASH to file:") + "\n" +
		      file_name);
    }
}

void
Gui::write_flash (void)
{

  file_name =
    QFileDialog::getOpenFileName (this, tr ("Read FLASH from..."), path,
				  tr ("GB Rom Dumps (*.gb *.gbc *.sgb)"));
  path = Logic::get_path (file_name);
  if (file_name != "")
    {
      long bytes_count;
      short kilobytes_count;
      thread_WFLA->port = create_port ();
      if (thread_WFLA->port->open_port (settings->getCom().toAscii()) == FALSE)
	{
	  print_error (PORT_ERROR);
	  return;
	}
      thread_WFLA->file = fopen (file_name.toAscii (), "rb");
      thread_WFLA->mbc = settings->getMbc ();
      thread_WFLA->algorythm = Settings::algorythm;
      thread_WFLA->dap = Settings::dap;

      if (settings->isAuto () == FALSE)
	{
	  bytes_count = Logic::file_size (thread_WFLA->file);
	  thread_WFLA->page_count =
	    (short) ((bytes_count % 16384L) ? (bytes_count / 16384 +
					       1) : (bytes_count / 16384L));
	  kilobytes_count =
	    (short) ((bytes_count % 1024L) ? (bytes_count / 1024 +
					      1) : (bytes_count / 1024L));

	}
      else if ((kilobytes_count = Logic::flash_file_size (thread_WFLA->file))
	       != FALSE)
	thread_WFLA->page_count = kilobytes_count / 16;
      else
	{
	  print_error (WRONG_SIZE);
	  thread_WFLA->port->close_port ();
	  return;
	}
      setEnabledButtons (FALSE);

      thread_WFLA->start (Settings::priority);
      console->print (tr ("Writing data to FLASH from file:") + "\n" +
		      file_name);
      console->print (tr ("File size: ") + QString::number (kilobytes_count) +
		      "KB");
    }
}


void
Gui::read_ram (void)
{


  file_name =
    QFileDialog::getSaveFileName (this, tr ("Write RAM to..."), path,
				  tr ("GB Save (*.sav)"));
  path = Logic::get_path (file_name);
  if (file_name != "")
    {
      thread_RRAM->port = create_port ();
      if (thread_RRAM->port->open_port (settings->getCom().toAscii()) == FALSE)
	{
	  print_error (PORT_ERROR);
	  return;
	}
      if (!file_name.contains (".sav", Qt::CaseInsensitive))
	file_name = file_name + ".sav";
      thread_RRAM->file = fopen (file_name.toAscii (), "wb");
      thread_RRAM->mbc = settings->getMbc ();
      thread_RRAM->algorythm = Settings::algorythm;
      thread_RRAM->dap = Settings::dap;
      if (settings->getRam () == 2)
	{
	  thread_RRAM->_2k = 1;
	  thread_RRAM->page_count = 1;
	}
      else
	{
	  thread_RRAM->_2k = 0;
	  thread_RRAM->page_count = settings->getRam () / 8;
	}
      setEnabledButtons (FALSE);
      thread_RRAM->start (Settings::priority);

      console->print (tr ("Reading data from RAM to file:") + "\n" +
		      file_name);
    }
}

void
Gui::write_ram (void)
{

  file_name =
    QFileDialog::getOpenFileName (this, tr ("Read RAM from..."), path,
				  tr ("GB Save (*.sav)"));
  path = Logic::get_path (file_name);
  if (file_name != "")
    {
      long bytes_count;
      short kilobytes_count;
      thread_WRAM->port = create_port ();;
      if (thread_WRAM->port->open_port (settings->getCom().toAscii()) == FALSE)
	{
	  print_error (PORT_ERROR);
	  return;
	}
      thread_WRAM->file = fopen (file_name.toAscii (), "rb");
      thread_WRAM->mbc = settings->getMbc ();
      thread_WRAM->algorythm = Settings::algorythm;
      thread_WRAM->dap = Settings::dap;

      if (settings->isAuto () == FALSE)
	{
	  bytes_count = Logic::file_size (thread_WRAM->file);
	  if (bytes_count == 2048)
	    {
	      thread_WRAM->_2k = 1;
	      thread_WRAM->page_count = 1;
	      kilobytes_count = 2;
	    }
	  else
	    {
	      thread_WRAM->_2k = 0;
	      thread_WRAM->page_count =
		(short) ((bytes_count % 8192L) ? (bytes_count / 8192L +
						  1) : bytes_count / 8192L);
	      kilobytes_count =
		(short) ((bytes_count % 1024L) ? (bytes_count / 1024 +
						  1) : (bytes_count / 1024L));
	    }
	}
      else if ((kilobytes_count = Logic::ram_file_size (thread_WRAM->file)) !=
	       FALSE)
	if (kilobytes_count == 2)
	  {
	    thread_WRAM->_2k = 1;
	    thread_WRAM->page_count = 1;
	  }
	else
	  {
	    thread_WRAM->_2k = 0;
	    thread_WRAM->page_count = kilobytes_count / 8;
	  }
      else
	{
	  print_error (WRONG_SIZE);
	  thread_WRAM->port->close_port ();
	  return;
	}
      setEnabledButtons (FALSE);

      thread_WRAM->start (Settings::priority);
      console->print (tr ("Writing data to RAM from file:") + "\n" +
		      file_name);
      console->print (tr ("File size: ") + QString::number (kilobytes_count) +
		      "KB");
    }
}



void
Gui::erase_flash (void)
{
  thread_E->port = create_port ();
  if (thread_E->port->open_port (settings->getCom().toAscii()) == FALSE)
    {
      print_error (PORT_ERROR);
      return;
    }
  thread_E->mbc = settings->getMbc ();
  thread_E->mem = EFLA;		//FLASH
  thread_E->par = Settings::algorythm;
  thread_E->dap = Settings::dap;
  setEnabledButtons (FALSE);
  console->print (tr ("Erasing FLASH memory..."));
  thread_E->start (Settings::priority);
}

void
Gui::erase_ram (void)
{
  thread_E->port = create_port ();
  if (thread_E->port->open_port (settings->getCom().toAscii()) == FALSE)
    {
      print_error (PORT_ERROR);
      return;
    }
  int window_count;
  thread_E->mbc = settings->getMbc ();
  thread_E->mem = ERAM;
  switch (settings->getRam ())
    {
    case 2:
    case 8:
      window_count = 0;
      break;
    case 32:
      window_count = 3;
      break;
    case 128:
      window_count = 15;
      break;
    default:
      window_count = 15;
    }

  thread_E->par = window_count;
  thread_E->dap = Settings::dap;
  setEnabledButtons (FALSE);
  console->print (tr ("Erasing RAM memory..."));
  thread_E->start (Settings::priority);
}



void
Gui::setProgress (int ile, int max)
{
  progress->setMinimum (0);
  progress->setMaximum (max);
  progress->setValue (ile);
}


void
Gui::setEnabledButtons (bool state)
{
  status_btn->setEnabled (state);
  rflash_btn->setEnabled (state);
  wflash_btn->setEnabled (state);
  eflash_btn->setEnabled (state);
  cancel_btn->setEnabled (!state);
  //this buttons needs to be dissabled 
  //if no ram is avilable
  if (settings->isRamDisabled ())
    state = FALSE;
  rram_btn->setEnabled (state);
  wram_btn->setEnabled (state);
  eram_btn->setEnabled (state);

}

void
Gui::setRamButtons ()
{
  if (status_btn->isEnabled ())
    setEnabledButtons (TRUE);
}

void
Gui::print_error (int err)
{
  switch (err)
    {
    case FILEERROR_O:
      console->print (tr (">Error opening file."));
      break;

    case FILEERROR_W:
      console->print (tr (">File write error."));
      break;

    case FILEERROR_R:
      console->print (tr (">File read error."));
      break;

    case SEND_ERROR:
      console->print (tr (">Error sending data to device."));
      break;

    case TIMEOUT:
      console->print (tr (">Timeout!"));
      break;

    case END:
      console->print (tr (">Canceled."));
      break;

    case PORT_ERROR:
      console->print (tr (">Error opening COM port."));
      break;

    case WRONG_SIZE:
      console->print (tr (">Bad file size."));
      break;

    case FALSE:
      console->print (tr (">Operation failure."));
      break;

    case TRUE:
      console->print (tr (">Success!"));	/* succes is not a error code */
      break;
    }

  console->line ();
  setProgress (0, 1);
  setEnabledButtons (TRUE);
}

void
Gui::about ()
{
  About about (this);
  about.exec ();
}
