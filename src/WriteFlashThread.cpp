/*****************************************************************************
** WriteFlashThread.cpp
** Author: Kraku
*****************************************************************************/
#include "WriteFlashThread.h"
#include "Settings.h"
#include "Logic.h"
#include "const.h"


void
WriteFlashThread::run ()
{
  end = FALSE;
  bool zamykanie = FALSE;

  int character, page_number, packet_number, retries = 0;
  unsigned char packet[72], data[16384];	/* 16 kB is one page */
  config_t cfg;
  cfg.operation = WROM;
  cfg.mbc = mbc;
  cfg.algorythm = algorythm;
  cfg.dap = Settings::dap;
  cfg.page_count = page_count;

  if (file == NULL)
    {
      port->close_port ();
      emit error (FILEERROR_O);
      return;
    }

  do
    {				/* send start and wait for ACK */
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
  while (character != ACK && ++retries < 10);	/* repeat 10 times */

  if (retries == 10)
    {				/* wait time passed */
      port->close_port ();
      fclose (file);
      emit error (TIMEOUT);
      return;
    }

  retries = 0;

  /* send rest of data - page after page*/
  for (page_number = 0; page_number < page_count; page_number++)
    {
      packet_number = 0;
      /* read another page of data */
      memset (data, 0xff, sizeof data);
      fread (data, sizeof (char), sizeof (data), file);
      /* first packet on page */
      Logic::fill_data_packet (packet, &data[packet_number * 64], NORMAL_DATA,
			       packet_number, page_number);

      do
	{			/* send packet and wait for ACK */

	  //end of thread!!!!
	  if (end)
	    {
	      Logic::fill_data_packet (packet, &data[packet_number * 64],
				       LAST_DATA, packet_number, page_number);
	      zamykanie = TRUE;

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
	    {			/* ACK of packet recive */
	      if (zamykanie)
		{
		  port->close_port ();
		  fclose (file);
		  emit error (END);
		  return;
		}

	      if (++packet_number == 256)	/* last packet on page */
		break;
	      retries = 0;
	      /* is it last packet of transmision ? */
	      if (packet_number == 255 && page_number == page_count - 1)
		Logic::fill_data_packet (packet, &data[packet_number * 64],
					 LAST_DATA, packet_number,
					 page_number);
	      else		/* nope, it it ordinary packet */
		Logic::fill_data_packet (packet, &data[packet_number * 64],
					 NORMAL_DATA, packet_number,
					 page_number);
	      /* update progress */
	      emit set_progress (page_number * 256 + packet_number,
				 page_count * 256 - 1);
	    }
	  /* NAK - bad packet */
	  else if (++retries == 10)
	    {
	      port->close_port ();
	      fclose (file);
	      emit error (END);
	      return;
	    }
	}
      while (1);		/* until all page is send and all ACK recived */
    }

  port->close_port ();
  fclose (file);
  emit error (TRUE);
}

void
WriteFlashThread::canceled (void)
{
  end = TRUE;
}
