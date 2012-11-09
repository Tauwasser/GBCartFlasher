/*****************************************************************************
** Logic.h - Header file for utility functions used in communication proces
** Author: Kraku
*****************************************************************************/
#ifndef _LOGIC_H_
#define _LOGIC_H_
#include <QObject>
#include "const.h"
#include "AbstractPort.h"

/* 
 * structure consist of key/value pairs for any kind of descriptions
 * most of those codes/descriptions are taken from GB CPU Manual
 */
typedef struct
{
  unsigned char index;
  char name[30];
} desc_t;

/* struct being used to fill packet header */
typedef struct
{
  unsigned char operation;
  unsigned char mbc;
  unsigned char algorythm;
  unsigned char dap;
  int page_count;
} config_t;

/* struct filled by status */
typedef struct
{
/* digits describing version of device soft */
  unsigned char ver_11;
  unsigned char ver_12;
  unsigned char ver_21;
  unsigned char ver_22;
/* flash chip date */
  unsigned char manufacturer_id;
  char manufacturer[30];
  unsigned char chip_id;
  unsigned char bbl;
/* info about loaded game */
  unsigned char logo_correct;
  unsigned char cgb;
  unsigned char sgb;
  char rom_size[6];
  char ram_size[6];
  unsigned short crc16;
  char typ[30];
  char game_name[17];
} status_t;

/* Logic has to inherit from QObject in order to send signals */
class Logic:public QObject
{
Q_OBJECT public:

/* generate crc16 checksum stored in last two bytes of packet */
  static unsigned short generate_crc16 (unsigned char packet[PACKETSIZE]);
/* both function calculate file size, and check if it's correct ROM/RAM size
 * if it's OK it returns file size in kB else return FALSE
 */
  static short flash_file_size (FILE * file);
  static short ram_file_size (FILE * file);
/* additional function returning exact file size in B */
  static long file_size (FILE * file);
/* function extracting path from fully qualified filename */
  static QString get_path (QString name);
/* check packet type and crc
 * return packet type if ok or BAD_PACKET otherwise
 */
  static int check_packet (unsigned char packet[PACKETSIZE]);
/* create data packet from data, and set packet/page number and crc */
  static void fill_data_packet (unsigned char packet[PACKETSIZE],
				unsigned char data[FRAMESIZE],
				char packet_type, unsigned char packet_number,
				int page_number);
    Logic (QObject * parent = 0);
/* query device for status of itself and cart, fills status_t struct */
  static int read_status (AbstractPort * port, const char *name_portu, char id,
			  char mbc, char alg, status_t * flasher_stat);
/* sends start packet generateg from given configuration */
  static bool send_start_packet (AbstractPort * port, config_t cfg);

  signals:
  void set_progress (int procent, int max);
  void refresh (void);
};

#endif
