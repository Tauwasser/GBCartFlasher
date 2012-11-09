/*****************************************************************************
** Logic.cpp - Source file containing all utility functions used in 
** communication process by Threads and Gui
** Author: Kraku
*****************************************************************************/
#include "Logic.h"
#include "const.h"
#include "AbstractPort.h"
#include <ctype.h>
#include <cassert>

/* array used to generate crc16 */
unsigned short crc16_tab[256] = {
  0x0000, 0x1021, 0x2042, 0x3063, 0x4084, 0x50A5, 0x60C6, 0x70E7, 0x8108,
  0x9129, 0xA14A, 0xB16B, 0xC18C, 0xD1AD, 0xE1CE, 0xF1EF, 0x1231, 0x0210,
  0x3273, 0x2252, 0x52B5, 0x4294, 0x72F7, 0x62D6, 0x9339, 0x8318, 0xB37B,
  0xA35A, 0xD3BD, 0xC39C, 0xF3FF, 0xE3DE, 0x2462, 0x3443, 0x0420, 0x1401,
  0x64E6, 0x74C7, 0x44A4, 0x5485, 0xA56A, 0xB54B, 0x8528, 0x9509, 0xE5EE,
  0xF5CF, 0xC5AC, 0xD58D, 0x3653, 0x2672, 0x1611, 0x0630, 0x76D7, 0x66F6,
  0x5695, 0x46B4, 0xB75B, 0xA77A, 0x9719, 0x8738, 0xF7DF, 0xE7FE, 0xD79D,
  0xC7BC, 0x48C4, 0x58E5, 0x6886, 0x78A7, 0x0840, 0x1861, 0x2802, 0x3823,
  0xC9CC, 0xD9ED, 0xE98E, 0xF9AF, 0x8948, 0x9969, 0xA90A, 0xB92B, 0x5AF5,
  0x4AD4, 0x7AB7, 0x6A96, 0x1A71, 0x0A50, 0x3A33, 0x2A12, 0xDBFD, 0xCBDC,
  0xFBBF, 0xEB9E, 0x9B79, 0x8B58, 0xBB3B, 0xAB1A, 0x6CA6, 0x7C87, 0x4CE4,
  0x5CC5, 0x2C22, 0x3C03, 0x0C60, 0x1C41, 0xEDAE, 0xFD8F, 0xCDEC, 0xDDCD,
  0xAD2A, 0xBD0B, 0x8D68, 0x9D49, 0x7E97, 0x6EB6, 0x5ED5, 0x4EF4, 0x3E13,
  0x2E32, 0x1E51, 0x0E70, 0xFF9F, 0xEFBE, 0xDFDD, 0xCFFC, 0xBF1B, 0xAF3A,
  0x9F59, 0x8F78, 0x9188, 0x81A9, 0xB1CA, 0xA1EB, 0xD10C, 0xC12D, 0xF14E,
  0xE16F, 0x1080, 0x00A1, 0x30C2, 0x20E3, 0x5004, 0x4025, 0x7046, 0x6067,
  0x83B9, 0x9398, 0xA3FB, 0xB3DA, 0xC33D, 0xD31C, 0xE37F, 0xF35E, 0x02B1,
  0x1290, 0x22F3, 0x32D2, 0x4235, 0x5214, 0x6277, 0x7256, 0xB5EA, 0xA5CB,
  0x95A8, 0x8589, 0xF56E, 0xE54F, 0xD52C, 0xC50D, 0x34E2, 0x24C3, 0x14A0,
  0x0481, 0x7466, 0x6447, 0x5424, 0x4405, 0xA7DB, 0xB7FA, 0x8799, 0x97B8,
  0xE75F, 0xF77E, 0xC71D, 0xD73C, 0x26D3, 0x36F2, 0x0691, 0x16B0, 0x6657,
  0x7676, 0x4615, 0x5634, 0xD94C, 0xC96D, 0xF90E, 0xE92F, 0x99C8, 0x89E9,
  0xB98A, 0xA9AB, 0x5844, 0x4865, 0x7806, 0x6827, 0x18C0, 0x08E1, 0x3882,
  0x28A3, 0xCB7D, 0xDB5C, 0xEB3F, 0xFB1E, 0x8BF9, 0x9BD8, 0xABBB, 0xBB9A,
  0x4A75, 0x5A54, 0x6A37, 0x7A16, 0x0AF1, 0x1AD0, 0x2AB3, 0x3A92, 0xFD2E,
  0xED0F, 0xDD6C, 0xCD4D, 0xBDAA, 0xAD8B, 0x9DE8, 0x8DC9, 0x7C26, 0x6C07,
  0x5C64, 0x4C45, 0x3CA2, 0x2C83, 0x1CE0, 0x0CC1, 0xEF1F, 0xFF3E, 0xCF5D,
  0xDF7C, 0xAF9B, 0xBFBA, 0x8FD9, 0x9FF8, 0x6E17, 0x7E36, 0x4E55, 0x5E74,
  0x2E93, 0x3EB2, 0x0ED1, 0x1EF0
};

/* array of producers names and codes */
desc_t producers[] = {
  {0x01, "AMD"}, {0x02, "AMI"}, {0xe5, "Analog Devices"},
  {0x1f, "Atmel"}, {0x31, "Catalyst"}, {0x34, "Cypress"},
  {0x04, "Fujitsu"}, {0xE0, "Goldstar"}, {0x07, "Hitachi"},
  {0xad, "Hyundai"}, {0xc1, "Infineon"}, {0x89, "Intel"},
  {0xd5, "Intg. Silicon Systems"}, {0xc2, "Macronix"}, {0x29, "Microchip"},
  {0x2c, "Micron"}, {0x1c, "Mitsubishi"}, {0x10, "Nec"},
  {0x15, "Philips Semiconductors"}, {0xce, "Samsung"}, {0x62, "Sanyo"},
  {0x20, "SGS Thomson"}, {0xb0, "Sharp"}, {0xbf, "SST"},
  {0x97, "Texas Instruments"}, {0x98, "Toshiba"}, {0xda, "Winbond"},
  {0x19, "Xicor"}, {0xc9, "Xilinx"}
};

/* array of cart types - source GB CPU Manual */
desc_t carts[] = {
  {0x00, "ROM ONLY"}, {0x01, "ROM+MBC1"},
  {0x02, "ROM+MBC1+RAM"}, {0x03, "ROM+MBC1+RAM+BATT"},
  {0x05, "ROM+MBC2"}, {0x06, "ROM+MBC2+BATTERY"},
  {0x08, "ROM+RAM"}, {0x09, "ROM+RAM+BATTERY"},
  {0x11, "ROM+MBC3"},
  {0x0b, "ROM+MMMO1"}, {0x0c, "ROM+MMMO1+SRAM"},
  {0x0d, "ROM+MMMO1+SRAM+BATT"}, {0x0f, "ROM+MBC3+TIMER+BATT"},
  {0x10, "ROM+MBC3+TIMER+RAM+BAT"}, {0x12, "ROM+MBC3+RAM"},
  {0x13, "ROM+MBC3+RAM+BATT"}, {0x19, "ROM+MBC5"},
  {0x1a, "ROM+MBC5+RAM"}, {0x1b, "ROM+MBC5+RAM+BATT"},
  {0x1c, "ROM+MBC5+RUMBLE"}, {0x1d, "ROM+MBC5+RUMBLE+SRAM"},
  {0x1e, "ROM+MBC5+RUMBLE+SRAM+BATT"}, {0x1f, "Pocket Camera"},
  {0xfd, "Bandai TAMA5"}, {0xfe, "Hudson HuC-3"}
};

desc_t rom_sizes[] = {
  {0x00, "32KB"}, {0x01, "64KB"}, {0x02, "128KB"}, {0x03, "256KB"},
  {0x04, "512KB"}, {0x05, "1MB"}, {0x06, "2MB"}, {0x52, "1.1MB"},
  {0x53, "1.2MB"}, {0x54, "1.5MB"}
};

desc_t ram_sizes[] = {
  {0x00, "0KB"}, {0x01, "2KB"}, {0x02, "8KB"}, {0x03, "32KB"}, {0x04, "128KB"}
};

Logic::Logic (QObject * parent):QObject (parent)
{

  //version with threads
  //TODO: move it

}


int
Logic::read_status (AbstractPort * port, const char *port_name, char id, char mbc,
		    char algorythm, status_t * flasher_stat)
{
  assert (port != NULL);
  if (port->open_port (port_name) == FALSE)
    return PORT_ERROR;

  unsigned short crc16;
  unsigned char packet[PACKETSIZE];
  memset (packet, 0x00, PACKETSIZE);
  packet[0] = DATA;
  packet[1] = STATUS;
  packet[2] = id;
  packet[3] = mbc;
  packet[4] = algorythm;
  crc16 = generate_crc16 (packet);
  packet[PACKETSIZE - 2] = crc16 / 256;
  packet[PACKETSIZE - 1] = crc16 % 256;

  if (port->send_packet (packet) < PACKETSIZE)
    {
      port->close_port ();
      return SEND_ERROR;
    }
  else
    {
      if (port->receive_packet (packet) == DATA)
	{
	  int i, producers_count = sizeof producers / sizeof producers[0];
	  int types_count = sizeof carts / sizeof carts[0];
	  int types_count_rom = sizeof rom_sizes / sizeof rom_sizes[0];
	  int types_count_ram = sizeof ram_sizes / sizeof ram_sizes[0];

	  /* filling struct with data from packet */
	  flasher_stat->ver_11 = packet[2] / 16;
	  flasher_stat->ver_12 = packet[2] % 16;
	  flasher_stat->ver_21 = packet[3] / 16;
	  flasher_stat->ver_22 = packet[3] % 16;
	  flasher_stat->manufacturer_id = packet[4];
	  /* if code if not found - it remains unknown */
	  strcpy (flasher_stat->manufacturer,
		  tr ("Unknown manufacturer").toLocal8Bit ());
	  for (i = 0; i < producers_count; i++)
	    if (flasher_stat->manufacturer_id == producers[i].index)
	      {
		memset (flasher_stat->manufacturer, 0x00, 30);
		strcpy (flasher_stat->manufacturer, producers[i].name);
	      }
	  flasher_stat->chip_id = packet[5];
	  flasher_stat->bbl = packet[6] & 0x01;

	  flasher_stat->logo_correct = packet[8];
	  memcpy (flasher_stat->game_name, &packet[9], 16);
	  for (int i = 0; i < 16; i++)
	    if (!isalnum (flasher_stat->game_name[i])
		|| flasher_stat->game_name[i] == '\n')
	      flasher_stat->game_name[i] = ' ';
	  flasher_stat->game_name[16] = '\0';
	  flasher_stat->cgb = (packet[24] == 0x80) ? 1 : 0;
	  flasher_stat->sgb = (packet[27] == 0x03) ? 1 : 0;
	  strcpy (flasher_stat->typ, tr ("Unknown type").toLocal8Bit ());
	  for (i = 0; i < types_count; i++)
	    if (packet[28] == carts[i].index)
	      {
		memset (flasher_stat->typ, 0x00, 30);
		strcpy (flasher_stat->typ, carts[i].name);
	      }

	  for (i = 0; i < types_count_rom; i++)
	    if (packet[29] == rom_sizes[i].index)
	      {
		memset (flasher_stat->rom_size, 0x00,
			sizeof flasher_stat->rom_size);
		strcpy (flasher_stat->rom_size, rom_sizes[i].name);
	      }

	  for (i = 0; i < types_count_ram; i++)
	    if (packet[30] == ram_sizes[i].index)
	      {
		memset (flasher_stat->ram_size, 0x00,
			sizeof flasher_stat->ram_size);
		strcpy (flasher_stat->ram_size, ram_sizes[i].name);
	      }
	  flasher_stat->crc16 = 256 * packet[35] + packet[36];

	  port->close_port ();
	  return TRUE;
	}
      else
	{
	  port->close_port ();
	  return FALSE;
	}
    }
}

unsigned short
Logic::generate_crc16 (unsigned char packet[PACKETSIZE])
{
  int i;
  unsigned short crc16 = 0x0000;
  /* generate for whole packet, except for two last bytes */
  for (i = 0; i < PACKETSIZE - 2; i++)
    crc16 = (crc16 << 8) ^ crc16_tab[((crc16 >> 8) ^ packet[i]) & 0xFF];
  return crc16;
}

void
Logic::fill_data_packet (unsigned char packet[PACKETSIZE],
			 unsigned char data[FRAMESIZE], char packet_type,
			 unsigned char packet_index, int page_index)
{
  unsigned short crc16;
  memset (packet, 0x00, PACKETSIZE);
  packet[0] = DATA;
  packet[1] = packet_type;
  packet[2] = RESERVED;
  packet[3] = packet_index;
  packet[4] = page_index / 256;
  packet[5] = page_index % 256;
  /* copy data to packet */
  memcpy (&packet[6], data, FRAMESIZE);

  /* generate and set crc */
  crc16 = generate_crc16 (packet);
  packet[PACKETSIZE - 2] = crc16 / 256;
  packet[PACKETSIZE - 1] = crc16 % 256;
}


int
Logic::check_packet (unsigned char packet[PACKETSIZE])
{
  unsigned short crc16 = generate_crc16 (packet);
  /* data packet with correct crc */
  if (packet[0] == DATA && packet[PACKETSIZE - 2] == crc16 / 256
      && packet[PACKETSIZE - 1] == crc16 % 256)
    return packet[1];		/* return packet type */
  else
    return BAD_PACKET;
}

long
Logic::file_size (FILE * file)
{
  long size = 0;
  fseek (file, 0x00, SEEK_END);
  size = ftell (file);
  fseek (file, 0x00, SEEK_SET);
  return size;
}

short
Logic::flash_file_size (FILE * file)
{
  long mem_size = Logic::file_size (file);
  /* file_size w kB */
  switch (mem_size)
    {
    case 32 * 1024:
    case 64 * 1024:
    case 128 * 1024:
    case 256 * 1024:
    case 512 * 1024:
    case 1024 * 1024:
    case 2048 * 1024:
    case 4096 * 1024:
    case 8192 * 1024:
      return (short) (mem_size / 1024L);
    default:
      return FALSE;
    }
}

short
Logic::ram_file_size (FILE * file)
{
  long mem_size = Logic::file_size (file);
  switch (mem_size)
    {
    case 2 * 1024:
    case 8 * 1024:
    case 32 * 1024:
    case 128 * 1024:
      return (short) (mem_size / 1024L);
    default:
      return FALSE;
    }
}

bool
Logic::send_start_packet (AbstractPort * port, config_t cfg)
{
  unsigned short crc16;
  unsigned char packet[PACKETSIZE];
  memset (packet, 0x00, PACKETSIZE);
  packet[0] = DATA;
  packet[1] = CONFIG;
  packet[2] = cfg.operation;
  packet[3] = cfg.mbc;
  packet[4] = cfg.algorythm;
  packet[5] = cfg.dap;
  packet[6] = (cfg.page_count - 1) / 256;
  packet[7] = (cfg.page_count - 1) % 256;
  crc16 = Logic::generate_crc16 (packet);
  packet[PACKETSIZE - 2] = crc16 / 256;
  packet[PACKETSIZE - 1] = crc16 % 256;
  if (port->send_packet (packet) < PACKETSIZE)
    return FALSE;
  else
    return TRUE;
}

QString
Logic::get_path (QString name)
{
  int end;

  if ((end = name.lastIndexOf ("/")) != -1)
    {
      return name.left (end);
    }
  else
    return QString (".");
}
