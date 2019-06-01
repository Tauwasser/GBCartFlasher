# Interface for Nintendo® Game Boy® Cartridges

This is a firmware for the Game Boy Cart Flasher, which was originally developed during 2005–2007 by
Polish Game Boy enthusiasts Kraku & Chroost.
This firmware is a reimplementation of the USB firmware in the C programming language.

For information on the hardware and original firmware images, check out the hardware branch
*hardware*.

For information on the host software, check out the software branch *reader_sw*. 

## Features

The Game Boy Cart Flasher is an ATmega8515 MCU which interfaces which either USB or RS-232 and a
GB Game Edge Connector (GEC) Receptacle (REC) to communicate both ways. It can read ROM, read/write 
SRAM, and read/write commonly available FLASH chips.

The ATmega8515 communicates using UART either directly via a MAX232 RS-232 driver/receiver or via 
FTDI's FT232 USB 2.0 FS bridge IC. The ATmega8515 is clocked at 6 MHz.

This reimplemented firmware only supports the USB version of GB Cart Flasher.

## Compilation

Use Atmel Studio v7.0 or later to compile this firmware. You should have at least one .hex file after 
successful compilation.

## Licensing

The original files came without an explicit license other than the statement "[T]his project is free 
and any commercial use is prohibited". I, Tauwasser, believe the BY-NC-SA 4.0 International license 
to best match the spirit and intent of the original authors, Kraku & Chroost.

This C firmware reimplementation is licensed under the Attribution-NonCommercial-ShareAlike 4.0 International.

```
Copyright (C) 2005–2007 Kraku & Chroost (gbflasher@interia.pl)
Copyright (C) 2015 Tauwasser (tauwasser@tauwasser.eu)

This work is licensed under the Creative Commons Attribution-NonCommercial-ShareAlike 4.0
International License.

To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-sa/4.0/
or send a letter to Creative Commons, PO Box 1866, Mountain View, CA 94042, USA.
```

## Legalese

I'm not affiliated with Nintendo in any way. Neither are Kraku or Chroost.

Game Boy® is a registered trademark by Nintendo. Nintendo® is a registered trademark.
All other trademarks are property of their respective owner.
