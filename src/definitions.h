#ifndef DEFINITIONS_H_
#define DEFINITIONS_H_

/**
 * Packet Size.
 * Should be sizeof(packet_t).
 */
#define PACKETSIZE 72
/**
 * Frame Size.
 * Should be sizeof(packet_t.data.frame).
 */
#define FRAMESIZE 64

/**
 * Firmware Major Version.
 */
#define FW_VER_MAJ "02"
/**
 * Firmware Mino Version.
 */
#define FW_VER_MIN "10"

/**
 * Helper macro to convert char
 * numeric values to binary values.
 * @param \_X\_ Char input '0' - '9'.
 */
#define CHAR2NUM(_X_) (_X_ - '0')

/**
 * Enumeration of all supported
 * Memory Bank Controllers.
 */
enum mbc_types {

	/**
	 * Automatic Selection of
	 * Memory Bank Controller
	 * based on cartridge header.
	 */
	MBCAUTO = 0x00,
	/**
	 * Memory Bank Controller 1.
	 */
	MBC1    = 0x01,
	/**
	 * Memory Bank Controller 2.
	 */
	MBC2    = 0x02,
	/**
	 * Memory Bank Controller 3.
	 */
	MBC3    = 0x03,
	/**
	 * up to 64kByte ROM only.
	 */
	ROMONLY = 0x04,
	/**
	 * Memory Bank Controller 5.
	 */
	MBC5    = 0x05,
	/**
	 * Memory Bank Controller 5
	 * with rumble support.
	 */
	RUMBLE  = 0x06,

};

/**
 * Enumeration of all
 * flash write modes.
 */
enum write_mode {

	/**
	 * Program flash chip.
	 */
	FLASH_PROGRAM = 0x00u,
	/**
	 * Erase flash chip.
	 */
	FLASH_ERASE   = 0x01u,

};

/**
 * Enumeration of all supported
 * flash algorithms.
 */
enum flash_algorithm {

	/**
	 * 16-bit algorithm.
	 */
	ALG16 = 0x00u,
	/**
	 * 12-bit algorithm.
	 */
	ALG12 = 0x01u,

};

/**
 * Enumeration of all
 * possible means of detecting
 * the completion of a program/erase cycle
 * for flash chips.
 */
enum write_status_detection {

	/**
	 * Wait long amount of time.
	 * 10.1s for chip erase,
	 * 110us for chip program.
	 */
	LONGER    = 0x00u,
	/**
	 * Wait regular amount of time.
	 * 5.1s for chip erase,
	 * 55us for chip program.
	 */
	DEFAULT   = 0x01u,
	/**
	 * Poll Data on DQ7.
	 * DQ7 will be the inverse of the actual
	 * data written to FLASH until completion
	 * of program/erase cycle.
	 */
	DATA_POLL = 0x02u,
	/**
	 * Check flash DQ6.
	 * DQ6 will toggle on successive reads
	 * while program/erase is in progress.
	 * Toggle will stop, once finished.
	 */
	TOGGLE    = 0x03u,

};

/**
 * Enumeration of all packet types.
 */
enum packet_type {

	/**
	 * Configuration.
	 */
	CONFIG      = 0x00u,
	/**
	 * Data package.
	 * Used when reading/writing
	 * ROM, FLASH or SRAM.
	 */
	NORMAL_DATA = 0x01u,
	/**
	 * Last data package.
	 * Used when reading/writing
	 * the last payload from/to
	 * ROM, FLASH or SRAM.
	 */
	LAST_DATA   = 0x02u,
	/**
	 * Erase ROM, FLASH or SRAM.
	 */
	ERASE       = 0x03u,
	/**
	 * Status Type.
	 * This is used to determine firmware version
	 * and, optionally, also flash device info and
	 * reading the cartridge header.
	 */
	STATUS      = 0x04u,

};

/**
 * Enumeration of
 * Memory Bank Controller 1's
 * memory models.
 */
enum mbc1_memory_model {

	/**
	 * Default Memory Model.
	 * 16 Mbit of ROM, 8 kB of SRAM.
	 */
	MBC1_16_8   = 0x00u,
	/**
	 * Extended Memory Model.
	 * 4 MBit of ROM, 32 kB of SRAM.
	 */
	MBC1_4_32   = 0x01u,

};

/**
 * Enumeration of all
 * memory types.
 */
enum memory {

	/**
	 * SRAM.
	 */
	RAM = 0x00u,
	/**
	 * ROM.
	 */
	ROM = 0x01u,

};

/**
 * Enumeration of all supported
 * read/write operations.
 */
enum rw_operation {

	/**
	 * Read ROM.
	 */
	RROM = 0x00u,
	/**
	 * Read SRAM.
	 */
	RRAM = 0x01u,
	/**
	 * Write ROM.
	 */
	WROM = 0x02u,
	/**
	 * Write SRAM.
	 */
	WRAM = 0x03u,
	/**
	 * Erase FLASH.
	 */
	EFLA = 0x00u,
	/**
	 * Erase SRAM.
	 */
	ERAM = 0x01u,

};

/**
 * Enumeration of optional
 * status information.
 */
enum stat_operation {

	/**
	 * Do not read cartridge header
	 * and flash device info.
	 */
	DONT_READ_INFO = 0x00,
	/**
	 * Read cartridge header
	 * and flash device info.
	 */
	READ_INFO      = 0x01,

};

/**
 * Enumeration of
 * all supported commands.
 */
enum command {

	/**
	 * Acknowledge.
	 */
	ACK  = 0xAAu,
	/**
	 * Not Acknowledge.
	 */
	NAK  = 0xF0u,
	/**
	 * End Command.
	 * Quits transfer in
	 * case of errors.
	 */
	END  = 0x0Fu,
	/**
	 * Data Command.
	 * Initiate all operations.
	 */
	DATA = 0x55u,

};

/**
 * Packet Structure.
 * The communication to the host PC is
 * package-based.
 * There are fixed header parts and
 * depending on the package type,
 * several different layouts of actual
 * data relevant for the operation.
 */
struct packet_t {

	union {


		/**
		 * General Package Layout
		 */
		union {

			/**
			 * Raw package bytes.
			 */
			uint8_t raw[PACKETSIZE];

			/**
			 * Common package format.
			 */
			struct {

				/**
				 * Payload bytes.
				 */
				uint8_t payload[PACKETSIZE - 2];
				/**
				 * High byte of package CRC.
				 */
				uint8_t crc_hi;
				/**
				 * Low byte of package CRC.
				 */
				uint8_t crc_lo;

			};

		};

		/**
		 * Request package header.
		 */
		struct {

			/**
			 * Package command.
			 * One of ::command.
			 */
			uint8_t command;
			/**
			 * Package type.
			 * One of ::packet_type.
			 */
			uint8_t type;
			/**
			 * Package operation.
			 * Depending on type.
			 * One of ::rw_operation.
			 */
			uint8_t operation;
			/**
			 * Memory Bank Controller.
			 * MBC to be assumed for this session.
			 * One of ::mbc_types.
			 */
			uint8_t mbc;
			/**
			 * Flash Algorithm.
			 * Algorithm to be used for this session.
			 * One of ::flash_algorithm.
			 */
			uint8_t algorithm;
			/**
			 * Flash Wait Mode.
			 * Wait mode to be used for this session.
			 * One of ::write_status_detection.
			 */
			uint8_t wait_mode;
			/**
			 * High byte of number of ROM/SRAM banks.
			 */
			uint8_t num_banks_hi;
			/**
			 * Low byte of number of ROM/SRAM banks.
			 */
			uint8_t num_banks_lo;

		} request;

		/**
		 * Erase Request.
		 */
		struct {

			/**
			 * Package command.
			 * One of ::command.
			 */
			uint8_t command;
			/**
			 * Package type.
			 * One of ::packet_type.
			 */
			uint8_t type;
			/**
			 * Package operation.
			 * Depending on type.
			 * One of ::rw_operation.
			 */
			uint8_t operation;
			/**
			 * Memory Bank Controller.
			 * MBC to be assumed for this session.
			 * One of ::mbc_types.
			 */
			uint8_t mbc;
			/**
			 * Number of SRAM banks or flash algorithm.
			 * Depending on #operation.
			 * Latter is one of ::flash_algorithm.
			 */
			uint8_t num_banks_or_algorithm;
			uint8_t wait_mode;

		} erase;

		/**
		 * Information Request Answer.
		 */
		struct {

			/**
			 * Package command.
			 * One of ::command.
			 */
			uint8_t command;
			/**
			 * Package type.
			 * One of ::packet_type.
			 */
			uint8_t type;
			/**
			 * Major Version Number.
			 * Two figures 4-bit BCD.
			 */
			uint8_t ver_maj;
			/**
			 * Minor Version Number.
			 * Two figures in 4-bit BCD.
			 */
			uint8_t ver_min;
			/**
			 * Flash Chip Manufacturer ID.
			 */
			uint8_t flash_manufacturer;
			/**
			 * Flash Chip Device ID.
			 */
			uint8_t flash_device_id;
			/**
			 * Flash Chip Sector Group Protect.
			 */
			uint8_t flash_sector_group_protect;
			/**
			 * Unused.
			 */
			uint8_t byte_07;
			/**
			 * Cartridge Nintendo Logo correct.
			 * 0x00u - Incorrect
			 * 0x01u - Correct
			 */
			uint8_t cart_logo_correct;
			/**
			 * Cartridge Header Name.
			 */
			uint8_t cart_name[16];
			/**
			 * Cartridge Header High Byte of New Licensee Code.
			 */
			uint8_t cart_new_licensee_hi;
			/**
			 * Cartridge Header Low Byte of New Licensee Code.
			 */
			uint8_t cart_new_licensee_lo;
			/**
			 * Cartridge Header SGB Compatiblity byte.
			 */
			uint8_t cart_sgb;
			/**
			 * Catridge Header Memory Bank Controller byte.
			 */
			uint8_t cart_mbc;
			/**
			 * Cartridge Header ROM size byte.
			 */
			uint8_t cart_rom_size;
			/**
			 * Cartridge Header SRAM size byte.
			 */
			uint8_t cart_ram_size;
			/**
			 * Cartridge Header Destination Code.
			 */
			uint8_t cart_dst_code;
			/**
			 * Cartridge Header Old Licensee Code.
			 */
			uint8_t cart_old_licensee;
			/**
			 * Cartridge Header Mask ROM Version.
			 */
			uint8_t cart_mask_rom_version;
			/**
			 * Cartridge Header Complement Checksum.
			 */
			uint8_t cart_complement;
			/**
			 * Cartridge Header High Byte of Cart Checksum.
			 */
			uint8_t cart_checksum_hi;
			/**
			 * Cartridge Header Low Byte of Cart Checksum.
			 */
			uint8_t cart_checksum_lo;

		} info_ans;

		/**
		 * Data Request/Answer Packet.
		 */
		struct {

			/**
			 * Package command.
			 * One of ::command.
			 */
			uint8_t command;
			/**
			 * Package type.
			 * One of ::packet_type.
			 */
			uint8_t type;
			/**
			 * Reserved for future use.
			 * 0x00u.
			 */
			uint8_t reserved;
			/**
			 * Packet Index.
			 */
			uint8_t packet_index;
			/**
			 * High Byte of Page Index.
			 * Either ROM, FLASH or SRAM.
			 */
			uint8_t page_index_hi;
			/**
			 * Low Byte of Page Index.
			 * Either ROM, FLASH or SRAM.
			 */
			uint8_t page_index_lo;
			/**
			 * Actual payload data from/to ROM, FLASH or SRAM.
			 */
			uint8_t frame[FRAMESIZE];

		} data;


	};

}__attribute__((packed));


#endif /* DEFINITIONS_H_ */
