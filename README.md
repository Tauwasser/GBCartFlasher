# Interface for Nintendo® Game Boy® Cartridges

This is the host software for the Game Boy Cart Flasher, which was originally developed during 2005–2007 by
Polish Game Boy enthusiasts Kraku & Chroost.

For a reimplementation of the microcontroller firmware for the USB model in the C programming language,
check out the master branch *master*.

For information on the hardware and original firmware images, check out the hardware branch
*hardware*.

## Features

The Game Boy Cart Flasher is an ATmega8515 MCU which interfaces which either USB or RS-232 and a
GB Game Edge Connector (GEC) Receptacle (REC) to communicate both ways. It can read ROM, read/write 
SRAM, and read/write commonly available FLASH chips.

The ATmega8515 communicates using UART either directly via a MAX232 RS-232 driver/receiver or via 
FTDI's FT232 USB 2.0 FS bridge IC. The ATmega8515 is clocked at 6 MHz.

This reimplemented firmware only supports the USB version of GB Cart Flasher.

## Compilation

Use QTCreator with qmake to compile this project.

## Licensing

This host software is licensed under the GNU General Public Licence Version 2, or any later version, at the licensee's option.
(SPDX-License-Identifier: GPL-2.0-or-later). See COPYING for a full text of the license.

```
 This program is free software; you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation; either version 2 of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with this program; if not, write to the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA. 
```

## Legalese

I'm not affiliated with Nintendo in any way. Neither are Kraku or Chroost.

Game Boy® is a registered trademark by Nintendo. Nintendo® is a registered trademark.
All other trademarks are property of their respective owner.
