/* GB Cart Reader Microcontroller Firmware
 * Copyright (C) 2005–2007 Kraku & Chroost (gbflasher@interia.pl)
 * Copyright (C) 2015 Tauwasser (tauwasser@tauwasser.eu)
 * 
 * This work is licensed under the Creative Commons Attribution-NonCommercial-ShareAlike 4.0 
 * International License.
 * 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-sa/4.0/
 * or send a letter to Creative Commons, PO Box 1866, Mountain View, CA 94042, USA.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 */
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

#include <avr/io.h>
#include <avr/pgmspace.h>
#include <avr/interrupt.h>

#include "definitions.h"

/**
 * Macro that generates wait loops to create correct bus timing
 * that isn't optimized away by the compiler and doesn't create
 * a variable on the stack.
 * @param \_X\_   Number of _"cycles" to wait.
 *                \#cycles = 1 + (\_X\_ - 1) * 2 + \_X\_ * sizeof(\_VAR\_)
 *                \#cycles = \#last_branch + (\_X\_ - 1) * \#other_branches + (\_X\_ + 1) * \#decrement_and_load
 * @param \_VAR\_ Loop variable. Must be big enough to hold \_X\_.
 */
#define WAIT_LOOP(_X_, _VAR_) for (_VAR_ = _X_; _VAR_; --_VAR_) __asm__ __volatile__ ("" : "=r"(_VAR_) : "r"(_VAR_))

/**
 * Command Sequence for entering flash programming mode.
 * Format: address hi, address lo, data.
 */
const uint8_t BYTE_PROGRAM[] PROGMEM = {

	/** ALG16 */
	0x55, 0x55, 0xaa, 0x2a, 0xaa, 0x55, 0x55, 0x55, 0xa0,

	/** ALG12 */
	0x05, 0x55, 0xaa, 0x02, 0xaa, 0x55, 0x05, 0x55, 0xa0

};

/**
 * Command Sequence to issue flash chip erase.
 * Format: address hi, address lo, data
 */
const uint8_t CHIP_ERASE[] PROGMEM = {

	/** ALG16 */
	0x55, 0x55, 0xaa, 0x2a, 0xaa, 0x55, 0x55, 0x55, 0x80, 0x55, 0x55, 0xaa, 0x2a, 0xaa, 0x55, 0x55, 0x55, 0x10,

	/** ALG12 */
	0x05, 0x55, 0xaa, 0x02, 0xaa, 0x55, 0x05, 0x55, 0x80, 0x05, 0x55, 0xaa, 0x02, 0xaa, 0x55, 0x05, 0x55, 0x10

};

/**
 * Command Sequence for entering product ID mode.
 * Format: address hi, address lo, data.
 */
const uint8_t PRODUCT_ID_ENTRY[] PROGMEM = {

	/** ALG16 */
	0x55, 0x55, 0xaa, 0x2a, 0xaa, 0x55, 0x55, 0x55, 0x90,

	/** ALG12 */
	0x05, 0x55, 0xaa, 0x02, 0xaa, 0x55, 0x05, 0x55, 0x90

};

/**
 * Command Sequence to exit product ID mode.
 * Format: address hi, address lo, data.
 */
const uint8_t PRODUCT_ID_EXIT[] PROGMEM = {

	/** ALG16 */
	0x55, 0x55, 0xaa, 0x2a, 0xaa, 0x55, 0x55, 0x55, 0xf0,

	/** ALG12 */
	0x05, 0x55, 0xaa, 0x02, 0xaa, 0x55, 0x05, 0x55, 0xf0

};

/**
 * Nintendo Character Data.
 * Used to verify cartridge character data.
 */
const uint8_t NINTENDO_LOGO[] PROGMEM = {

	0xCE, 0xED, 0x66, 0x66, 0xCC, 0x0D, 0x00, 0x0B,
	0x03, 0x73, 0x00, 0x83, 0x00, 0x0C, 0x00, 0x0D,
	0x00, 0x08, 0x11, 0x1F, 0x88, 0x89, 0x00, 0x0E,
	0xDC, 0xCC, 0x6E, 0xE6, 0xDD, 0xDD, 0xD9, 0x99,
	0xBB, 0xBB, 0x67, 0x63, 0x6E, 0x0E, 0xEC, 0xCC,
	0xDD, 0xDC, 0x99, 0x9F, 0xBB, 0xB9, 0x33, 0x3E

};

/**
 * CRC-16 CCITT Lookup Table 1.
 * High Order Bits
 */
const uint16_t LUT1[] PROGMEM = {

	0x0000, 0x0010, 0x0020, 0x0030, 0x0040, 0x0050, 0x0060, 0x0070, 0x0081, 0x0091, 0x00a1, 0x00b1, 0x00c1, 0x00d1, 0x00e1, 0x00f1

};

/**
 * CRC-16 CCITT Lookup Table 0.
 * Low Order Bits
 */
const uint16_t LUT0[] PROGMEM = {

	0x0000, 0x0021, 0x0042, 0x0063, 0x0084, 0x00a5, 0x00c6, 0x00e7, 0x0008, 0x0029, 0x004a, 0x006b, 0x008c, 0x00ad, 0x00ce, 0x00ef

};

/**
 * High byte of calculated CRC.
 */
static uint8_t crc_hi8;
/**
 * Index into packet.raw when receiving packet.
 */
static volatile uint8_t packet_raw_ix;
/**
 * USART raw UDR byte.
 * Used to store UDR when in ::raw_udr_mode.
 */
static volatile uint8_t raw_udr;
/**
 * Temporary storage for frame data when
 * reading/writing.
 */
static uint8_t frame[FRAMESIZE];
/**
 * Packet Data.
 * Used when receiving/transmitting a packet
 * to the host PC.
 */
static volatile struct packet_t packet;
/**
 * Memory Bank Controller to be used
 * for current session.
 */
static uint8_t cur_mbc;
/**
 * Flash Chip Algorithm.
 */
static enum flash_algorithm cur_flash_algorithm;
/**
 * Flash Chip Wait Mode.
 */
static uint8_t cur_wait_mode;
/**
 * High byte of number of banks
 * for current operation.
 */
static uint8_t cur_numbanks_lo;
/**
 * Low byte of number of banks
 * for current operation.
 */
static uint8_t cur_numbanks_hi;
/**
 * USART raw UDR mode switch.
 * 0x00 - Packet mode.
 * 0x01 - Raw UDR mode.
 */
static volatile uint8_t raw_udr_mode;
/**
 * USART raw UDR flag.
 * 0x00 - No data received.
 * 0x01 - Byte received and stored in ::raw_udr.
 */
static volatile uint8_t raw_udr_received;
/**
 * Low byte of calculated CRC.
 */
static uint8_t crc_lo8;

/**
 * USART Receive Interrupt Handler.
 * Stores data either in current frame if any space is left
 * or in ::raw_udr when in raw UDR mode, depending on ::raw_udr_mode switch.
 */
ISR(USART_RX_vect)
{

	if (!raw_udr_mode && packet_raw_ix < PACKETSIZE) {

		packet.raw[packet_raw_ix++] = UDR;

	}

	if (raw_udr_mode && !raw_udr_received) {

		raw_udr = UDR;
		raw_udr_received = 0x01;

	}

}

static volatile uint8_t *out_ptr;

ISR(USART_UDRE_vect)
{
	
	if (out_ptr != (packet.raw + PACKETSIZE)) {
	
		UDR = *out_ptr++;
		return;
		
	}
	
	UCSRB &= ~(1u << UDRIE);
}

/**
 * USART Write Routine.
 * @param b Byte to be written to USART.
 */
void write_usart(const uint8_t b)
{

	// wait for data register empty, then write data
	while (!(UCSRA & (1u << UDRE)));
	UDR = b;

}

void write_frame(void) {
	
	out_ptr = packet.raw;
	UCSRB |= (1u << UDRIE);
	
}

/**
 * Start 8-bit timer.
 * Clock is prescaled by 8.
 */
void start_timer0(void)
{

	TIFR = (1u << TOV0);  // clear overflow flag
	TCNT0 = 0x00u;        // reset counter
	TCCR0 = (1u << CS01); // clk_io/8

}

/**
 * Stop 8-bit timer.
 */
void stop_timer0(void)
{

	TCCR0 = 0x00u;        // no clock source
	TCNT0 = 0x00u;        // reset counter
	TIFR = (1u << TOV0);  // reset overflow flag

}

/**
 * Start 16-bit timer.
 * Clock is prescaled by 2048.
 */
void start_timer1(void)
{

	TIFR = (1u << OCF1A); // clear output compare match a flag
	TCNT1H = 0x00u;       // reset counter
	TCNT1L = 0x00u;
	// clk_io/2048 and clear timer on compare match (ctc)
	TCCR1B = (1u << WGM12 | 1u << CS12 | 1u << CS10);

}

/**
 * Stop 16-bit timer.
 */
void stop_timer1(void)
{

	TCCR1B = 0x00u;      // no clock source
	TCNT1H = 0x00u;      // reset counter
	TCNT1L = 0x00u;
	TIFR = (1u << OCF1A); // clear output compare match a flag

}

/**
 * Write data to Game Edge Connector (GEC).
 * @param d Data byte to be written.
 */
void write_gec(const uint8_t d)
{

	uint8_t i;

	// output d on PORTC
	PORTC = d;
	DDRC = 0xFFu;

	WAIT_LOOP(0x02u, i);

	PORTD &= ~(1u << PD3); // assert /WR

	WAIT_LOOP(0x02u, i);

	PORTD |= (1u << PD3); // deassert /WR

	WAIT_LOOP(0x02u, i);

	// PORTC is input, pull-up
	DDRC = 0x00u;
	PORTC = 0xFFu;

}

/**
 * Read data from Game Edge Connector (GEC).
 * @return Data byte read.
 */
const uint8_t read_gec(void)
{

	uint8_t i;
	uint8_t result;

	// PORTC is input, pull-up
	DDRC = 0x00u;
	PORTC = 0xFFu;

	WAIT_LOOP(0x02u, i);

	PORTD &= ~(1u << PD4); // assert /RD

	WAIT_LOOP(0x02u, i);

	result = PINC;
	PORTD |= (1u << PD4); // deassert /RD

	WAIT_LOOP(0x02u, i);

	return result;

}

/**
 * Write to flash chip using AIN method.
 * AIN is supposed to go to a low-active /WE
 * on flash chip.
 * @param d Byte to be written to flash.
 */
void write_flash_ain(const uint8_t d)
{

	uint8_t i;

	// output d on PORTC
	PORTC = d;
	DDRC = 0xFFu;

	WAIT_LOOP(0x02u, i);

	PORTD &= ~(1u << PD7); // assert /AIN

	WAIT_LOOP(0x02u, i);

	PORTD |= (1u << PD7); // deassert /AIN

	WAIT_LOOP(0x02u, i);

	// PORTC is input, pull-up
	DDRC = 0x00u;
	PORTC = 0xFFu;

}

/**
 * Reset Memory Bank Controller (MBC).
 */
void reset_mbc(void)
{

	uint16_t i;

	PORTD &= ~(1u<< PD2); // assert /RED_LED

	WAIT_LOOP(15000u, i);

	PORTD &= ~(1u << PD6); // assert /RST

	WAIT_LOOP(1500u, i);

	PORTD |= (1u << PD6); // deassert /RST

	WAIT_LOOP(1500u, i);

	// Unlock Sachen Mapper
	i = 0x30u;
	while (i-- > 0) {

		PORTA = 0x80u;
		write_gec(0xFFu);
		PORTA = 0x00u;
		read_gec();

	}

	PORTA = 0x80u;
	write_gec(0xFFu);

}

/**
 * Reset address busses.
 * Reset GEC address bus to 0x0000u
 * and turn red LED off.
 */
void turn_led_off(void)
{

	PORTB = 0x00u;
	PORTA = 0x00u;

	PORTD |= (1u << PD2); // deassert /RED_LED

}

/**
 * Send command to enable SRAM to MBC.
 * Writes 0x0Au to 0x0000u.
 */
void send_sram_enable(void)
{

	uint8_t i;

	PORTB = 0x00u;
	PORTA = 0x00u;

	write_gec(0x0Au);
	WAIT_LOOP(0x04u, i);

}

/**
 * Send command to disable SRAM to MBC.
 * Writes 0x00u to 0x0000u.
 */
void send_sram_disable(void)
{

	PORTB = 0x00u;
	PORTA = 0x00u;

	write_gec(0x00u);

}

/**
 * Select Memory Bank Controller 1 memory model.
 * Writes model to 0x6000u.
 * @param model MBC1 memory model.
 */
void set_mbc1_model(const uint8_t model)
{

	if (MBC2 > cur_mbc) {

		PORTB = 0x00u;
		PORTA = 0x60u;

		write_gec(model);

	}

}

/**
 * Switch MBC ROM Bank.
 * Writes bank_lo to 0x2000 and - preceding that -
 * if ::cur_mbc is ::MBC5 or ::MBCAUTO bank_hi to 0x3000.
 * \note MBC2 implementation is broken and will actually
 *       only disable/enable SRAM depending on bank_lo.
 * @param bank_hi High byte of ROM bank.
 * @param bank_lo Low byte of ROM bank.
 */
void switch_rom_bank(const uint8_t bank_hi, const uint8_t bank_lo)
{

	uint8_t i;

	PORTB = 0x00u;

	PORTA = 0x20u;
	
	if (bank_lo)
		write_gec(bank_lo);
	else
		write_gec(bank_lo | 0x80u);

}

/**
 * Switch MBC SRAM Bank.
 * Writes byte to 0x4000u.
 * @param b Bank byte.
 */
void switch_sram_bank(const uint8_t b)
{

	PORTB = 0x00u;
	PORTA = 0x40u;
	write_gec(b);

}

/**
 * Read data from ROM.
 * Will return 0xFFu if address is outside permissible range.
 * This will not switch away from the current ROM bank.
 * ROM bank is only provided to account for bank 0x000u.
 * @param address ROM address, relative to current bank (i.e. range 0x0000u -- 0x3FFFu)
 * @param bank_hi High byte of ROM bank.
 * @param bank_lo Low byte of ROM bank.
 * @return Data byte read from GEC.
 */
const uint8_t read_rom_data(const uint16_t address, const uint8_t bank_hi, const uint8_t bank_lo)
{

	uint8_t result;
	uint8_t addr_hi = address >> 8;

	PORTB = address & 0xFFu;

	if ((address >> 8) < 0x40u) {
		
		// dump all RB from 0x4000 range
		addr_hi |= 0x40u;

		PORTA = addr_hi;
		result = read_gec();

	} else {

		result = 0xFFu;

	}

	return result;

}

/**
 * Read data from SRAM.
 * Will return 0xFFu if address is outside permissible range.
 * @param address SRAM address, relative to current bank (i.e. range 0x0000u -- 0x1FFFu)
 * @return Data byte read from GEC.
 */
const uint8_t read_sram_data(const uint16_t address)
{

	uint8_t result;
	uint8_t i;

	PORTB = address & 0xFFu;

	if ((address >> 8) < 0x20u) {

		PORTA = (address >> 8) | 0xA0u;

		WAIT_LOOP(0x02u, i);

		PORTD &= ~(1u << PD5); // assert /CS
		result = read_gec();
		PORTD |= (1u << PD5);  // deassert /CS

	} else {

		result = 0xFFu;

	}

	return result;

}

/**
 * Write data to SRAM.
 * Will not write anything if address is outside of range.
 * @param d Byte to be written.
 * @param address SRAM address, relative to current bank (i.e. range 0x0000u -- 0x1FFFu)
 */
void write_sram_data(const uint8_t d, const uint16_t address)
{

	uint8_t i;

	PORTB = address & 0xFFu;

	if ((address >>8) < 0x20u) {

		PORTA = (address >> 8) | 0xA0u;

		WAIT_LOOP(0x02u, i);

		PORTD &= ~(1u << PD5); // assert /CS
		write_gec(d);
		PORTD |= (1u << PD5);  // deassert /CS

	}

}

/**
 * Wait for flash chip write cycle completion.
 * @param option Wait mode, one of ::write_status_detection.
 * @param d Byte written. Used to wait in ::DATA_POLL mode.
 */
void wait_program_flash(const uint8_t option, const uint8_t d)
{

	uint16_t i;
	int8_t j;
	uint8_t k;

	switch ((enum write_status_detection) cur_wait_mode) {

		case DATA_POLL:

			k = d & 0x80u;

			if (FLASH_PROGRAM == option) {

				start_timer0();

				do {

					WAIT_LOOP(0x04u, j);

					if ((read_gec() & 0x80u) == k)
						break;

				} while (!(TIFR & (1u << TOV0)));

				stop_timer0();

			} else {

				do {

					WAIT_LOOP(0x04u, j);

				} while ((read_gec() & 0x80u) != k);

			}

			break;

		case TOGGLE:

			do {

				WAIT_LOOP(0x04u, j);

				k = read_gec() & 0x40u;

				WAIT_LOOP(0x04u, j);

			} while (k != (read_gec() & 0x40u));

			break;

		case DEFAULT:

			if (FLASH_PROGRAM == option) {

				// actual cycles from original ASM: 330
				// 55us
				WAIT_LOOP(110, j);
				break;

			}

			// 0x33 * 0x0A * 15000 cycles wait
			// actual cycles from original ASM: 30602703
			// ~5.1s
			k = 0x00u;
			do {
				
				j = 0x09;
				do {
					
					WAIT_LOOP(15000, i);
					
				} while (--j >= 0);
				
			} while (++k < 0x33u);

			break;

		case LONGER:

			// $$FALL-THROUGH$$

		default:

			if (FLASH_PROGRAM == option) {

				// actual cycles from original ASM: 660
				// 110us
				WAIT_LOOP(220, j);
				break;

			}
			
			// 0x65 * 0x0A * 15000 cycles wait
			// actual cycles from original ASM: 60605353
			// ~10.1s
			k = 0x00u;
			do {
				
				j = 0x09;
				do {
					
					WAIT_LOOP(15000, i);
					
				} while (--j >= 0);
				
			} while (++k < 0x65u);
			
			break;

	}

}

/**
 * Program Flash Chip.
 * Sends programming sequence to flash chip using /AIN (::cur_flash_algorithm) and regular /WE (::ALG12) methods.
 * @param data    Byte to be written.
 * @param address ROM address, relative to bank (i.e. range 0x0000u -- 0x1FFFu)
 * @param bank_hi High byte of ROM bank.
 * @param bank_lo Low byte of ROM bnak.
 */
void program_flash(const uint8_t data, const uint16_t address, const uint8_t bank_hi, const uint8_t bank_lo)
{

	uint16_t addr;
	uint16_t ix;
	uint8_t i;
	uint16_t offset;
	const uint8_t* offset1;
	const uint8_t* offset2;
	const uint8_t* offset3;

	addr = address;

	if (0x00u != bank_hi || 0x00 != bank_lo)
		addr |= 0x4000u;

	// RB = 0x001 for addressing flash chip
	switch_rom_bank(0x00u, 0x01u);

	//////////////////////////////////////////////////////////////////////////
	// Write data using AIN
	//////////////////////////////////////////////////////////////////////////

	// Bring flash chip into write mode
	for (ix = 0x0000u, i = 0x09u; i; ix += 0x03u, i -= 0x03u) {

		offset = cur_flash_algorithm * 0x09u + ix;

		PORTA = pgm_read_byte(&BYTE_PROGRAM[offset + 0]);
		PORTB = pgm_read_byte(&BYTE_PROGRAM[offset + 1]);

		write_flash_ain(pgm_read_byte(&BYTE_PROGRAM[offset + 2]));

	}

	switch_rom_bank(bank_hi, bank_lo);
	PORTA = addr >> 8;
	PORTB = addr & 0xFFu;
	write_flash_ain(data);

	//////////////////////////////////////////////////////////////////////////
	// Write data using /WR
	//////////////////////////////////////////////////////////////////////////

	offset3 = &BYTE_PROGRAM[0x09u + 2];
	offset2 = &BYTE_PROGRAM[0x09u + 1];
	offset1 = &BYTE_PROGRAM[0x09u + 0];

	// Bring flash chip into write mode
	for (i = 0x09u; i; i -= 0x03u, offset3 += 0x03u, offset2 += 0x03u, offset1 += 0x03u) {

		PORTA = pgm_read_byte(offset1);
		PORTB = pgm_read_byte(offset2);

		write_gec(pgm_read_byte(offset3));

	}

	//FIXME: This seems broken.
	//       Why is memory model changed _after_ writing to GEC?
	PORTA = addr >> 8;
	PORTB = addr & 0xFFu;
	write_gec(data);

	set_mbc1_model(MBC1_16_8);
	switch_rom_bank(bank_hi, bank_lo);
	PORTA = addr >> 8;
	PORTB = addr & 0xFFu;

	wait_program_flash(FLASH_PROGRAM, data);

}

/**
 * Erase Flash Chip.
 * Erases whole chip using /AIN (::cur_flash_algorithm) and regular /WE (::ALG12 only) methods.
 */
void erase_flash(void)
{

	uint8_t i;
	uint16_t ix;
	uint16_t offset;
	const uint8_t* offset1;
	const uint8_t* offset2;
	const uint8_t* offset3;

	// RB = 0x001 for addressing flash chip
	switch_rom_bank(0x00u, 0x01u);

	offset3 = &CHIP_ERASE[0x12u + 2];
	offset2 = &CHIP_ERASE[0x12u + 1];
	offset1 = &CHIP_ERASE[0x12u + 0];

	for (ix = 0x0000u, i = 0x12u; i; ix += 0x03u, offset3 += 0x03u, offset2 += 0x03u, offset1 += 0x03u, i -= 0x03u) {

		//////////////////////////////////////////////////////////////////////////
		// Erase data using AIN
		//////////////////////////////////////////////////////////////////////////

		offset = cur_flash_algorithm * 0x12u + ix;

		PORTA = pgm_read_byte(&CHIP_ERASE[offset + 0]);
		PORTB = pgm_read_byte(&CHIP_ERASE[offset + 1]);

		write_flash_ain(pgm_read_byte(&CHIP_ERASE[offset + 2]));

		//////////////////////////////////////////////////////////////////////////
		// Erase data using /WR
		//////////////////////////////////////////////////////////////////////////

		PORTA = pgm_read_byte(offset1);
		PORTB = pgm_read_byte(offset2);

		write_gec(pgm_read_byte(offset3));

	}

	wait_program_flash(FLASH_ERASE, 0xFFu);

}

/**
 * Enter Flash Chip product ID mode.
 * Enters mode using /AIN (::cur_flash_algorithm) and regular /WE (::ALG12 only) methods.
 */
void enter_product_id_mode(void)
{

	uint8_t i;
	uint16_t ix;
	uint16_t offset;
	const uint8_t* offset1;
	const uint8_t* offset2;
	const uint8_t* offset3;

	// RB = 0x001 for addressing flash chip
	switch_rom_bank(0x00u, 0x01u);

	offset3 = &PRODUCT_ID_ENTRY[0x09u + 2];
	offset2 = &PRODUCT_ID_ENTRY[0x09u + 1];
	offset1 = &PRODUCT_ID_ENTRY[0x09u + 0];

	for (ix = 0x0000u, i = 0x09u; i; ix += 0x03u, offset3 += 0x03u, offset2 += 0x03u, offset1 += 0x03u, i -= 0x03u) {

		//////////////////////////////////////////////////////////////////////////
		// Enter mode using AIN
		//////////////////////////////////////////////////////////////////////////

		offset = cur_flash_algorithm * 0x09u + ix;

		PORTA = pgm_read_byte(&PRODUCT_ID_ENTRY[offset + 0]);
		PORTB = pgm_read_byte(&PRODUCT_ID_ENTRY[offset + 1]);

		write_flash_ain(pgm_read_byte(&PRODUCT_ID_ENTRY[offset + 2]));

		//////////////////////////////////////////////////////////////////////////
		// Enter mode using /WR
		//////////////////////////////////////////////////////////////////////////

		PORTA = pgm_read_byte(offset1);
		PORTB = pgm_read_byte(offset2);

		write_gec(pgm_read_byte(offset3));

	}

}

/**
 * Exit Flash Chip product ID mode.
 * Exits mode using /AIN (::cur_flash_algorithm) and regular /WE (::ALG12 only) methods.
 */
void exit_product_id_mode(void)
{

	uint8_t i;
	uint16_t ix;
	uint16_t offset;
	const uint8_t* offset1;
	const uint8_t* offset2;
	const uint8_t* offset3;

	offset3 = &PRODUCT_ID_EXIT[0x09u + 2];
	offset2 = &PRODUCT_ID_EXIT[0x09u + 1];
	offset1 = &PRODUCT_ID_EXIT[0x09u + 0];

	for (ix = 0x0000u, i = 0x09u; i; ix += 0x03u, offset3 += 0x03u, offset2 += 0x03u, offset1 += 0x03u, i -= 0x03u) {

		//////////////////////////////////////////////////////////////////////////
		// Enter mode using AIN
		//////////////////////////////////////////////////////////////////////////

		offset = cur_flash_algorithm * 0x09u + ix;

		PORTA = pgm_read_byte(&PRODUCT_ID_EXIT[offset + 0]);
		PORTB = pgm_read_byte(&PRODUCT_ID_EXIT[offset + 1]);

		write_flash_ain(pgm_read_byte(&PRODUCT_ID_EXIT[offset + 2]));

		//////////////////////////////////////////////////////////////////////////
		// Enter mode using /WR
		//////////////////////////////////////////////////////////////////////////

		PORTA = pgm_read_byte(offset1);
		PORTB = pgm_read_byte(offset2);

		write_gec(pgm_read_byte(offset3));

	}

	send_sram_disable();

}

/**
 * Reinitialize Packet.
 * Fill packet.raw with all zeros and set packet.data.command to ::DATA.
 */
void clear_packet(void)
{

	uint8_t i;

	for (i = PACKETSIZE - 1; i; i--) {

		packet.raw[i + 1] = 0x00u;

	}

	packet.data.command = DATA;

}

/**
 * Add nibble to current CRC-16 CCITT value.
 * \note High bits of nibble byte must not be set!
 * @param nibble Nibble to be added.
 */
void add_crc(const uint8_t nibble)
{

	uint8_t ix = nibble ^ (crc_hi8 >> 4);

	crc_hi8 = (((crc_hi8 << 4) | (crc_lo8 >> 4)) & 0xFFu) ^ pgm_read_byte(&LUT1[ix]);
	crc_lo8 = (crc_lo8 << 4) ^ pgm_read_byte(&LUT0[ix]);

}

/**
 * Compute packet CRC-16 CCITT.
 * Compute new CRC value for packet.payload in ::crc_hi8 and ::crc_lo8.
 */
void compute_crc(void)
{

	uint8_t i;
	volatile uint8_t* p;

	crc_lo8 = 0x00u;
	crc_hi8 = 0x00u;

	p = packet.payload;

	for (i = sizeof(packet.payload) ; i ; --i) {

		add_crc((*p & 0xF0u) >> 4);
		add_crc(*p++ & 0x0Fu);

	}

}

/**
 * Check packet for correct CRC.
 * \note ::crc_lo8 and ::crc_hi8 are changed.
 * @return <code>false</code> if CRC is correct, else <code>true</code>.
 */
const uint16_t check_crc(void)
{
	uint16_t result = true;

	compute_crc();

	if (packet.crc_hi == crc_hi8 && packet.crc_lo == crc_lo8)
		result = false;

	return result;

}

/**
 *  Compute and apply packet CRC.
 *  Compute new CRC value for packet.payload and store CRC in crc_hi and crc_lo.
 *  \note ::crc_lo8 and ::crc_hi8 are changed.
 */
void create_crc(void)
{

	compute_crc();
	packet.crc_hi = crc_hi8;
	packet.crc_lo = crc_lo8;

}

/**
 * Wait for next packet from host PC.
 * @param packet_ix Expected packet index.
 * @param bank_hi   Expected high byte of bank.
 * @param bank_lo   Expected low byte of bank.
 * @param ram_rom   Depending on ram_rom, the page_index rolls over at 0x100u (for ::ROM), or at 0x80u (for ::RAM).
 * @return <code>false</code> on success, else <code>true</code>, i.e. on timeout, crc failure or packet index failure.
 */
const uint8_t wait_packet(const uint8_t packet_ix, const uint8_t bank_hi, const uint8_t bank_lo, const uint8_t ram_rom)
{

	uint8_t crc_fail_counter = 0x00u;
	uint8_t ix_fail_counter = 0x00u;

	uint8_t packet_ix_adj;
	uint8_t packet_ix_adj_rom;

	while (1) {

		start_timer1();

		// Wait for packet or timeout
		while (1) {

			if (PACKETSIZE <= packet_raw_ix) {

				stop_timer1();

				if (DATA == packet.data.command && !check_crc()) {

					// Neither of the permitted packet types
					if (NORMAL_DATA != packet.data.type && LAST_DATA != packet.data.type) {

						write_usart(END);
						return true;

					}

					if (packet_ix == packet.data.packet_index) {


						 if (bank_hi == packet.data.page_index_hi && bank_lo == packet.data.page_index_lo) {

							 return false;

						 }

						 write_usart(END);
						 return true;

					}

					packet_ix_adj_rom = packet_ix - 1;

					if (!(RAM == ram_rom)) {

						packet_ix_adj = packet_ix_adj_rom;

					} else {

						packet_ix_adj = packet_ix_adj_rom & 0x7Fu;

					}

					if (packet_ix_adj == packet.data.packet_index) {

						ix_fail_counter++;

						if (10 <= ix_fail_counter) {

							write_usart(END);
							return true;

						}

						packet_raw_ix = 0x00u;

						write_usart(ACK);
						break;

					}

					write_usart(END);
					return true;

				}

				crc_fail_counter++;

				if (10 <= crc_fail_counter) {

					write_usart(END);
					return true;

				}

				packet_raw_ix = 0x00u;

				write_usart(NAK);
				break;

			}

			if (TIFR & (1u << OCF1A)) {

				stop_timer1();
				return true;

			}

		}

	}

	return true;

}

/**
 * Write data to either ROM or RAM.
 * Enter continuous write mode from host PC to cartridge.
 * @param ram_rom Target, must be either ::ROM or ::RAM.
 */
void write_rom_ram(const uint8_t ram_rom)
{

	uint16_t num_banks;
	uint8_t packet_per_bank;
	uint16_t packet_ix;
	uint16_t bank_ix;
	uint16_t address;
	uint8_t packet_type;
	uint8_t i;
	volatile uint8_t* p;
	uint8_t* p_rom;
	uint8_t* p_ram;
	uint8_t* d;

	reset_mbc();

	if (RAM != ram_rom) {

		set_mbc1_model(MBC1_16_8);

	} else {

		set_mbc1_model(MBC1_4_32);
		send_sram_enable();

	}

	num_banks = (cur_numbanks_hi << 8) + cur_numbanks_lo;

	if (num_banks == 0xFFFFu)
		num_banks = 0xFFFEu;

	if (RAM != ram_rom) {

		packet_per_bank = 0x100u - 1;

	} else {

		packet_per_bank = 0x80u - 1;

	}

	raw_udr_mode = 0x00u;
	packet_raw_ix = 0x00u;
	write_usart(ACK);

	bank_ix = 0x0000u;

	do {

		if (ROM != ram_rom)
			switch_sram_bank(bank_ix & 0xFFu);

		packet_ix = 0x0000u;
		address = 0x0000u;

		do {

			if (wait_packet(packet_ix, bank_ix >> 8, bank_ix & 0xFFu, ram_rom)) {
				send_sram_disable();
				return;
			}

			p = packet.raw;
			d = frame;

			for (i = FRAMESIZE; i; i--, p++) {

				*d++ = *(p + offsetof(struct packet_t, data.frame));

			}

			packet_type = packet.request.type;
			packet_raw_ix = 0x00u;
			write_usart(ACK);

			p_rom = frame;
			p_ram = frame;

			for (i = FRAMESIZE; i ; i--, p_ram++, p_rom++) {

				if (RAM != ram_rom) {

					program_flash(*p_rom, address, bank_ix >> 8, bank_ix & 0xFFu);

				} else {

					write_sram_data(*p_ram, address);

				}

				address++;

			}

			if (LAST_DATA == packet_type) {
				send_sram_disable();
				return;
			}

			packet_ix++;

		} while (packet_ix <= packet_per_bank);

		bank_ix++;

	} while (bank_ix <= num_banks);

	send_sram_disable();

}

/**
 * Send packet to host PC.
 * @return <code>false</code> on success, else <code>true</code>, i.e. when host cancels transaction or times out.
 */
const uint8_t send_packet(void)
{

	uint8_t tries;

	create_crc();
	raw_udr_received = false;

	for (tries = 0x00; tries < 0x09; tries++, raw_udr_received = false) {

		write_frame();

		start_timer1();

		while (1) {

			if (raw_udr_received) {

				stop_timer1();

				if (END == raw_udr)
					return true;

				if (ACK != raw_udr)
					break;

				return false;

			}

			if (!(TIFR & (1u << OCF1A)))
				continue;

			stop_timer1();
			return true;

		}

	}

	return true;

}

/**
 * Read data from either ROM or RAM.
 * Enter continuous read mode from cartrdige to host PC.
 * @param ram_rom Target, must be either ::ROM or ::RAM.
 */
void read_rom_ram(const uint8_t ram_rom)
{

	uint16_t num_banks;
	uint8_t packet_per_bank;
	uint16_t packet_ix;
	uint16_t bank_ix;
	uint16_t address;
	uint8_t data;
	uint8_t packet_type;
	uint8_t i;
	volatile uint8_t* p;
	volatile uint8_t* p_rom;
	volatile uint8_t* p_ram;

	reset_mbc();

	if (RAM != ram_rom) {

		set_mbc1_model(MBC1_4_32);

	} else {

		set_mbc1_model(MBC1_4_32);
		send_sram_enable();

	}

	num_banks = (cur_numbanks_hi << 8) + cur_numbanks_lo;

	if (num_banks == 0xFFFFu)
		num_banks = 0xFFFEu;

	if (RAM != ram_rom) {

		packet_per_bank = 0x100u - 1;

	} else {

		packet_per_bank = 0x80u - 1;

	}

	raw_udr_mode = 0x01u;
	bank_ix = 0x0000u;

	do {

		if (RAM != ram_rom) {

			switch_rom_bank(bank_ix >> 8, bank_ix & 0xFFu);

		} else {

			switch_sram_bank(bank_ix & 0xFFu);

		}

		packet_ix = 0x0000u;
		address = 0x0000u;

		do {

			clear_packet();

			if (bank_ix == num_banks && packet_ix == packet_per_bank)
				packet_type = 0x02;
			else
				packet_type = 0x01;

			packet.data.type = packet_type;
			packet.data.packet_index = packet_ix;
			packet.data.page_index_hi = bank_ix >> 8;
			packet.data.page_index_lo = bank_ix & 0xFFu;

			p_rom = packet.raw;
			p_ram = packet.raw;

			for (i = FRAMESIZE; i; i--, address++, p_rom++, p_ram++) {

				if (RAM != ram_rom) {

					data = read_rom_data(address, bank_ix >> 8, bank_ix & 0xFFu);
					p = p_rom;

				} else {

					data = read_sram_data(address);
					p = p_ram;

				}

				*(p + offsetof(struct packet_t, data.frame)) = data;

			}

			if (send_packet()) {

				if (ROM != ram_rom)
					send_sram_disable();

				return;

			}

			packet_ix++;

		} while (packet_ix <= packet_per_bank);

		bank_ix++;

	} while (bank_ix <= num_banks);

	if (ROM != ram_rom) {

		send_sram_disable();

	}

}

/**
 * Global delegate for all manipulation commands.
 * Saves session information such as MBC, flash algorithm etc.
 * and calls requested operation in packet.request.operation.
 * Not acknowledges to host for unknown operation.
 */
void manipulate_data(void)
{

	cur_mbc             = packet.request.mbc;
	cur_flash_algorithm = packet.request.algorithm & (ALG12 - ALG16 + 1 - 1);
	cur_wait_mode       = packet.request.wait_mode;
	cur_numbanks_hi     = packet.request.num_banks_hi;
	cur_numbanks_lo     = packet.request.num_banks_lo;

	switch ((enum rw_operation) packet.request.operation) {

		case RRAM:

			read_rom_ram(RAM);
			break;

		case WROM:

			write_rom_ram(ROM);
			break;

		case WRAM:

			write_rom_ram(RAM);
			break;

		case RROM:

			read_rom_ram(ROM);
			break;

		default:

			write_usart(NAK);
			break;

	};

}

/**
 * Erase either FLASH or SRAM.
 * Issue chip erase to FLASH or erase SRAM bank,
 * depending on packet.erase.operation.
 */
void erase_data(void)
{

	uint16_t sram_bank;
	uint16_t address;

	reset_mbc();

	cur_mbc = packet.erase.mbc;

	if (EFLA != packet.erase.operation) {

		// assume ERAM
		set_mbc1_model(MBC1_4_32);
		send_sram_enable();

		sram_bank = 0x0000u;
		packet.erase.num_banks_or_algorithm;

		do {

			switch_sram_bank(sram_bank & 0xFFu);

			for (address = 0x0000u; address < 0x2000u; address++) {

				write_sram_data(0x00u, address);

			}

			sram_bank++;

		} while (sram_bank <= packet.erase.num_banks_or_algorithm);

	} else {

		// EFLA
		cur_flash_algorithm = packet.erase.num_banks_or_algorithm & (ALG12 - ALG16 + 1 - 1);
		cur_wait_mode       = packet.erase.wait_mode;
		erase_flash();

	}

	send_sram_disable();
	write_usart(ACK);

}

/**
 * Send answer to status information request.
 * Send firmware version information and - depending on packet.request.operation -
 * cartridge header and flash device data as well.
 */
void send_status(void)
{

	uint8_t i;
	uint8_t read_header = false;
	const uint8_t* logo_p;
	uint16_t address;
	volatile uint8_t* p;

	if (READ_INFO == packet.request.operation) {

		read_header         = true;
		cur_mbc             = packet.request.mbc;
		cur_flash_algorithm = packet.request.algorithm & (ALG12 - ALG16 + 1 - 1);

	}

	clear_packet();

	if (read_header) {

		reset_mbc();
		packet.info_ans.cart_logo_correct = true;

		logo_p = NINTENDO_LOGO;
		address = 0x0104u;

		for (i = sizeof(NINTENDO_LOGO); i ; --i, address++, logo_p++) {

			if (read_rom_data(address, 0x00u, 0x00u) != pgm_read_byte(logo_p))
				packet.info_ans.cart_logo_correct = false;

		}

		address = 0x0134u;
		p = packet.info_ans.cart_name;

		for (i = offsetof(struct packet_t, info_ans.cart_checksum_lo) - offsetof(struct packet_t, info_ans.cart_name) + 1; i; i--, address++) {

			*p++ = read_rom_data(address, 0x00u, 0x00u);

		}

		WAIT_LOOP(0x04u, i);

		enter_product_id_mode();

		WAIT_LOOP(220u, i);

		packet.info_ans.flash_manufacturer         = read_rom_data(0x0000u, 0x00u, 0x00u);
		packet.info_ans.flash_device_id            = read_rom_data(0x0001u, 0x00u, 0x00u);
		packet.info_ans.flash_sector_group_protect = read_rom_data(0x0002u, 0x00u, 0x00u);

		exit_product_id_mode();

	}

	packet.info_ans.ver_maj = (CHAR2NUM(FW_VER_MAJ[0]) << 4) | CHAR2NUM(FW_VER_MAJ[1]);
	packet.info_ans.ver_min = (CHAR2NUM(FW_VER_MIN[0]) << 4) | CHAR2NUM(FW_VER_MIN[1]);

	create_crc();

	write_frame();

}

/**
 * Main Program Loop.
 * Setup of all ports.
 * Set USART speed depending on E0 and E2:
 * <ul><li>Short E0 to E1 for 125,000 baud</li>
 * <li>Short E2 to E1 for 375,000 baud\</li>
 * <li>Leave all three pins open for 187,500 baud.</li>
 * </ul>
 * Not acknowledges to host on unknown/unexpected packet.request.command,
 * packet.request.type, CRC error or timeout.
 * Never returns.
 */
int main(void)
{

	PORTA = 0x00u;
	DDRA = 0xFFu;
	PORTB = 0x00u;
	DDRB = 0xFFu;

	DDRC = 0x00u;
	PORTC = 0xFFu;

	PORTD = 0xFFu;
	DDRD = 0xFF ^ (1 << DDD0);

	PORTE = 0x05u;
	DDRE = (1u << DDE1);

	if ((PINE & (1u << DDE0 | 1u << DDE2)) == 0x01u) {

		UBRRL = 0x02;

	} else if ((PINE & (1u << DDE0 | 1u << DDE2)) == 0x04u) {

		UBRRL = 0x00;

	} else {

		UBRRL = 0x01;

	}

	DDRE = 0x00u;
	PORTE = 0x07u;

	OCR1AH = 0x44u;
	OCR1AL = 0xAAu;

	ACSR = (1u << ACD);
	UBRRL = 0x00; // force 750000 baud anyway
	UCSRA = (1u << U2X);
	UCSRB = (1u << RXCIE | 1u << RXEN | 1u << TXEN);
	sei();

	while (1)
	{

		// Wait for incoming data
		while(packet_raw_ix == 0x00u);

		// Start timeout
		start_timer1();

		while (1) {

			if (TIFR & (1u << OCF1A)) {

				stop_timer1();
				packet_raw_ix = 0x00u;
				break;

			}

			if (PACKETSIZE <= packet_raw_ix) {

				stop_timer1();

				if (DATA == packet.request.command && !check_crc()) {

					switch ((enum packet_type) packet.request.type) {

						case ERASE:

							erase_data();
							break;

						case STATUS:

							send_status();
							break;

						case CONFIG:

							manipulate_data();
							break;

						default:

							write_usart(NAK);
							break;

					}

				} else {

					write_usart(NAK);

				}

				packet_raw_ix = 0x00u;
				raw_udr_mode = 0x00u;
				turn_led_off();
				break;

			}

		}

	}
	
	return 0;

}
