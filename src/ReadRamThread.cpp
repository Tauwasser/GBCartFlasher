/*****************************************************************************
** ReadRamThread.cpp
** Author: Kraku
*****************************************************************************/
#include "ReadRamThread.h"
#include "Settings.h"
#include "Logic.h"
#include "const.h"


void
ReadRamThread::run ()
{

  end = FALSE;
  unsigned char packet[72], data[2048];	/* 2kB cart is avilable */
  int n, data_type = 0x00, recived = 0, retries =
    0, page_number, packet_number, prv_count = 0;
  config_t cfg;
  cfg.operation = RRAM;
  cfg.mbc = mbc;
  cfg.algorythm = algorythm;
  cfg.dap = Settings::dap;
  cfg.page_count = page_count;
  if (_2k)			/* if only 2kB to process */
    page_count = 1;		/* treat as 8kB - one page of RAM */

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
      n = port->receive_packet (packet);
      if (n == END || n == TIMEOUT)
	{
	  port->close_port ();
	  fclose (file);
	  emit error (n);
	  return;
	}
      if (n == DATA)
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

  do
    {
      if (recived != 0)
	n = port->receive_packet (packet);
      if (n == TIMEOUT || n == END)
	{
	  port->close_port ();
	  fclose (file);
	  emit error (n);
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
	{			/* good packet */
	  page_number = recived / 128;	/* each ram page consist of 128 packets */
	  packet_number = recived % 128;
	  retries = 0;

	  /* current packet */
	  if (packet_number == packet[3]
	      && page_number == packet[4] * 256 + packet[5])
	    {
	      memcpy (&data[(packet_number % 32) * 64], &packet[6],
		      FRAMESIZE);
	      if (packet_number % 32 == 31)
		if (fwrite (data, 1, sizeof data, file) < sizeof data)
		  {
		    port->close_port ();
		    fclose (file);
		    emit error (FILEERROR_W);
		    return;
		  }
	      /* if only 2kB RAM, then force end of transmision (send END),
	       * device try to process whole page - 8kB*/
	      if (_2k && recived == 31)
		{					    /*_2k => page_count = 1 */
		  port->send_char (END);
		  port->close_port ();
		  fclose (file);
		  emit error (TRUE);
		  return;
		}
	      else
		port->send_char (ACK);
	      recived++;
	      prv_count = 0;
	      /* set amount of data with _2k flag */
	      emit set_progress (recived + 1, _2k ? 32 : page_count * 128);
	    }
	  else if (((packet_number - 1) % 127) == packet[3])
	    {			/* last packet */
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
	    }
	  else
	    {			/* packet valid but not current */
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

  if (recived * 64 == page_count * 8192)
    {
      port->close_port ();
      fclose (file);
      emit error (TRUE);
      return;
    }
  else
    {
      port->close_port ();
      fclose (file);
      emit error (END);
      return;
    }
}

void
ReadRamThread::canceled (void)
{
  end = TRUE;
}
