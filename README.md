# Interface for Nintendo® Game Boy® Cartridges

These are the hardware design files, schematics and binary firmware images for the Game Boy Cart Flasher,
which was originally developed during 2005–2007 by Polish Game Boy enthusiasts Kraku & Chroost.
There are two different models, the RS-232 and the USB model.

For a reimplementation of the microcontroller firmware for the USB model in the C programming language,
check out the master branch *master*.

For information on the host software, check out the software branch *reader_sw*. 

## Features

The Game Boy Cart Flasher is an ATmega8515 MCU which interfaces which either USB or RS-232 and a
GB Game Edge Connector (GEC) Receptacle (REC) to communicate both ways. It can read ROM, read/write 
SRAM, and read/write commonly available FLASH chips.

The ATmega8515 communicates using UART either directly via a MAX232 RS-232 driver/receiver or via 
FTDI's FT232 USB 2.0 FS bridge IC. The ATmega8515 is clocked at 6 MHz.

This reimplemented firmware only supports the USB version of GB Cart Flasher.

## Printed Circuit Board

The hardware designs were originally provided as PDF files for toner-transfer technique used by enthusiasts.
However, there are quite a few smaller commercial houses that will gladly word off of the PDF files to
produce a printed circuit board.

## Firmware

Flashing the firmware requires an external flasher and adapter as the boards are not equipped for
in-system programmability.

## Disassembly

The excellent avrdisas 0.06-win32 <https://www.johannes-bauer.com/mcus/avrdisas/> was used to aid in the disassembly.
The tag file can be found inside the firmware folder.

## Licensing

The original files came without an explicit license other than the statement "[T]his project is free 
and any commercial use is prohibited". I, Tauwasser, believe the BY-NC-SA 4.0 International license 
to best match the spirit and intent of the original authors, Kraku & Chroost.

## Legalese

I'm not affiliated with Nintendo in any way. Neither are Kraku or Chroost.

Game Boy® is a registered trademark by Nintendo. Nintendo® is a registered trademark.
All other trademarks are property of their respective owner.
