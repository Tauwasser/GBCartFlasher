# Definition of labels
0x0EC	L		Start
0x120	L		Dummy_ISR
0x122	L		UART_ISR
0x188	L		Exit_ISR
0x18A	L		Write_USART
0x192	L		Start_Timer0
0x19C	L		Stop_Timer0
0x1A6	L		Start_Timer1
0x1B4	L		Stop_Timer1
0x1C0	L		Write_GEC
0x1E2	L		Read_GEC
0x206	L		Write_Flash_AIN
0x228	L		Reset_MBC
0x248	L		Turn_LED_Off
0x250	L		Send_SRAM_Enable
0x260	L		Send_SRAM_Disable
0x26A	L		Set_MBC1_Model
0x280	L		Switch_ROM_Bank
0x2CA	L		Switch_SRAM_Bank
0x2D4	L		Read_ROM_Data
0x2F4	L		Read_SRAM_Data
0x318	L		Write_SRAM_Data
0x338	L		Wait_Program_Flash
0x410	L		Program_Flash
0x512	L		Erase_Flash
0x5BE	L		Enter_Product_ID_Mode
0x664	L		Exit_Product_ID_Mode
0x706	L		Clear_Packet
0x71C	L		Add_CRC
0x774	L		Compute_CRC
0x7A4	L		Check_CRC
0x7CC	L		Create_CRC
0x7E0	L		Wait_Packet
0x896	L		Write_ROM_RAM
0x9D6	L		Send_Packet
0xA46	L		Read_ROM_RAM
0xB84	L		Manipulate_Data
0xBEC	L		Erase_Data
0xC62	L		Send_Status
0xD52	L		Port_Setup
0xE06	L		_Send_NAK
0xE16	L		_Timeout


# Definition of data
0x22	P	W	16	LUT0
0x42	P	W	16	LUT1
0x62	P	B	48	NINTENDO_LOGO
0x92	P	B	18	PRODUCT_ID_EXIT
0xA4	P	B	18	PRODUCT_ID_ENTRY
0xB6	P	B	36	CHIP_ERASE
0xDA	P	B	18	BYTE_PROGRAM

# Definition of registers
0x0060	M	B	1	CRC_HI
0x0061	M	B	1	Input_Cnt
0x0062	M	B	1	Receive_Register
0x0063	M	B	64	FRAME
0x00A3	M	B	72	PACKET
0x00EB	M	B	1	MBC
0x00EC	M	B	1	Algorithm
0x00ED	M	B	1	Flash_Type
0x00EE	M	B	1	NumBanks_HI
0x00EF	M	B	1	NumBanks_LO
0x00F0	M	B	1	Receive_Mode
0x00F1	M	B	1	Receive_Status
0x00F2	M	B	1	CRC_LO
