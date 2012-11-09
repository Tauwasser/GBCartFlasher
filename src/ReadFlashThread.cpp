/*****************************************************************************
** ReadFlashThread.cpp
** Author: Kraku
*****************************************************************************/
#include "ReadFlashThread.h"
#include "Settings.h"
#include "Logic.h"
#include "const.h"


void
ReadFlashThread::run ()
{
  end = FALSE;
  unsigned char packet[72], data[16384];
  int character, data_type = 0x00, recived = 0, retries =
    0, page_number, packet_number, prv_count = 0;
  config_t cfg;
  cfg.operation = RROM;
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
    {				/* send start packet and wait for first one of response */
      if (Logic::send_start_packet (port, cfg) == FALSE)
	{
	  port->close_port ();
	  fclose (file);
	  emit error (SEND_ERROR);
	  return;
	}

      character = port->receive_packet (packet);

      if (character == END || character == TIMEOUT)
	{
	  port->close_port ();
	  fclose (file);
	  emit error (character);
	  return;
	}

      if (character == DATA)	/* if packet received - check it */
	data_type = Logic::check_packet (packet);
    }
  while (data_type != NORMAL_DATA && ++retries < 10);

  if (retries == 10)
    {
      port->close_port ();
      fclose (file);
      emit error (TIMEOUT);
      return;
    }

  retries = 0;
  /* work until LAST_DATA */
  do
    {
      if (recived != 0)		/* if first packet, don't need to get it */
	character = port->receive_packet (packet);

      if (character == TIMEOUT || character == END)
	{
	  port->close_port ();
	  fclose (file);
	  emit error (character);
	  return;
	}

      //end of thread!!!!
      if (end)
	{
	  port->send_char (END);
	  port->close_port ();
	  fclose (file);
	  emit error (END);
	  return;
	}

      data_type = Logic::check_packet (packet);

      if (data_type == NORMAL_DATA || data_type == LAST_DATA)
	{			/* valid data packet */
	  page_number = recived / 256;	/* 256 packets = page */
	  packet_number = recived % 256;
	  retries = 0;

	  /* current packet */
	  if (packet_number == packet[3]
	      && page_number == packet[4] * 256 + packet[5])
	    {
	      memcpy (&data[packet_number * 64], &packet[6], FRAMESIZE);
	      if (packet_number == 255)	/* if entire page - save it to file */
		if (fwrite (data, 1, sizeof data, file) < sizeof data)
		  {
		    port->close_port ();
		    fclose (file);
		    emit error (FILEERROR_W);
		    return;
		  }
	      port->send_char (ACK);	/* send confirmation */
	      recived++;
	      prv_count = 0;
	      /* set progress bar status */
	      emit set_progress (recived, page_count * 256 - 1);
	    }
	  /* last packet received- packet is lost */
	  else if (((packet_number - 1) % 255) == packet[3])
	    {
	      /* device is sending last packets - end of transmision */
	      if (++prv_count == 10)
		{
		  port->send_char (END);
		  port->close_port ();
		  fclose (file);
		  emit error (END);
		  return;
		}
	      else
		port->send_char (ACK);
	      /* confirm receivment, force device to send current packet */
	    }
	  else
	    {			/* valid packet, but not current nor last */
	      port->send_char (END);
	      port->close_port ();
	      fclose (file);
	      emit error (END);
	      return;
	    }
	}
      else
	{			/* bad data or NAK */
	  if (++retries < 10)
	    port->send_char (NAK);
	  else
	    {
	      port->send_char (END);
	      port->close_port ();
	      fclose (file);
	      emit error (END);
	      return;
	    }
	}
    }
  while (data_type != LAST_DATA);

  /* if correct data size received - end */
  if (recived * 64 == page_count * 16384)
    {
      port->close_port ();
      fclose (file);
      emit error (TRUE);
    }
  else
    {
      port->close_port ();
      fclose (file);
      emit error (END);
    }
}

void
ReadFlashThread::canceled (void)
{
  end = TRUE;
}
