/*****************************************************************************
** Settings.h - configuration widget
** Author: Kraku
*****************************************************************************/
#ifndef _SETTINGS_H_
#define _SETTINGS_H_

#include <QGroupBox>
#include <QComboBox>
#include <QLabel>
#include <QLayout>
#include <QString>
#include <QCheckBox>
#include <QThread>
#include "const.h"

class Settings:public QGroupBox
{
  Q_OBJECT mbc_t mbc;
  QString com_name;
  int flash_size;
  int ram_size;
  bool auto_size;
  QComboBox *com_combo;
  QComboBox *mbc_combo;
  QComboBox *flash_combo;
  QComboBox *ram_combo;
  QComboBox *lang_combo;
  QLabel *com_label;
  QLabel *mbc_label;
  QLabel *flash_label;
  QLabel *ram_label;
  QLabel *lang_label;
  QCheckBox *auto_check;
  QVBoxLayout *labels, *combo_boxes;
  QHBoxLayout *north, *down;
  QVBoxLayout *all;



public:
  static bool commanual;
  static alg_t algorythm;
  static dap_t dap;
  static bool showbbl;
  static speed_type speed;
  static QThread::Priority priority;



    Settings (QWidget * parent = 0);

  int getFlash ()
  {
    return flash_size;
  }

  int getRam ()
  {
    return ram_size;
  }

  QString getCom (void)
  {
    return com_name;
  }

  QString getCom (int index)
  {
    return com_combo->itemText (index);
  }

  mbc_t getMbc ()
  {
    if (mbc == MBC5 || mbc == RUMBLE)
      return MBC5;
    else
      return mbc;
  }
  bool isAuto ()
  {
    return auto_size;
  }

  bool isRamDisabled ()
  {

    return (ram_combo->currentText () == "");
  }

  void ram_types (int type);
  void flash_types (int type);
  void get_langs();

  public slots:
  void setCom (int index);
  void setMbc (int mbc_nr);
  void setFlash (int index);
  void setRam (int index);
  void setLang (const QString & lang);
  void setAuto (int state);
signals:
  void refresh_ram_buttons ();

};

#endif
