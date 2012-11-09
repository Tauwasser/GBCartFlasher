/*****************************************************************************
** WriteRamThread.cpp
** Author: Kraku
*****************************************************************************/
#include "WriteRamThread.h"
#include "Settings.h"
#include "Logic.h"
#include "const.h"


void
WriteRamThread::run ()
{
  end = FALSE;
  bool closing = FALSE;
  /* function similar to write_flash */
  int character, page_number, packet_number, retries = 0;
  unsigned char packet[72], data[8192];
  config_t cfg;
  cfg.operation = WRAM;
  cfg.mbc = mbc;
  cfg.algorythm = 0x00;
  cfg.dap = Settings::dap;
  if (_2k)
    page_count = 1;
  cfg.page_count = page_count;


  if (file == NULL)
    {
      port->close_port ();
      emit error (FILEERROR_O);
      return;
    }

  do
    {
      if (Logic::send_start_packet (port, cfg) == FALSE)
	{
	  port->close_port ();
	  fclose (file);
	  emit error (SEND_ERROR);
	  return;
	}
      character = port->receive_char ();
      if (character == END || character == TIMEOUT)
	{
	  port->close_port ();
	  fclose (file);
	  emit error (character);
	  return;
	}
    }
  while (character != ACK && ++retries < 10);

  if (retries == 10)
    {
      port->close_port ();
      emit error (TIMEOUT);
      fclose (file);
      return;
    }
  retries = 0;
  for (page_number = 0; page_number < page_count; page_number++)
    {
      packet_number = 0;
      memset (data, 0x00, sizeof data);
      if (_2k)			/* if set, read only 2kB - some carts */
	fread (data, sizeof (char), 2048, file);
      else
	fread (data, sizeof (char), sizeof (data), file);

      /* send first packet */
      Logic::fill_data_packet (packet, &data[packet_number * 64], NORMAL_DATA,
			       packet_number, page_number);

      do
	{			/* send packet and wait for ack */
	  if (end)
	    {
	      Logic::fill_data_packet (packet, &data[packet_number * 64],
				       LAST_DATA, packet_number, page_number);
	      closing = TRUE;

	    }

	  if (port->send_packet (packet) < PACKETSIZE)
	    {
	      port->close_port ();
	      fclose (file);
	      emit error (SEND_ERROR);
	      return;
	    }
	  character = port->receive_char ();
	  if (character == END || character == TIMEOUT)
	    {
	      port->close_port ();
	      fclose (file);
	      emit error (character);
	      return;
	    }
	  if (character == ACK)
	    {
	      if (closing)
		{
		  port->close_port ();
		  fclose (file);
		  emit error (END);
		  return;
		}
	      if (_2k == 1 && packet_number == 31)
		{
		  port->close_port ();
		  fclose (file);
		  emit error (TRUE);
		  return;	/* end of 2kB page */
		}
	      if (++packet_number == 128)
		break;
	      retries = 0;
	      if ((packet_number == 127 && page_number == page_count - 1)
		  || (_2k && packet_number == 31))
		Logic::fill_data_packet (packet, &data[packet_number * 64],
					 LAST_DATA, packet_number,
					 page_number);
	      else
		Logic::fill_data_packet (packet, &data[packet_number * 64],
					 NORMAL_DATA, packet_number,
					 page_number);
	      emit set_progress (page_number * 128 + packet_number + 1,
				 _2k ? 32 : page_count * 128);
	    }
	  else if (++retries == 10)
	    {
	      port->close_port ();
	      fclose (file);
	      emit error (END);
	      return;
	    }
	}
      while (1);
    }

  port->close_port ();
  fclose (file);
  emit error (TRUE);
  return;
}

void
WriteRamThread::canceled (void)
{
  end = TRUE;
}
