; Disassembly of gbcf-fw-2.1-usb.BIN (avr-gcc style)

;;    MCU          GB
;; A7 .. A0 == A15 .. A8
;; B7 .. B0 ==  A7 .. A0
;; C7 .. C0 ==  D7 .. D0
;; D7 == AIN
;; D6 == /RST
;; D5 == /CS
;; D4 == /RD
;; D3 == /WR
;; D2 == /RED_LED
;; D1 == USART TxD
;; D0 == USART RxD

;; RAM Map
;; 0060 - calculated crc hi8
;; 0061 - frame byte counter/index
;; 0062 - raw byte receive register
;;
;; 0063-00A2 - temporary copy of payload data (00A9 - 00E8)
;;
;; struct frame {
;;  00A3 - command
;;          0xAA - ACK
;;          0xF0 - NACK
;;          0x0F - END
;;          0x55 - DATA
;;  00A4 - operation
;;          0x00 - CONFIG
;;          0x01 - NORMAL_DATA (only sent)
;;          0x02 - LAST_DATA (only sent)
;;          0x03 - ERASE
;;          0x04 - STATUS
;;  00A5 - read cart info
;;          0x00 - no
;;          0x01 - yes
;;  00A6 - MBC
;;          ver 2.1
;;          0x00 - ROM ONLY?
;;          0x01 - MBC1
;;          0x02 - MBC2
;;          0x03 - MBC3
;;          0x04 - ROMONLY?
;;          0x05 - MBC5
;;          0x06 - RUMBLE
;;  00A7 - Flash Algorithm
;;          0x00 - ALG16
;;          0x01 - ALG12
;;  00A8 - Flash Type
;;          0x00 - LONGER
;;          0x01 - DEFAULT
;;          0x02 - DATAPOLL
;;          0x03 - TOGGLE
;;  00A9 - NumBanks hi8
;;  00AA - NumBanks lo8
;;  
;;  00E9 - CRC hi8
;;  00EA - CRC lo8
;; }
;; 00EB - session MBC
;; 00EC - session Flash Type
;; 00ED - session Flash Algorithm
;; 00EE - session NumBanks hi8
;; 00EF - session NumBanks lo8
;; 00F0 - receive frame (00)/ receive raw byte (<> 00)
;; 00F1 - raw byte receive status (00 - not ~, <> 00 - received)
;; 00F2 - calculated crc lo8

main:
rjmp    Start         ; RESET
rjmp    Dummy_ISR     ; INT0
rjmp    Dummy_ISR     ; INT1
rjmp    Dummy_ISR     ; TIMER1 CAPT
rjmp    Dummy_ISR     ; TIMER1 COMPA
rjmp    Dummy_ISR     ; TIMER1 COMBP
rjmp    Dummy_ISR     ; TIMER1 OVF
rjmp    Dummy_ISR     ; TIMER0 OVF
rjmp    Dummy_ISR     ; SPI, STC
rjmp    UART_ISR      ; USART, RXC
rjmp    Dummy_ISR     ; USART, UDRE
rjmp    Dummy_ISR     ; USART, TXC
rjmp    Dummy_ISR     ; ANA_COMP
rjmp    Dummy_ISR     ; INT2
rjmp    Dummy_ISR     ; TIMER0 COMP
rjmp    Dummy_ISR     ; EE_RDY
rjmp    Dummy_ISR     ; SPM_RDY
; Inline PGM data: 16 words starting at 0x22 (LUT0)
.dw 0x0000
.dw 0x0021
.dw 0x0042
.dw 0x0063
.dw 0x0084
.dw 0x00a5
.dw 0x00c6
.dw 0x00e7
.dw 0x0008
.dw 0x0029
.dw 0x004a
.dw 0x006b
.dw 0x008c
.dw 0x00ad
.dw 0x00ce
.dw 0x00ef

; Inline PGM data: 16 words starting at 0x42 (LUT1)
.dw 0x0000
.dw 0x0010
.dw 0x0020
.dw 0x0030
.dw 0x0040
.dw 0x0050
.dw 0x0060
.dw 0x0070
.dw 0x0081
.dw 0x0091
.dw 0x00a1
.dw 0x00b1
.dw 0x00c1
.dw 0x00d1
.dw 0x00e1
.dw 0x00f1

; Inline PGM data: 48 bytes starting at 0x62 (NINTENDO_LOGO)
.db 0xce, 0xed, 0x66, 0x66, 0xcc, 0x0d, 0x00, 0x0b, 0x03, 0x73, 0x00, 0x83, 0x00, 0x0c, 0x00, 0x0d, 0x00, 0x08, 0x11, 0x1f, 0x88, 0x89, 0x00, 0x0e, 0xdc, 0xcc, 0x6e, 0xe6, 0xdd, 0xdd, 0xd9, 0x99, 0xbb, 0xbb, 0x67, 0x63, 0x6e, 0x0e, 0xec, 0xcc, 0xdd, 0xdc, 0x99, 0x9f, 0xbb, 0xb9, 0x33, 0x3e

; Inline PGM data: 18 bytes starting at 0x92 (PRODUCT_ID_EXIT)
.db 0x55, 0x55, 0xaa, 0x2a, 0xaa, 0x55, 0x55, 0x55, 0xf0,   0x05, 0x55, 0xaa, 0x02, 0xaa, 0x55, 0x05, 0x55, 0xf0

; Inline PGM data: 18 bytes starting at 0xa4 (PRODUCT_ID_ENTRY)
.db 0x55, 0x55, 0xaa, 0x2a, 0xaa, 0x55, 0x55, 0x55, 0x90,   0x05, 0x55, 0xaa, 0x02, 0xaa, 0x55, 0x05, 0x55, 0x90

; Inline PGM data: 36 bytes starting at 0xb6 (CHIP_ERASE)
.db 0x55, 0x55, 0xaa, 0x2a, 0xaa, 0x55, 0x55, 0x55, 0x80, 0x55, 0x55, 0xaa, 0x2a, 0xaa, 0x55, 0x55, 0x55, 0x10,   0x05, 0x55, 0xaa, 0x02, 0xaa, 0x55, 0x05, 0x55, 0x80, 0x05, 0x55, 0xaa, 0x02, 0xaa, 0x55, 0x05, 0x55, 0x10

; Inline PGM data: 18 bytes starting at 0xda (BYTE_PROGRAM)
.db 0x55, 0x55, 0xaa, 0x2a, 0xaa, 0x55, 0x55, 0x55, 0xa0,   0x05, 0x55, 0xaa, 0x02, 0xaa, 0x55, 0x05, 0x55, 0xa0

; Referenced from offset 0x00 by rjmp
Start:
clr     r1				; r1 = 0x00
out     SREG, r1
ldi     r28, 0x5f
ldi     r29, 0x02
out     SPH, r29
out     SPL, r28		; SP = 0x025F
  
  ;; Disabled Init SRAM Data
  
ldi     r17, 0x00		; 0x0060 bytes
ldi     r26, 0x60
ldi     r27, 0x00		; to X = 0x0060
ldi     r30, 0x1e
ldi     r31, 0x0e		; from Z = 0x0E1E
rjmp    Label6

; Referenced from offset 0x10c by brne
Label5:
lpm     r0, Z+
st      X+, r0

; Referenced from offset 0x102 by rjmp
Label6:
cpi     r26, 0x60		; 0x0060 comparison
cpc     r27, r17
brne    Label5
 
 ;; Clear 0x0060 - 0x00F2
 
ldi     r17, 0x00		; 0x00F3 bytes
ldi     r26, 0x60
ldi     r27, 0x00		; to X = 0x0060
rjmp    Label8

; Referenced from offset 0x11c by brne
Label7:
st      X+, r1

; Referenced from offset 0x114 by rjmp
Label8:
cpi     r26, 0xf3       ; 0x00F3 comparison
cpc     r27, r17
brne    Label7
rjmp    Port_Setup

; Referenced from offset 0x14 by rjmp
; Referenced from offset 0x16 by rjmp
; Referenced from offset 0x10 by rjmp
; Referenced from offset 0x1a by rjmp
; Referenced from offset 0x1c by rjmp
; Referenced from offset 0x1e by rjmp
; Referenced from offset 0x20 by rjmp
; Referenced from offset 0x0e by rjmp
; Referenced from offset 0x0c by rjmp
; Referenced from offset 0x0a by rjmp
; Referenced from offset 0x08 by rjmp
; Referenced from offset 0x06 by rjmp
; Referenced from offset 0x04 by rjmp
; Referenced from offset 0x02 by rjmp
; Referenced from offset 0x18 by rjmp
Dummy_ISR:
rjmp    Exit_ISR

; Referenced from offset 0x12 by rjmp
UART_ISR:
push    r1
push    r0
in      r0, SREG		;; push Status Register
push    r0
clr     r1				;; r1 = 0x00
push    r24
push    r30
push    r31
lds     r24, 0x00f0
tst     r24
brne    Label11			;; (0x00F0) != 0x00
lds     r24, 0x0061
cpi     r24, 0x48
brcc    Label11			;; (0x0061) >= 0x48
lds     r24, 0x0061
mov     r30, r24		;; r30 = r24 = (0x0061)
clr     r31				;; r31 = 0x00
subi    r30, 0x5d
sbci    r31, 0xff		;; Z = r31 r30 - 0xFF5D = r31 r30 + 0x00A3
in      r24, UDR		;; r24 <= Received Data
st      Z, r24			;; Store Received Data to (0x0061) + 0x00A3
lds     r24, 0x0061
subi    r24, 0xff       ; (0x0061)++
sts     0x0061, r24

; Referenced from offset 0x138 by brne
; Referenced from offset 0x140 by brcc

 ;; 0x00F0 is not 0x00

Label11:
lds     r24, 0x00f0
tst     r24
breq    Label12			;; (0x00F0) == 0x00
lds     r24, 0x00f1
tst     r24
brne    Label12			;; (0x00F1) != 0x00
in      r24, UDR
sts     0x0062, r24		;; (0x0062) <= Received Data	(Receive_Status)
ldi     r24, 0x01
sts     0x00f1, r24		;; Disable Receive_Status

; Referenced from offset 0x162 by breq
; Referenced from offset 0x16a by brne
Label12:
pop     r31
pop     r30
pop     r24
pop     r0
out     SREG, r0		;; restore status register
pop     r0
pop     r1
reti

; Referenced from offset 0x120 by rjmp
Exit_ISR:
reti

; Referenced from offset 0x9f2 by rcall
; Referenced from offset 0x18c by rjmp
; Referenced from offset 0xbe8 by rcall
; Referenced from offset 0xc56 by rcall
; Referenced from offset 0x954 by rcall
; Referenced from offset 0x90a by rcall
; Referenced from offset 0xd3e by rcall
; Referenced from offset 0x882 by rcall
; Referenced from offset 0x87c by rcall
; Referenced from offset 0xe08 by rcall
 ;; Send r24 via USART

Write_USART:
sbis    UCSRA, 5		;; USART Data Register == Empty
rjmp    Write_USART
out     UDR, r24		;; USART Data Register <= r24
ret


; Referenced from offset 0x39e by rcall
Start_Timer0:
ldi     r24, 0x02
out     TIFR, r24		;; Clear Timer 0 Overflow Flag
out     TCNT0, r1		;; Clear Timer 0 Counter
out     TCCR0, r24		;; Timer 0 Counter Control Register <= CLK_IO/8
ret


; Referenced from offset 0x3be by rcall
Stop_Timer0:
out     TCCR0, r1		;; Timer 0 Counter Control Register <= CLK_NONE
out     TCNT0, r1		;; Clear Timer 0 Counter
ldi     r24, 0x02
out     TIFR, r24		;; Clear Timer 0 Overflow Flag
ret


; Referenced from offset 0xdbc by rcall
; Referenced from offset 0x9fa by rcall
; Referenced from offset 0x7f8 by rcall
Start_Timer1:
ldi     r24, 0x40       ; 64
out     TIFR, r24		;; Reset Timer 1 Output Compare A Match flag
out     TCNT1H, r1
out     TCNT1L, r1		;; Reset Timer 1 Counter
ldi     r24, 0x0d
out     TCCR1B, r24		;; Timer 1 Counter Control Register B <= CLR_TMA_ON_COMPARE_MATCH | CLK_IO/1024
ret


; Referenced from offset 0xdcc by rcall
; Referenced from offset 0xa0a by rcall
; Referenced from offset 0x808 by rcall
; Referenced from offset 0xe16 by rcall
; Referenced from offset 0x80c by rcall
; Referenced from offset 0xa0e by rcall
Stop_Timer1:
out     TCCR1B, r1		;; Timer 1 Counter Control Register B <= CLK_NONE
out     TCNT1H, r1 
out     TCNT1L, r1		;; Reset Timer 1 Counter
ldi     r24, 0x40
out     TIFR, r24		;; Reset Timer 1 Output Compare A Match flag
ret


; Referenced from offset 0x29a by rcall
; Referenced from offset 0x6d2 by rcall
; Referenced from offset 0x2a8 by rcall
; Referenced from offset 0x586 by rcall
; Referenced from offset 0x4e2 by rcall
; Referenced from offset 0x2c4 by rcall
; Referenced from offset 0x2d0 by rcall
; Referenced from offset 0x4c2 by rcall
; Referenced from offset 0x27c by rcall
; Referenced from offset 0x266 by rcall
; Referenced from offset 0x332 by rcall
; Referenced from offset 0x256 by rcall
; Referenced from offset 0x632 by rcall
 ;; Write r24 to GEC
 
Write_GEC:
out     PORTC, r24		;; C <= r24
ser     r25
out     DDRC, r25		;; Port C is output
ldi     r24, 0x02

; Referenced from offset 0x1ca by brne
Label14:
dec     r24
brne    Label14			;; Wait 2 cycles
cbi     PORTD, 3		;; D3 <= 0 (/WR)
ldi     r24, 0x02

; Referenced from offset 0x1d2 by brne
Label15:
dec     r24
brne    Label15			;; Wait 2 cycles
sbi     PORTD, 3		;; D3 <= 1 (/WR)
ldi     r24, 0x02

; Referenced from offset 0x1da by brne
Label16:
dec     r24
brne    Label16			;; Wait 2 cycles
out     DDRC, r1		;; Port C is input
out     PORTC, r25		;; C <= 0xFF
ret


; Referenced from offset 0x3c8 by rcall
; Referenced from offset 0x3d4 by rcall
; Referenced from offset 0x2ee by rcall
; Referenced from offset 0x38a by rcall
; Referenced from offset 0x3a6 by rcall
; Referenced from offset 0x310 by rcall
 ;; Read r24 from GEC

Read_GEC:
out     DDRC, r1		;; Port C is input
ser     r24
out     PORTC, r24		;; C <= 0xFF
ldi     r24, 0x02

; Referenced from offset 0x1ec by brne
Label17:
dec     r24
brne    Label17			;; Wait 2 cycles
cbi     PORTD, 4		;; D4 <= 0 (/RD)
ldi     r24, 0x02

; Referenced from offset 0x1f4 by brne
Label18:
dec     r24
brne    Label18			;; Wait 2 cycles
in      r25, PINC		;; r25 <= C
sbi     PORTD, 4		;; D4 <= 1 (/RD)
ldi     r24, 0x02

; Referenced from offset 0x1fe by brne
Label19:
dec     r24
brne    Label19			;; Wait 2 cycles
mov     r24, r25		;; r24 = r25 (= C)
clr     r25				;; r25 = 0x00
ret


; Referenced from offset 0x574 by rcall
; Referenced from offset 0x482 by rcall
; Referenced from offset 0x620 by rcall
; Referenced from offset 0x49a by rcall
; Referenced from offset 0x6c0 by rcall
 ;; Write r24 to FlashCart
 
Write_Flash_AIN:
out     PORTC, r24		;; C <= r24
ser     r25
out     DDRC, r25		;; C is output
ldi     r24, 0x02

; Referenced from offset 0x210 by brne
Label20:
dec     r24				;; Wait 2 cycles
brne    Label20
cbi     PORTD, 7		;; D7 <= 0 (AIN)
ldi     r24, 0x02

; Referenced from offset 0x218 by brne
Label21:
dec     r24				;; Wait 2 cycles
brne    Label21
sbi     PORTD, 7		;; D7 <= 1 (AIN)
ldi     r24, 0x02

; Referenced from offset 0x220 by brne
Label22:
dec     r24				;; Wait 2 cycles
brne    Label22
out     DDRC, r1		;; C is input
out     PORTC, r25		;; C <= 0xFF
ret


; Referenced from offset 0xc92 by rcall
; Referenced from offset 0x8bc by rcall
; Referenced from offset 0xbf4 by rcall
; Referenced from offset 0xa6a by rcall
 ;; Reset MBC
 ;; Turn red LED on.
 
Reset_MBC:
cbi     PORTD, 2		;; D2 <= 0 (/RED_LED)
ldi     r24, 0x98
ldi     r25, 0x3a		;; X = 0x3A98

; Referenced from offset 0x230 by brne
Label23:
sbiw    r24, 0x01		;; Wait 15000 cycles
brne    Label23
cbi     PORTD, 6        ;; D6 <= 0 (/RST)
ldi     r24, 0xdc
ldi     r25, 0x05		;; X = 0x05DC

; Referenced from offset 0x23a by brne
Label24:
sbiw    r24, 0x01		;; Wait 1500 cycles
brne    Label24
sbi     PORTD, 6        ;; D6 <= 1 (/RST)
ldi     r24, 0xdc
ldi     r25, 0x05		;; X = 0x05DC

; Referenced from offset 0x244 by brne
Label25:
sbiw    r24, 0x01		;; Wait 1500 cycles
brne    Label25
ret


; Referenced from offset 0xe12 by rcall
 ;; Turn red LED off.
 
Turn_LED_Off:
out     PORTB, r1		;; B <= 0x00
out     PORTA, r1		;; A <= 0x00
sbi     PORTD, 2        ; D2 <= 1 (/RED_LED)
ret


; Referenced from offset 0xc0a by rcall
; Referenced from offset 0xa7a by rcall
; Referenced from offset 0x8cc by rcall
 ;; Enable SRAM
 ;; i.e. write 0x0A to 0x0000

Send_SRAM_Enable:
out     PORTB, r1		;; B <= 0x00
out     PORTA, r1		;; A <= 0x00
ldi     r24, 0x0a
rcall   Write_GEC		;; Write 0x0A to GEC
ldi     r24, 0x04

; Referenced from offset 0x25c by brne
Label26:
dec     r24				;; Wait 4 cycles
brne    Label26
ret


; Referenced from offset 0xb5e by rcall
; Referenced from offset 0x9ae by rcall
; Referenced from offset 0x6f0 by rcall
; Referenced from offset 0xc52 by rcall
 ;; Disable SRAM
 ;; i.e. write 0x00 to 0x0000

Send_SRAM_Disable:
out     PORTB, r1		;; B <= 0x00
out     PORTA, r1		;; A <= 0x00
ldi     r24, 0x00
rcall   Write_GEC		;; Write 0x00 to GEC
ret


; Referenced from offset 0x8ca by rcall
; Referenced from offset 0xa78 by rcall
; Referenced from offset 0x8c4 by rcall
; Referenced from offset 0xc08 by rcall
; Referenced from offset 0x4e6 by rcall
; Referenced from offset 0xa72 by rcall
 ;; Select MBC1 memory model?
 
Set_MBC1_Model:
mov     r25, r24
lds     r24, 0x00eb
cpi     r24, 0x02
brcc    Label27			;; (0x00EB) >= 02
out     PORTB, r1
ldi     r24, 0x60
out     PORTA, r24		;; Select GB Address 0x6000
mov     r24, r25
rcall   Write_GEC		;; Write r24 to GEC

; Referenced from offset 0x272 by brcc
Label27:
ret


; Referenced from offset 0x492 by rcall
; Referenced from offset 0x528 by rcall
; Referenced from offset 0x4ec by rcall
; Referenced from offset 0x5d4 by rcall
; Referenced from offset 0x448 by rcall
; Referenced from offset 0xac6 by rcall
 ;; Switch GB ROM bank.
 ;; r22		GB bank lo8
 ;; r24		GB bank hi8
 ;; Will change MBC1 4/8 memory model RAM bank!
 
Switch_ROM_Bank:
push    r28
mov     r25, r24
mov     r28, r22
out     PORTB, r1
lds     r24, 0x00eb
cpi     r24, 0x05
breq    Label28			;; (0x00EB) == 0x05
tst     r24
brne    Label30			;; (0x00EB) != 0x00

; Referenced from offset 0x28e by breq
Label28:
ldi     r24, 0x30
out     PORTA, r24		;; Select GB Address 0x3000
mov     r24, r25
rcall   Write_GEC		;; Write r24 to GEC
ldi     r24, 0x02

; Referenced from offset 0x2a0 by brne
Label29:
dec     r24				;; Wait 2 cycles
brne    Label29

; Referenced from offset 0x292 by brne
Label30:
ldi     r24, 0x20
out     PORTA, r24		;; Select GB Address 0x2000 -- FIXME: Won't work for MBC2!!!
mov     r24, r28
rcall   Write_GEC		;; Write r28 to GEC
lds     r24, 0x00eb
cpi     r24, 0x02
brcc    Label32			;; (0x00EB) >= 0x02
ldi     r24, 0x02

; Referenced from offset 0x2b6 by brne
Label31:
dec     r24				;; Wait two cycles
brne    Label31
ldi     r24, 0x40
out     PORTA, r24		;; Select GB Address 0x4000
mov     r24, r28
swap    r24
lsr     r24
andi    r24, 0x07		;; r24 = MBC1 upper rom bank bits
rcall   Write_GEC		;; Write r24 to GEC

; Referenced from offset 0x2b0 by brcc
Label32:
pop     r28
ret


; Referenced from offset 0x91e by rcall
; Referenced from offset 0xacc by rcall
; Referenced from offset 0xc16 by rcall
 ;; Switch RAM bank
 ;; r24 RAM bank
 
Switch_SRAM_Bank:
out     PORTB, r1
ldi     r25, 0x40
out     PORTA, r25		;; Select GB Address 0x4000
rcall   Write_GEC
ret


; Referenced from offset 0xcd8 by rcall
; Referenced from offset 0xcac by rcall
; Referenced from offset 0xb10 by rcall
; Referenced from offset 0xd1a by rcall
; Referenced from offset 0xd0c by rcall
; Referenced from offset 0xcfe by rcall
 ;; Read GB ROM data
 ;; r20		GB Bank lo8
 ;; r22		GB Bank hi8
 ;; r24		GB Address lo8
 ;; r25		GB Address hi8 (range 0x00 - 0x3F)
 ;; Invalid address range will return 0xFF.
 
Read_ROM_Data:
out     PORTB, r24		;; Select GB ROM Addres lo8
mov     r24, r25
clr     r25				;; r25 = 0x00
cpi     r24, 0x40
brcs    Label33			;; r24 < 0x40
ser     r24				;; r24 = 0xFF
ldi     r25, 0x00		;; r25 = 0x00
ret


; Referenced from offset 0x2dc by brcs
Label33:
tst     r22
brne    Label34			;; r22 != 0x00
cpse    r20, r1			;; Skip if r20 == 0x00

; Referenced from offset 0x2e6 by brne
Label34:
ori     r24, 0x40
out     PORTA, r24		;; Select GB ROM Address hi8
rcall   Read_GEC		;; Read r24 from GEC
clr     r25				;; r25 = 0x00
ret


; Referenced from offset 0xb18 by rcall
 ;; Read GB SRAM data
 ;; r24		GB Address lo8
 ;; r25		GB Address hi8 (range 0x00 - 0x1F)
Read_SRAM_Data:
out     PORTB, r24		;; Select GB SRAM Address lo8
mov     r24, r25
clr     r25
cpi     r24, 0x20
brcs    Label35			;; r24 < 0x20
ser     r24				;; r24 = 0xFF
ldi     r25, 0x00		;; r25 = 0x00
ret


; Referenced from offset 0x2fc by brcs
Label35:
ori     r24, 0xa0
out     PORTA, r24		;; Select GB SRAM Address hi8
ldi     r24, 0x02

; Referenced from offset 0x30c by brne
Label36:
dec     r24				;; Wait 2 cycles
brne    Label36
cbi     PORTD, 5		;; D5 <= 0 (/CS)
rcall   Read_GEC		;; Read r24 from GEC
sbi     PORTD, 5		;; D5 <= 1 (/CS)
clr     r25				;; r25 = 0x00
ret


; Referenced from offset 0x974 by rcall
; Referenced from offset 0xc20 by rcall
 ;; Write GB SRAM data
 ;; r22		GB Address lo8
 ;; r23		GB Address hi8 (range 0x00 - 0x1F)
 ;; r24		Data
 
Write_SRAM_Data:
mov     r18, r24
out     PORTB, r22
mov     r24, r23
clr     r25
cpi     r24, 0x20
brcc    Label38			;; r24 >= 0x20
ori     r24, 0xa0
out     PORTA, r24		;; Select GB SRAM Address hi8
ldi     r24, 0x02

; Referenced from offset 0x32c by brne
Label37:
dec     r24				;; Wait 2 cycles
brne    Label37
cbi     PORTD, 5		;; D5 <= 0 (/CS)
mov     r24, r18
rcall   Write_GEC		;; Write r24 to GEC
sbi     PORTD, 5		;; D5 <= 1 (/CS)

; Referenced from offset 0x322 by brcc
Label38:
ret


; Referenced from offset 0x4f6 by rcall
; Referenced from offset 0x5a8 by rcall
 ;; Verify Program Flash
 ;; 
 ;; r22		Data Written
 ;; r24		Option
 ;;
 ;; (0x00ED) - Verify Mode
 ;; 			00: Static Wait
 ;; 				Option	00: Wait 0xDC cycles
 ;; 						<>: Wait 0x65 * 0x09 * 15000 cycles
 ;; 			01: Static Wait
 ;; 				Option	00: Wait 0x6E cycles
 ;; 						<>: Wait 0x33 * 0x09 * 15000 cycles
 ;; 			02: Verify D7.
 ;; 				Option	00: With Timeout
 ;; 						<>: Without Timeout
 ;; 			03: Wait until subsequent D6 match.
 ;; 			<>: Like 0x00
 ;;
 ;; r18, r24, r25 overwritten
 
Verify_Program_Flash:
push    r28
mov     r18, r24
mov     r28, r22
lds     r24, 0x00ed		;; r24 <= (0x00ED)
clr     r25
cpi     r24, 0x02
cpc     r25, r1
breq    Label45			;; r25 r24 == 0x0002
cpi     r24, 0x03
cpc     r25, r1
brge    Label39			;; r25 r24 >= 0x0003
sbiw    r24, 0x01
breq    Label40			;; r25 r24 == 0x0001
rjmp    Label55			;; r25 r24 == 0x0000

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

; Referenced from offset 0x34e by brge
 ;; Case >= 0x0003
 
Label39:
sbiw    r24, 0x03
breq    Label52			;; Case 0x0003
rjmp    Label55

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
 
; Referenced from offset 0x352 by breq
 ;; Case == 0x0001
 
Label40:
tst     r18
breq    Label44			;; r18 == 0x00
 
 ;; r18 != 0x00
 
ldi     r28, 0x00		;; Wait 0x33 times

; Referenced from offset 0x376 by brcs
Label41:
ldi     r18, 0x09		;; Wait 0x09 times

; Referenced from offset 0x370 by rjmp
Label42:
ldi     r24, 0x98
ldi     r25, 0x3a

; Referenced from offset 0x36a by brne
Label43:
sbiw    r24, 0x01		;; Wait 15000 cycles
brne    Label43
subi    r18, 0x01		;; r18--
sbrs    r18, 7
rjmp    Label42			;; r18 >= 0
subi    r28, 0xff		;; r28++
cpi     r28, 0x33
brcs    Label41			;; r28 < 0x33
rjmp    Label61			;; Exit sub

; Referenced from offset 0x35e by breq

Label44:
ldi     r24, 0x6e		;; Wait 110 cycles
rjmp    Label60

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
 
; Referenced from offset 0x348 by breq

 ;; r25 r24 == 0x0002

Label45:
andi    r28, 0x80
tst     r18
breq    Label48			;; r18 == 0x00

; Referenced from offset 0x39a by brne
Label46:
ldi     r24, 0x04

; Referenced from offset 0x388 by brne
Label47:
dec     r24				;; Wait 4 cycles
brne    Label47
rcall   Read_GEC		;; Read r24 from GEC
clr     r25
andi    r24, 0x80
andi    r25, 0x00
mov     r18, r28
clr     r19
cp      r24, r18
cpc     r25, r19
brne    Label46			;; high bit unexpected
rjmp    Label61			;; Exit sub

; Referenced from offset 0x382 by breq
Label48:
rcall   Start_Timer0		;; Start Timer 0

; Referenced from offset 0x3bc by rjmp
Label49:
ldi     r24, 0x04

; Referenced from offset 0x3a4 by brne
Label50:
dec     r24				;; Wait 4 cycles
brne    Label50
rcall   Read_GEC		;; Read r24 from GEC
clr     r25
andi    r24, 0x80
andi    r25, 0x00
mov     r18, r28
clr     r19
cp      r24, r18
cpc     r25, r19
breq    Label51			;; high bit expected
in      r0, TIFR
sbrs    r0, 1			;; Skip if timeout
rjmp    Label49

; Referenced from offset 0x3b6 by breq
Label51:
rcall   Stop_Timer0		;; Stop timer
rjmp    Label61			;; Exit sub

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

; Referenced from offset 0x3e4 by brne
; Referenced from offset 0x358 by breq

 ;; r25 r24 == 0x0003

Label52:
ldi     r24, 0x04       ; 4

; Referenced from offset 0x3c6 by brne
Label53:
dec     r24				;; Wait 4 cycles
brne    Label53
rcall   Read_GEC		;; Read r24 from GEC
mov     r28, r24
andi    r28, 0x40		;; bit6
ldi     r24, 0x04

; Referenced from offset 0x3d2 by brne
Label54:
dec     r24				;; Wait 4 cycles
brne    Label54
rcall   Read_GEC		;; Read r24 from GEC
clr     r25
andi    r24, 0x40
andi    r25, 0x00
mov     r18, r28
clr     r19
cp      r24, r18
cpc     r25, r19
brne    Label52			;; Subsequent bit6 don't match
rjmp    Label61			;; Exit sub

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

; Referenced from offset 0x35a by rjmp
; Referenced from offset 0x354 by rjmp

 ;; r25 r24 == 0x0000

Label55:
tst     r18
breq    Label59			;; r18 == 0x00
ldi     r28, 0x00		;; Wait 0x65 times

; Referenced from offset 0x402 by brcs
Label56:
ldi     r18, 0x09		;; Wait 0x09 times

; Referenced from offset 0x3fc by rjmp
Label57:
ldi     r24, 0x98
ldi     r25, 0x3a

; Referenced from offset 0x3f6 by brne
Label58:
sbiw    r24, 0x01		;; Wait 15000 cycles
brne    Label58
subi    r18, 0x01		;; r18--
sbrs    r18, 7			;; r18 >= 0x00
rjmp    Label57
subi    r28, 0xff		;; r28++
cpi     r28, 0x65
brcs    Label56			;; r28 < 0x65
rjmp    Label61			;; Exit sub

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

; Referenced from offset 0x3ea by breq
Label59:
ldi     r24, 0xdc		;; Wait 0xDC cycles

; Referenced from offset 0x37c by rjmp
; Referenced from offset 0x40a by brne
Label60:
dec     r24
brne    Label60

; Referenced from offset 0x3c0 by rjmp
; Referenced from offset 0x3e6 by rjmp
; Referenced from offset 0x39c by rjmp
; Referenced from offset 0x404 by rjmp
; Referenced from offset 0x378 by rjmp
Label61:
pop     r28
ret


; Referenced from offset 0x96c by rcall
 ;; 
 ;; r18		Rom Bank lo8
 ;; r20		Rom Bank hi8
 ;; r22		GB Address lo8
 ;; r23		GB Address hi8
 ;; r24		Data
 ;;
 ;; r19, r21, r25 overwritten
 
Program_Flash:
push    r8
push    r9
push    r10
push    r11
push    r12
push    r13
push    r14
push    r15
push    r16
push    r17
push    r28
push    r29
mov     r8, r24
movw    r24, r22
mov     r10, r20
mov     r11, r18
mov     r9, r22
mov     r24, r25
clr     r25
mov     r12, r24
tst     r20
brne    Label62
tst     r18
breq    Label63

; Referenced from offset 0x43a by brne
Label62:
ldi     r24, 0x40
or      r12, r24		;; Make sure all Rom Banks != 0x00 are in range 0x4000 - 0x7FFF

; Referenced from offset 0x43e by breq
Label63:
ldi     r22, 0x01
ldi     r24, 0x00
rcall   Switch_ROM_Bank	;; Switch to ROM bank 0x001
ldi     r28, 0x00
ldi     r29, 0x00		;; counter Y = 0x0000
ldi     r22, 0x06		;; byte count 0x09
mov     r13, r22

; Referenced from offset 0x48c by rjmp
 ;; Write Type 0x00 or 0x01 using AIN
 ;; Write Type 0x01 using /WR
 
Label64:
lds     r25, 0x00ec
ldi     r24, 0x09
mul     r25, r24
movw    r18, r0
clr     r1
add     r18, r28
adc     r19, r29
movw    r24, r18
subi    r24, 0x26
sbci    r25, 0xff
movw    r30, r24		;; Z = 0x00DA + 0x09 * type + counter
lpm     r24, Z
out     PORTA, r24
movw    r24, r18
subi    r24, 0x25
sbci    r25, 0xff
movw    r30, r24		;; Z = 0x00DB + 0x09 * type + counter
lpm     r24, Z
out     PORTB, r24
subi    r18, 0x24
sbci    r19, 0xff
movw    r30, r18		;; Z = 0x00DC + 0x09 * type + counter
lpm     r24, Z
rcall   Write_Flash_AIN
ldi     r31, 0xfd
add     r13, r31		;; r13 -= 0x03
adiw    r28, 0x03		;; counter += 0x0003
sbrs    r13, 7
rjmp    Label64			;; r13 >= 0x00
mov     r22, r11
mov     r24, r10
rcall   Switch_ROM_Bank	;; Switch to destination RB
out     PORTA, r12
out     PORTB, r9		;; Select destination GB Address
mov     r24, r8
rcall   Write_Flash_AIN	;; Write data
ldi     r28, 0xe5
ldi     r29, 0x00		;; Y = 0x00E5
movw    r14, r28
sec
sbc     r14, r1
sbc     r15, r1			;; r15 r14 = Y - 0x0001 = 0x00E4
movw    r16, r28
subi    r16, 0x02
sbci    r17, 0x00		;; r17 r16 = Y - 0x0002 = 0x00E3
ldi     r21, 0x06		;; byte count 0x09
mov     r13, r21

; Referenced from offset 0x4da by rjmp
Label65:
movw    r30, r16
lpm     r24, Z
out     PORTA, r24
movw    r30, r14
lpm     r24, Z
out     PORTB, r24
movw    r30, r28
lpm     r24, Z
rcall   Write_GEC		;; Data at 0x00E3 is address range 0x0000 - 0x3FFF
ldi     r31, 0xfd
add     r13, r31		;; r13 -= 0x03
subi    r16, 0xfd
sbci    r17, 0xff		;; r17 r16 += 0x0003
ldi     r24, 0x03
ldi     r25, 0x00
add     r14, r24
adc     r15, r25		;; r15 r14 += 0x0003
add     r28, r24
adc     r29, r25		;; Y += 0x0003
sbrs    r13, 7
rjmp    Label65			;; r13 >= 0x00
out     PORTA, r12
out     PORTB, r9
mov     r24, r8
rcall   Write_GEC		;; Write r24 to GEC
ldi     r24, 0x00
rcall   Set_MBC1_Model	;; Select 16/8 model
mov     r22, r11
mov     r24, r10
rcall   Switch_ROM_Bank
out     PORTA, r12
out     PORTB, r9
mov     r22, r8
ldi     r24, 0x00
rcall   Verify_Program_Flash
pop     r29
pop     r28
pop     r17
pop     r16
pop     r15
pop     r14
pop     r13
pop     r12
pop     r11
pop     r10
pop     r9
pop     r8
ret


; Referenced from offset 0xc50 by rcall
 ;; Erase whole Flash Chip using all Methods available
 
Erase_Flash:
push    r11
push    r12
push    r13
push    r14
push    r15
push    r16
push    r17
push    r28
push    r29
ldi     r22, 0x01
ldi     r24, 0x00
rcall   Switch_ROM_Bank	;; Switch to ROM bank 0x001
ldi     r28, 0xca
ldi     r29, 0x00		;; Y = 0x00CA
movw    r14, r28
sec
sbc     r14, r1
sbc     r15, r1			;; r15 r14 = Y - 0x0001 = 0x00C9
movw    r16, r28
subi    r16, 0x02
sbci    r17, 0x00		;; r17 r16 = Y - 0x0002 = 0x00C8
clr     r12
clr     r13				;; counter r13 r12 = 0x0000
ldi     r30, 0x0f		;; byte count 0x12
mov     r11, r30

; Referenced from offset 0x5a2 by rjmp
 ;; Write Type 0x00 or 0x01 using AIN
 ;; Write Type 0x01 using /WR
 
Label66:
lds     r25, 0x00ec
ldi     r24, 0x12
mul     r25, r24		;; r1 r0 = (0x00EC) * 0x12
movw    r18, r0
clr     r1
add     r18, r12
adc     r19, r13
movw    r24, r18
subi    r24, 0x4a
sbci    r25, 0xff		;; r25 r24 -= 0xFF4A
movw    r30, r24		;; Z = 0x00B6 + 0x12 * type + counter
lpm     r24, Z			;; r24 = Program(Z)
out     PORTA, r24		;; Select GB Address hi8
movw    r24, r18
subi    r24, 0x49
sbci    r25, 0xff		;; r25 r24 -= 0xFF49
movw    r30, r24		;; Z = 0x00B7 + 0x12 * type + counter
lpm     r24, Z			;; r24 = Program(Z)
out     PORTB, r24		;; Select GB Address lo8
subi    r18, 0x48
sbci    r19, 0xff		;; r19 r18 -= 0xFF48
movw    r30, r18		;; Z = 0x00B8 + 0x12 * type + counter
lpm     r24, Z
rcall   Write_Flash_AIN	;; Write r24 to Flash
movw    r30, r16
lpm     r24, Z
out     PORTA, r24
movw    r30, r14
lpm     r24, Z
out     PORTB, r24
movw    r30, r28
lpm     r24, Z
rcall   Write_GEC		;; Write r24 to GEC
ldi     r31, 0xfd
add     r11, r31		;; r11 -= 0x03
ldi     r24, 0x03
ldi     r25, 0x00
add     r12, r24
adc     r13, r25		;; r13 r12 += 0x0003
add     r16, r24
adc     r17, r25		;; r17 r16 += 0x0003
add     r14, r24
adc     r15, r25		;; r15 r14 += 0x0003
add     r28, r24
adc     r29, r25		;; Y += 0x0003
sbrs    r11, 7
rjmp    Label66			;; r11 >= 0x00
ser     r22
ldi     r24, 0x01       ; 1
rcall   Verify_Program_Flash
pop     r29
pop     r28
pop     r17
pop     r16
pop     r15
pop     r14
pop     r13
pop     r12
pop     r11
ret


; Referenced from offset 0xcee by rcall
Enter_Product_ID_Mode:
push    r11
push    r12
push    r13
push    r14
push    r15
push    r16
push    r17
push    r28
push    r29
ldi     r22, 0x01
ldi     r24, 0x00
rcall   Switch_ROM_Bank	;; Switch to ROM bank 0x001
ldi     r28, 0xaf
ldi     r29, 0x00		;; Y = 0x00AF
movw    r14, r28
sec
sbc     r14, r1
sbc     r15, r1			;; r15 r14 = Y - 0x0001 = 0x00AE
movw    r16, r28
subi    r16, 0x02
sbci    r17, 0x00		;; r17 r16 = Y - 0x0002 = 0x00AD
clr     r12
clr     r13				;; counter
ldi     r31, 0x06		;; byte count 0x09
mov     r11, r31

; Referenced from offset 0x64e by rjmp
 ;; Write Type 0x00 or 0x01 using AIN
 ;; Write Type 0x01 using /WR
 
Label67:
lds     r25, 0x00ec
ldi     r24, 0x09
mul     r25, r24
movw    r18, r0
clr     r1
add     r18, r12
adc     r19, r13
movw    r24, r18
subi    r24, 0x5c
sbci    r25, 0xff
movw    r30, r24		;; Z = 0x00A4 + 0x09 * type + counter
lpm     r24, Z
out     PORTA, r24
movw    r24, r18
subi    r24, 0x5b
sbci    r25, 0xff
movw    r30, r24		;; Z = 0x00A5 + 0x09 * type + counter
lpm     r24, Z
out     PORTB, r24
subi    r18, 0x5a
sbci    r19, 0xff
movw    r30, r18		;; Z = 0x00A6 + 0x09 * type + counter
lpm     r24, Z
rcall   Write_Flash_AIN
movw    r30, r16
lpm     r24, Z
out     PORTA, r24
movw    r30, r14
lpm     r24, Z
out     PORTB, r24
movw    r30, r28
lpm     r24, Z
rcall   Write_GEC
ldi     r31, 0xfd
add     r11, r31		;; r11 -= 0x03
ldi     r24, 0x03
ldi     r25, 0x00
add     r12, r24
adc     r13, r25		;; r13 r12 += 0x0003
add     r16, r24
adc     r17, r25		;; r17 r16 += 0x0003
add     r14, r24
adc     r15, r25		;; r15 r14 += 0x0003
add     r28, r24
adc     r29, r25		;; Y += 0x0003
sbrs    r11, 7
rjmp    Label67			;; r11 >= 0x00
pop     r29
pop     r28
pop     r17
pop     r16
pop     r15
pop     r14
pop     r13
pop     r12
pop     r11
ret


; Referenced from offset 0xd20 by rcall
Exit_Product_ID_Mode:
push    r11
push    r12
push    r13
push    r14
push    r15
push    r16
push    r17
push    r28
push    r29
ldi     r28, 0x9d
ldi     r29, 0x00		;; Y = 0x009D
movw    r14, r28
sec
sbc     r14, r1
sbc     r15, r1			;; r15 r14 = Y - 0x0001 = 0x009C
movw    r16, r28
subi    r16, 0x02
sbci    r17, 0x00		;; r17 r16 = Y - 0x0002 = 0x009B
clr     r12
clr     r13				;; counter
ldi     r26, 0x06		;; byte count 0x09
mov     r11, r26

; Referenced from offset 0x6ee by rjmp
 ;; Write Type 0x00 or 0x01 using AIN
 ;; Write Type 0x01 using /WR
 
Label68:
lds     r25, 0x00ec
ldi     r24, 0x09
mul     r25, r24
movw    r18, r0
clr     r1
add     r18, r12
adc     r19, r13
movw    r24, r18
subi    r24, 0x6e
sbci    r25, 0xff
movw    r30, r24		;; Z = 0x0092 + 0x09 * type + counter
lpm     r24, Z
out     PORTA, r24
movw    r24, r18
subi    r24, 0x6d
sbci    r25, 0xff
movw    r30, r24		;; Z = 0x0093 + 0x09 * type + counter
lpm     r24, Z
out     PORTB, r24
subi    r18, 0x6c
sbci    r19, 0xff
movw    r30, r18		;; Z = 0x0094 + 0x09 * type + counter
lpm     r24, Z
rcall   Write_Flash_AIN
movw    r30, r16
lpm     r24, Z
out     PORTA, r24
movw    r30, r14
lpm     r24, Z
out     PORTB, r24
movw    r30, r28
lpm     r24, Z
rcall   Write_GEC
ldi     r31, 0xfd
add     r11, r31		;; r11 -= 0x03
ldi     r24, 0x03
ldi     r25, 0x00
add     r12, r24
adc     r13, r25		;; r13 r12 += 0x0003
add     r16, r24
adc     r17, r25		;; r17 r16 += 0x0003
add     r14, r24
adc     r15, r25		;; r15 r14 += 0x0003
add     r28, r24
adc     r29, r25		;; Y += 0x0003
sbrs    r11, 7
rjmp    Label68			;; r11 >= 0x00
rcall   Send_SRAM_Disable
pop     r29
pop     r28
pop     r17
pop     r16
pop     r15
pop     r14
pop     r13
pop     r12
pop     r11
ret


; Referenced from offset 0xc8a by rcall
; Referenced from offset 0xad4 by rcall
 ;; Clear 0x47 bytes at 0x00A4
 ;; This leaves the command byte at 0x00A3.
 
Clear_Frame:
ldi     r30, 0xa4
ldi     r31, 0x00		;; Z = 0x00A4
ldi     r24, 0x46		;; byte count 0x47

; Referenced from offset 0x712 by rjmp
Label69:
st      Z+, r1
subi    r24, 0x01		;; r24--
sbrs    r24, 7
rjmp    Label69			;; r24 >= 0
ldi     r24, 0x55
sts     0x00a3, r24		;; (0x00A3) <= 0x55
ret


; Referenced from offset 0x794 by rcall
; Referenced from offset 0x78e by rcall
 ;; Add to CRC
 ;; r24		lo nibble is added to CRC
 ;; 		hi nibble must not be set

Add_CRC:
lds     r25, 0x00f2		;; r25 <= (0x00F2)
mov     r19, r25
swap    r19
cbr     r19, 0xf0		;; r19 = hi nibble
eor     r19, r24
mov     r24, r25
clr     r25
ldi     r27, 0x04

; Referenced from offset 0x734 by brne
Label70:
lsl     r24
rol     r25
dec     r27				;; X = (0x00F2) << 4
brne    Label70
lds     r20, 0x0060
mov     r18, r20
swap    r18
cbr     r18, 0xf0		;; r18 = hi nibble
or      r18, r24
mov     r22, r19
clr     r23
lsl     r22
rol     r23				;; r23 r22 << 1
movw    r24, r22
subi    r24, 0xbe
sbci    r25, 0xff
movw    r30, r24		;; Z = 0x0042
lpm     r24, Z
eor     r18, r24
sts     0x00f2, r18
mov     r24, r20
swap    r24
cbr     r24, 0x0f       ; 15
sts     0x0060, r24
subi    r22, 0xde
sbci    r23, 0xff
movw    r30, r22		;; Z = 0x0022 + r23 r22
lpm     r25, Z
eor     r24, r25
sts     0x0060, r24
ret


; Referenced from offset 0x7cc by rcall
; Referenced from offset 0x7a8 by rcall
 ;; Compute frame CRC for 0x46 bytes (i.e. all bytes)
 
Compute_CRC:
push    r17
push    r28
push    r29
sts     0x00f2, r1		;; Clear (0x00F2)
sts     0x0060, r1		;; Clear (0x0060)
ldi     r28, 0xa3
ldi     r29, 0x00		;; Y = 0x00A3
ldi     r17, 0x45		;; byte count 0x46

; Referenced from offset 0x79a by rjmp
Label71:
ld      r24, Y
swap    r24
cbr     r24, 0xf0		;; Select hi nibble
rcall   Add_CRC
ld      r24, Y+
cbr     r24, 0xf0		;; Select lo nibble
rcall   Add_CRC
subi    r17, 0x01		;; r17--
sbrs    r17, 7
rjmp    Label71			;; r17 >= 0x00
pop     r29
pop     r28
pop     r17
ret


; Referenced from offset 0x816 by rcall
; Referenced from offset 0xdd6 by rcall
 ;; Check CRC
 ;; r24		00: CRC Matched
 ;; 		01: CRC Failed
 ;; r25 is overwritten
Check_CRC:
push    r28
ldi     r28, 0x01		;; Return false
rcall   Compute_CRC
lds     r25, 0x00ea		;; r25 <= (0x00EA)
lds     r24, 0x0060		;; r24 <= (0x0060)
cp      r25, r24
brne    Label72			;; (0x0060) != (0x00EA)
lds     r25, 0x00e9		;; r25 <= (0x00E9)
lds     r24, 0x00f2		;; r24 <= (0x00F2)
cp      r25, r24
brne    Label72			;; (0x00E9) != (0x00F2)
ldi     r28, 0x00		;; Return true

; Referenced from offset 0x7b4 by brne
; Referenced from offset 0x7c0 by brne
Label72:
mov     r24, r28
clr     r25
pop     r28
ret


; Referenced from offset 0x9e0 by rcall
; Referenced from offset 0xd34 by rcall
 ;; Create CRC for sending frame back.
 
Create_CRC:
rcall   Compute_CRC
lds     r24, 0x0060
sts     0x00ea, r24
lds     r24, 0x00f2
sts     0x00e9, r24
ret


; Referenced from offset 0x92e by rcall
 ;; r20		GB Bank lo8
 ;; r22		GB Bank hi8
 ;; r24		Packet Counter
 ;; r18		00: RAM
 ;; 		01: ROM
 
Wait_Frame:
push    r13
push    r14
push    r15
push    r16
push    r17
push    r28
mov     r16, r24
mov     r14, r22
mov     r13, r20
mov     r15, r18
ldi     r28, 0x00		;; Packet Index Mismatch Counter
mov     r17, r28		;; CRC Fail counter

; Referenced from offset 0x87e by rjmp
Label73:
rcall   Start_Timer1

; Referenced from offset 0x806 by rjmp
 ;; Wait for complete frame

Label74:
lds     r24, 0x0061
cpi     r24, 0x48
brcc    Label75			;; r24 >= 0x48
in      r0, TIFR
sbrs    r0, 6
rjmp    Label74			;; Timer 1 Output Compate A Match
rcall   Stop_Timer1
rjmp    Label84

; Referenced from offset 0x800 by brcc
 ;; Frame complete
 
Label75:
rcall   Stop_Timer1
lds     r24, 0x00a3
cpi     r24, 0x55
brne    Label76			;; (0x00A3) != DATA
rcall   Check_CRC
tst     r24
breq    Label77			;; CRC Pass

; Referenced from offset 0x814 by brne
 ;; CRC Fail at max 0x0A times

Label76:
subi    r17, 0xff		;; r17++
cpi     r17, 0x0a
brcc    Label83			;; r17 >= 0x0A
sts     0x0061, r1		;; Clear Input_Cnt
ldi     r24, 0xf0		;; Send NAK
rjmp    Label82

; Referenced from offset 0x81a by breq
 ;; CRC Pass
Label77:
lds     r24, 0x00a4
cpi     r24, 0x01
breq    Label78			;; NORMAL_DATA
lds     r24, 0x00a4
cpi     r24, 0x02
brne    Label83			;; (0x00a4) != LAST_DATA

; Referenced from offset 0x830 by breq
Label78:
lds     r24, 0x00a6
cp      r24, r16
brne    Label79			;; Packet Counter Mismatch
lds     r24, 0x00a7
cp      r24, r14
brne    Label83			;; Bank Mismatch
lds     r24, 0x00a8
cp      r24, r13
brne    Label83			;; Bank Mismatch
ldi     r24, 0x00
ldi     r25, 0x00		;; r25 r24 = 0x0000 (OK)
rjmp    Label85

; Referenced from offset 0x840 by brne
 ;; Packet Counter Mismatch

Label79:
mov     r24, r16
subi    r24, 0x01		;; r24 = Expected Packet Count - 0x01
tst     r15
breq    Label80			;; RAM
mov     r25, r24
rjmp    Label81

; Referenced from offset 0x85e by breq
Label80:
mov     r25, r24
cbr     r25, 0x80		;; r25 = (Expected Packet Count - 1) & 0x7F

; Referenced from offset 0x862 by rjmp
Label81:
lds     r24, 0x00a6
cp      r24, r25
brne    Label83			;; Packet Counter Still Mismatch
subi    r28, 0xff		;; r28++
cpi     r28, 0x0a
brcc    Label83			;; r28 >= 0x0A
sts     0x0061, r1		;; Reset Input_Cnt
ldi     r24, 0xaa		;; Send ACK

; Referenced from offset 0x828 by rjmp
Label82:
rcall   Write_USART
rjmp    Label73

; Referenced from offset 0x86e by brne
; Referenced from offset 0x874 by brcc
; Referenced from offset 0x838 by brne
; Referenced from offset 0x850 by brne
; Referenced from offset 0x848 by brne
; Referenced from offset 0x820 by brcc
 ;; Bank Mismatch

Label83:
ldi     r24, 0x0f
rcall   Write_USART		;; Send END

; Referenced from offset 0x80a by rjmp
Label84:
ldi     r24, 0x01
ldi     r25, 0x00		;; X = 0x0001 (Exit)

; Referenced from offset 0x856 by rjmp
Label85:
pop     r28
pop     r17
pop     r16
pop     r15
pop     r14
pop     r13
ret


; Referenced from offset 0xbe2 by rcall
 ;; Write data to either ROM or RAM.
 ;; r24		00: RAM
 ;; 		01: ROM
 
Write_ROM_RAM:
push    r2
push    r3
push    r4
push    r5
push    r6
push    r7
push    r8
push    r9
push    r10
push    r11
push    r12
push    r13
push    r14
push    r15
push    r16
push    r17
push    r28
push    r29
mov     r16, r24		;; Mode
rcall   Reset_MBC
tst     r16
breq    Label86			;; RAM
 
 ;; Case ROM
 
ldi     r24, 0x00
rcall   Set_MBC1_Model	;; 16/8
rjmp    Label87

; Referenced from offset 0x8c0 by breq
 ;; Case RAM
 
Label86:
ldi     r24, 0x01
rcall   Set_MBC1_Model	;; 4/32
rcall   Send_SRAM_Enable

; Referenced from offset 0x8c6 by rjmp
Label87:
lds     r24, 0x00ee
clr     r25				;; r25 r24 = 00 (0x00EE)
mov     r13, r24
clr     r12
lds     r24, 0x00ef
add     r12, r24
adc     r13, r1			;; r13 r12 = (0x00EE : 0x00EF)		NumBanks
ser     r24
cp      r12, r24
ser     r24
cpc     r13, r24
brne    Label88			;; r13 r12 != 0xFFFF
ldi     r25, 0xfe
mov     r12, r25
ser     r25
mov     r13, r25		;; r13 r12 = 0xFFFE

; Referenced from offset 0x8e8 by brne
Label88:
tst     r16
breq    Label89			;; RAM
 
 ;; Case ROM
 
ser     r24
mov     r2, r24
rjmp    Label90			;; Write Packet Count (0x100 * 0x40 = 0x4000)

; Referenced from offset 0x8f4 by breq
 ;; Case RAM
Label89:
ldi     r17, 0x7f
mov     r2, r17			;; Write Packet Count (0x80 * 0x40 = 0x2000)

; Referenced from offset 0x8fa by rjmp
Label90:
sts     0x00f0, r1		;; Clear (0x00F0) Flag
sts     0x0061, r1		;; Clear (0x0061) Input_Cnt
ldi     r24, 0xaa
rcall   Write_USART		;; Write ACK
clr     r8				;; RAM Bank/ ROM Bank lo8
clr     r9				;; GB ROM Bank hi8

; Referenced from offset 0x9ac by rjmp
Label91:
mov     r3, r8
mov     r24, r9
clr     r25
mov     r4, r24			;; r4 r3 = GB ROM Bank (RAM Bank)
tst     r16
brne    Label92			;; ROM
 
 ;; Case RAM
 
mov     r24, r8			;; GB RAM Bank
rcall   Switch_SRAM_Bank

; Referenced from offset 0x91a by brne
Label92:
clr     r10
clr     r11				;; Packet-per-Bank counter
movw    r6, r10			;; GB Address

; Referenced from offset 0x99e by brcc
Label93:
mov     r18, r16		;; ROM/RAM
mov     r20, r3			;; RAM Bank/ ROM Bank lo8
mov     r22, r4			;; ROM Bank hi8
mov     r24, r10		;; Packet-per-Bank counter
rcall   Wait_Frame
tst     r24
brne    Label98			;; Exit
ldi     r30, 0xa3
ldi     r31, 0x00		;; Z = 0x00A3
ldi     r26, 0x63
ldi     r27, 0x00		;; X = 0x0063
ldi     r17, 0x3f		;; byte count = 0x40

; Referenced from offset 0x948 by rjmp
 ;; Copy 0x40 bytes from 0x00A9 to 0x0063
 
Label94:
ldd     r24, Z+6
st      X+, r24
subi    r17, 0x01		;; r17--
adiw    r30, 0x01		;; Z += 0x0001
sbrs    r17, 7
rjmp    Label94			;; r17 >= 0x00
 
 ;; Process 0x40 bytes from 0x0063
 
lds     r5, 0x00a4		;; r5 = Packet Type
sts     0x0061, r1		;; Clear (0x0061)	Input_Cnt
ldi     r24, 0xaa
rcall   Write_USART		;; Write ACK
ldi     r28, 0x63
ldi     r29, 0x00		;; Y = 0x0063
movw    r14, r28
ldi     r17, 0x3f		;; byte count 0x40

; Referenced from offset 0x988 by rjmp
Label95:
tst     r16
breq    Label96			;; RAM
 
 ;; Case ROM
 
mov     r18, r3			;; GB Bank lo8
mov     r20, r4			;; GB Bank hi8
movw    r22, r6			;; GB Address
movw    r30, r14
ld      r24, Z			;; Data
rcall   Program_Flash
rjmp    Label97

; Referenced from offset 0x960 by breq
 ;; Case RAM
 
Label96:
movw    r22, r6			;; GB Address
ld      r24, Y			;; Data
rcall   Write_SRAM_Data

; Referenced from offset 0x96e by rjmp
Label97:
sec
adc     r6, r1
adc     r7, r1			;; GB Address++
subi    r17, 0x01		;; r17--
sec
adc     r14, r1
adc     r15, r1			;; Read Address++
adiw    r28, 0x01		;; Y += 0x0001
sbrs    r17, 7
rjmp    Label95			;; r17 >= 0x00
ldi     r31, 0x02
cp      r5, r31
breq    Label98			;; LAST_DATA
sec
adc     r10, r1
adc     r11, r1			;; Packet-per-Bank counter++
mov     r24, r2
clr     r25
cp      r24, r10
cpc     r25, r11
brcc    Label93			;; Packet-per-Bank counter <= Write Packet count
sec
adc     r8, r1
adc     r9, r1			;; GB Bank++
cp      r12, r8
cpc     r13, r9
brcs    Label98			;; GB Bank > NumBanks
rjmp    Label91

; Referenced from offset 0x932 by brne
; Referenced from offset 0x98e by breq
; Referenced from offset 0x9aa by brcs
Label98:
rcall   Send_SRAM_Disable
pop     r29
pop     r28
pop     r17
pop     r16
pop     r15
pop     r14
pop     r13
pop     r12
pop     r11
pop     r10
pop     r9
pop     r8
pop     r7
pop     r6
pop     r5
pop     r4
pop     r3
pop     r2
ret


; Referenced from offset 0xb34 by rcall
 ;; Create Frame

Send_Frame:
push    r15
push    r16
push    r17
push    r28
clr     r15				;; Retry counter
rcall   Create_CRC
sts     0x00f1, r15		;; Enable Receive Flag

; Referenced from offset 0xa3a by rjmp
 ;; Send Frame
Label99:
ldi     r16, 0xa3
ldi     r17, 0x00		;; r17 r16 = 0x00A3
ldi     r28, 0x47		;; byte count 0x48

; Referenced from offset 0x9f8 by rjmp
 ;; 
Label100:
movw    r30, r16
ld      r24, Z+
movw    r16, r30
rcall   Write_USART		;; Write Frame byte
subi    r28, 0x01		;; r28--
sbrs    r28, 7
rjmp    Label100		;; r28 >= 0x00
rcall   Start_Timer1

; Referenced from offset 0xa08 by rjmp
Label101:
lds     r24, 0x00f1     ;; Check Receive Flag
tst     r24
brne    Label102		;; Frame was received at other end
in      r0, TIFR
sbrs    r0, 6
rjmp    Label101		;; Not Timer 1 Output A Match
rcall   Stop_Timer1
rjmp    Label104

; Referenced from offset 0xa02 by brne
 ;; Other end received frame.

Label102:
rcall   Stop_Timer1
lds     r24, 0x0062
clr     r25
cpi     r24, 0x0f
cpc     r25, r1
breq    Label104		;; Receive_Status == END
cpi     r24, 0xaa
cpc     r25, r1
brne    Label103		;; Receive_Status != ACK
ldi     r24, 0x00
ldi     r25, 0x00		;; r25 r24 = 0x0000	(OK)
rjmp    Label106

; Referenced from offset 0xa20 by brne
Label103:
inc     r15				;; Retry Counter++
ldi     r31, 0x09
cp      r31, r15
brcc    Label105		;; Rety Counter <= 0x09

; Referenced from offset 0xa0c by rjmp
; Referenced from offset 0xa1a by breq
Label104:
ldi     r24, 0x01
ldi     r25, 0x00		;; r25 r24 = 0x0001	(TIMEOUT)
rjmp    Label106

; Referenced from offset 0xa2e by brcc
 ;; Try Resend
Label105:
sts     0x00f1, r1		;; Enable Receive Flag
rjmp    Label99

; Referenced from offset 0xa34 by rjmp
; Referenced from offset 0xa26 by rjmp
Label106:
pop     r28
pop     r17
pop     r16
pop     r15
ret


; Referenced from offset 0xbd8 by rcall
 ;; r24		00: RAM
 ;; 		01: ROM
Read_ROM_RAM:
push    r3
push    r4
push    r5
push    r6
push    r7
push    r8
push    r9
push    r10
push    r11
push    r12
push    r13
push    r14
push    r15
push    r16
push    r17
push    r28
push    r29
mov     r6, r24
rcall   Reset_MBC
tst     r6
breq    Label107		;; RAM
 
 ;; Case ROM
 
ldi     r24, 0x00
rcall   Set_MBC1_Model	;; 16/8
rjmp    Label108

; Referenced from offset 0xa6e by breq
 ;; Case RAM
 
Label107:
ldi     r24, 0x01
rcall   Set_MBC1_Model	;; 4/32
rcall   Send_SRAM_Enable

; Referenced from offset 0xa74 by rjmp
Label108:
lds     r24, 0x00ee
clr     r25
mov     r15, r24
clr     r14
lds     r24, 0x00ef
add     r14, r24
adc     r15, r1			;; r15 r14 = NumBank
ser     r24
cp      r14, r24
ser     r24
cpc     r15, r24
brne    Label109		;; NumBanks != 0xFFFF
ldi     r23, 0xfe
mov     r14, r23
ser     r23
mov     r15, r23		;; NumBanks = 0xFFFE

; Referenced from offset 0xa96 by brne
Label109:
tst     r6
breq    Label110		;; RAM
 
 ;; Case ROM
 
ser     r22
mov     r5, r22			;; r5 = Packets-per-Bank (0x100 * 0x40 = 0x4000)
rjmp    Label111

; Referenced from offset 0xaa2 by breq
 ;; Case RAM
Label110:
ldi     r21, 0x7f
mov     r5, r21			;; r5 = Packets-per-Bank (0x80 * 0x40 = 0x2000)

; Referenced from offset 0xaa8 by rjmp
Label111:
ldi     r24, 0x01
sts     0x00f0, r24		;; Enable Receive_Status Flag
ldi     r28, 0x00
ldi     r29, 0x00		;; Y = 0x0000

; Referenced from offset 0xb58 by rjmp
Label112:
mov     r3, r28
mov     r24, r29		;; GB Bank hi8
clr     r25
mov     r4, r24			;; r4 r3 = GB Bank
tst     r6
breq    Label113		;; RAM
 
 ;; Case ROM
 
mov     r22, r28		;; GB Bank lo8
rcall   Switch_ROM_Bank
rjmp    Label114

; Referenced from offset 0xac2 by breq
 ;; Case RAM
 
Label113:
mov     r24, r28		;; GB RAM Bank
rcall   Switch_SRAM_Bank

; Referenced from offset 0xac8 by rjmp
Label114:
clr     r10
clr     r11				;; Packets-per-Bank counter
movw    r8, r10			;; GB Address

; Referenced from offset 0xb4e by brcc
Label115:
rcall   Clear_Frame
cp      r28, r14
cpc     r29, r15
brne    Label116		;; GB Bank != NumBanks
mov     r24, r5
clr     r25
cp      r10, r24
cpc     r11, r25
brne    Label116		;; Packets-per-Bank counter != Packet number
ldi     r24, 0x02		;; LAST_DATA
rjmp    Label117

; Referenced from offset 0xae4 by brne
; Referenced from offset 0xada by brne
Label116:
ldi     r24, 0x01		;; NORMAL_DATA

; Referenced from offset 0xae8 by rjmp
Label117:
sts     0x00a4, r24		;; Packet Type
sts     0x00a6, r10		;; Packet Index
sts     0x00a7, r4
sts     0x00a8, r3		;; GB Bank
ldi     r16, 0xa3
ldi     r17, 0x00		;; r17 r16 = 0x00A3
movw    r12, r16
ldi     r19, 0x3f
mov     r7, r19			;; byte count 0x40

; Referenced from offset 0xb32 by rjmp
 ;; Read 0x40 bytes to Packet
 
Label118:
tst     r6
breq    Label119		;; RAM
 
 ;; Case ROM
 
mov     r20, r3
mov     r22, r4			;; GB Bank
movw    r24, r8			;; GB Address
rcall   Read_ROM_Data
movw    r30, r12
rjmp    Label120

; Referenced from offset 0xb08 by breq
Label119:
movw    r24, r8			;; GB Address
rcall   Read_SRAM_Data
movw    r30, r16

; Referenced from offset 0xb14 by rjmp
 ;; Copy byte to 0x00A9 + counter
Label120:
std     Z+6, r24
sec
adc     r8, r1
adc     r9, r1			;; GB Address++
dec     r7				;; r7--
sec
adc     r12, r1
adc     r13, r1			;; Packet0 Address++
subi    r16, 0xff
sbci    r17, 0xff		;; Packet1 Address++
sbrs    r7, 7
rjmp    Label118		;; r7 >= 0x00
rcall   Send_Frame
tst     r24
breq    Label121		;; OK
 
 ;; Case ERROR
 
tst     r6
brne    Label124		;; ROM
 
 ;; Case RAM
 
rjmp    Label123

; Referenced from offset 0xb38 by breq
 ;; Case OK

Label121:
sec
adc     r10, r1
adc     r11, r1			;; Packet Counter++
mov     r24, r5
clr     r25
cp      r24, r10
cpc     r25, r11
brcc    Label115		;; Packet Counter <= Packets-per-Bank
adiw    r28, 0x01		;; GB Bank++
cp      r14, r28
cpc     r15, r29
brcs    Label122		;; GB Bank >= NumBank
rjmp    Label112

; Referenced from offset 0xb56 by brcs
Label122:
tst     r6
brne    Label124		;; ROM

; Referenced from offset 0xb3e by rjmp
Label123:
rcall   Send_SRAM_Disable

; Referenced from offset 0xb3c by brne
; Referenced from offset 0xb5c by brne
Label124:
pop     r29
pop     r28
pop     r17
pop     r16
pop     r15
pop     r14
pop     r13
pop     r12
pop     r11
pop     r10
pop     r9
pop     r8
pop     r7
pop     r6
pop     r5
pop     r4
pop     r3
ret


; Referenced from offset 0xdfa by rcall
Manipulate_Data:
lds     r24, 0x00a6
sts     0x00eb, r24		;; (0x00EB) <= (0x00A6)		(MBC)
lds     r24, 0x00a7
andi    r24, 0x01
sts     0x00ec, r24		;; (0x00EC) <= 00/01		(Flash Algorithm)
lds     r24, 0x00a8
sts     0x00ed, r24		;; (0x00ED) <= Flash Type
lds     r24, 0x00a9
sts     0x00ee, r24		;; (0x00EE) <= (0x00A9)		(NumBanks hi8)
lds     r24, 0x00aa
sts     0x00ef, r24		;; (0x00EF) <= (0x00AA)		(NumBanks lo8)
lds     r24, 0x00a5
clr     r25
cpi     r24, 0x01
cpc     r25, r1
breq    Label127		;; (0x00A5) == RRAM
cpi     r24, 0x02
cpc     r25, r1
brge    Label125		;; (0x00A5) >= WROM
or      r24, r25
breq    Label126		;; (0x00A5) == RROM
rjmp    Label132		;; Invalid States

; Referenced from offset 0xbbe by brge
 ;; Case >= WROM

Label125:
cpi     r24, 0x02
cpc     r25, r1
breq    Label129		;; (0x00A5) == WROM
sbiw    r24, 0x03
breq    Label130		;; (0x00A5) == WRAM
rjmp    Label132		;; Invalid States

; Referenced from offset 0xbc2 by breq
 ;; Case RROM

Label126:
ldi     r24, 0x01		;; ROM
rjmp    Label128

; Referenced from offset 0xbb8 by breq
 ;; Case RRAM

Label127:
ldi     r24, 0x00		;; RAM

; Referenced from offset 0xbd4 by rjmp
Label128:
rcall   Read_ROM_RAM
ret


; Referenced from offset 0xbca by breq
 ;; Case WROM
 
Label129:
ldi     r24, 0x01		;; ROM
rjmp    Label131

; Referenced from offset 0xbce by breq
 ;; Case WRAM
 
Label130:
ldi     r24, 0x00		;; RAM

; Referenced from offset 0xbde by rjmp
Label131:
rcall   Write_ROM_RAM
ret


; Referenced from offset 0xbd0 by rjmp
; Referenced from offset 0xbc4 by rjmp
Label132:
ldi     r24, 0xf0
rcall   Write_USART		;; Write NAK
ret


; Referenced from offset 0xdfe by rcall
Erase_Data:
push    r16
push    r17
push    r28
push    r29
rcall   Reset_MBC
lds     r24, 0x00a6
sts     0x00eb, r24		;; (0x00EB) <= (0x00A6)		(MBC)
lds     r24, 0x00a5
tst     r24
breq    Label135		;; (0x00A5) == EFLA
 
 ;; Case 0x00 Erase SRAM	(ERAM)
 
ldi     r24, 0x01
rcall   Set_MBC1_Model	;; 4/32
rcall   Send_SRAM_Enable
ldi     r16, 0x00
ldi     r17, 0x00		;; SRAM Bank 0x00
lds     r24, 0x00a7

; Referenced from offset 0xc3a by brcc
Label133:
mov     r24, r16
rcall   Switch_SRAM_Bank
ldi     r28, 0x00
ldi     r29, 0x00		;; Y = 0x0000

; Referenced from offset 0xc2a by brcs
Label134:
movw    r22, r28
ldi     r24, 0x00
rcall   Write_SRAM_Data	;; Write 0x00
adiw    r28, 0x01		;; Y += 0x0001
ldi     r24, 0x20
cpi     r28, 0x00
cpc     r29, r24
brcs    Label134		;; Y < 0x2000
subi    r16, 0xff
sbci    r17, 0xff		;; SRAM Bank++
lds     r24, 0x00a7
clr     r25
cp      r24, r16
cpc     r25, r17
brcc    Label133		;; SRAM Bank <= (0x00A7)
rjmp    Label136

; Referenced from offset 0xc04 by breq
 ;; Case 0x01 Erase Flash	(EFLA)

Label135:
lds     r24, 0x00a7
andi    r24, 0x01
sts     0x00ec, r24		;; (0x00EC) <= 00/01	(Flash Algorithm)
lds     r24, 0x00a8
sts     0x00ed, r24		;; (0x00ED) <= Flash Type
rcall   Erase_Flash

; Referenced from offset 0xc3c by rjmp
Label136:
rcall   Send_SRAM_Disable
ldi     r24, 0xaa		;; Write ACK
rcall   Write_USART
pop     r29
pop     r28
pop     r17
pop     r16
ret


; Referenced from offset 0xe02 by rcall
Send_Status:
push    r15
push    r16
push    r17
push    r28
push    r29
ldi     r17, 0x00
lds     r24, 0x00a5
tst     r24
breq    Label137		;; (0x00A5) == 0x00 (NREAD_ID)
 
 ;; Case READ_ID
 
ldi     r17, 0x01
lds     r24, 0x00a6
sts     0x00eb, r24		;; (0x00EB) <= (0x00A6)		(MBC)
lds     r24, 0x00a7
and     r24, r17
sts     0x00ec, r24		;; (0x00EC) <= 00/01 		(Flash Algorithm)

; Referenced from offset 0xc74 by breq
Label137:
rcall   Clear_Frame
tst     r17
brne    Label138
rjmp    Label144

; Referenced from offset 0xc8e by brne
 ;; 
 
Label138:
rcall   Reset_MBC
ldi     r24, 0x01
sts     0x00ab, r24		;; (0x00AB) <= 0x01		(Logo Correct)
ldi     r28, 0x62
ldi     r29, 0x00		;; Y = 0x0062
ldi     r16, 0x04
ldi     r17, 0x01		;; GB Address 0x0104
ldi     r26, 0x2f		;; byte count 0x30
mov     r15, r26

; Referenced from offset 0xcc4 by rjmp
 ;; Transfer GB Header

Label139:
ldi     r20, 0x00
mov     r22, r20		;; GB Bank 0x000
movw    r24, r16		;; GB Address 0x0104
rcall   Read_ROM_Data	;; Read r24 from GEC
movw    r30, r28
lpm     r25, Z			;; Load Nintendo Char data
cp      r24, r25
breq    Label140
sts     0x00ab, r1		;; (0x00AB) <= 0x00		(Logo Incorrect)

; Referenced from offset 0xcb4 by breq
Label140:
dec     r15				;; Byte count--
subi    r16, 0xff
sbci    r17, 0xff		;; GB Address++
adiw    r28, 0x01		;; Nintendo Char Data++
sbrs    r15, 7
rjmp    Label139		;; byte count >= 0x00
ldi     r28, 0x34
ldi     r29, 0x01		;; GB Address Y = 0x0134
ldi     r16, 0xac
ldi     r17, 0x00		;; MCU RAM Address = 0x00AC		(GB Header)
ldi     r31, 0x1b
mov     r15, r31		;; byte count = 0x1C

; Referenced from offset 0xce6 by rjmp
 ;; Copy GB Header
 
Label141:
ldi     r20, 0x00
mov     r22, r20		;; GB Bank 0x000
movw    r24, r28
rcall   Read_ROM_Data	;; Read r24 from GEC
movw    r30, r16
st      Z+, r24			;; Store data
movw    r16, r30
dec     r15				;; byte count--
adiw    r28, 0x01		;; GB Address++
sbrs    r15, 7
rjmp    Label141		;; byte count >= 0x00
ldi     r24, 0x04

; Referenced from offset 0xcec by brne
 ;; Get Flash Chip Product Id
 
Label142:
dec     r24				;; Wait 4 cycles
brne    Label142
rcall   Enter_Product_ID_Mode
ldi     r24, 0xdc

; Referenced from offset 0xcf4 by brne
Label143:
dec     r24				;; Wait 0xDC cycles
brne    Label143
ldi     r20, 0x00
mov     r22, r20		;; GB Bank 0x000
ldi     r24, 0x00
ldi     r25, 0x00		;; GB Address 0x0000
rcall   Read_ROM_Data
sts     0x00a7, r24		;; (0x00A7) <= 00:0000		(Manufacturer ID)
ldi     r20, 0x00
mov     r22, r20		;; GB Bank 0x000
ldi     r24, 0x01
ldi     r25, 0x00		;; GB Address 0x0001
rcall   Read_ROM_Data
sts     0x00a8, r24		;; (0x00A8) <= 00:0001		(Device ID)
ldi     r20, 0x00
mov     r22, r20		;; GB Bank 0x000
ldi     r24, 0x02
ldi     r25, 0x00		;; GB Address 0x0002
rcall   Read_ROM_Data
sts     0x00a9, r24		;; (0x00A9) <= 00:0002		(Sector Group Protect)
rcall   Exit_Product_ID_Mode

; Referenced from offset 0xc90 by rjmp
 ;; Append Firmware Version

Label144:
ldi     r24, 0x04
sts     0x00a4, r24		;; (0x00A4) <= 0x04		(STATUS)
ldi     r24, 0x02
sts     0x00a5, r24		;; (0x00A5) <= 0x02		("02")
ldi     r24, 0x10
sts     0x00a6, r24		;; (0x00A6) <= 0x10		("10")
rcall   Create_CRC
ldi     r28, 0xa3
ldi     r29, 0x00		;; Y = 0x00A3
ldi     r17, 0x47		;; byte count 0x48

; Referenced from offset 0xd44 by rjmp
Label145:
ld      r24, Y+
rcall   Write_USART
subi    r17, 0x01		;; r17--
sbrs    r17, 7
rjmp    Label145		;; r17 >= 0x00
pop     r29
pop     r28
pop     r17
pop     r16
pop     r15
ret


; Referenced from offset 0x11e by rjmp
Port_Setup:
ldi     r28, 0x5f
ldi     r29, 0x02
out     SPH, r29
out     SPL, r28		; SP = 0x025F
out     PORTA, r1		; A <= 0x00
ser     r24
out     DDRA, r24		;; Port A is output
out     PORTB, r1		;; B <= 0x00
out     DDRB, r24		;; Port B is output
out     DDRC, r1		;; Port C is input
out     PORTC, r24		;; C <= 0xFF
out     PORTD, r24		;; D <= 0xFF
ldi     r24, 0xfe
out     DDRD, r24		;; Port D7-D1 is output, D0 is input (USART Rx)
ldi     r24, 0x05
out     PORTE, r24		;; D <= 0x05
ldi     r18, 0x02
out     DDRE, r18		;; Port E7-E2, E0 is input, E1 is output
nop
in      r24, PINE		;; r24 <= E
clr     r25				;; r25 = 0x00
andi    r24, 0x05       ;; r24 = E2 | E0
andi    r25, 0x00       ;; r25 = 0x00
sbiw    r24, 0x01       ;; r24 -= 0x0001
brne    Label147
 
 ;; Case E2 | E0 == 0x0001
 
out     UBRRL, r18		;; Baud Rate Register <= 0x02
rjmp    Label149

 ;; Case Else
 
Label147:
in      r24, PINE		;; r24 <= E
clr     r25				;; r25 = 0x00
andi    r24, 0x05       ;; r24 = E2 | E0
andi    r25, 0x00       ;; r25 = 0x00
sbiw    r24, 0x04       ;; r25 r24 -= 0x0004
brne    Label148
 
 ;; Case E2 | E0 == 0x0004
 
out     UBRRL, r1		;; Baud Rate Register <= 0x00
rjmp    Label149

 ;; Case Else

Label148:
ldi     r24, 0x01
out     UBRRL, r24		;; Baud Rate Register <= 0x01

; Referenced from offset 0xd86 by rjmp
; Referenced from offset 0xd96 by rjmp
Label149:
out     DDRE, r1		;; Port E is input
ldi     r24, 0x07
out     PORTE, r24		;; E <= 0x07
ldi     r24, 0x44
out     OCR1AH, r24
ldi     r24, 0xaa
out     OCR1AL, r24		;; Timer 1 Compare Register <= 0x44AA
ldi     r24, 0x80
out     ACSR, r24		;; Turn Analog Comparator Off
ldi     r24, 0x98
out     UCSRB, r24		;; USART <= RxInterruptEnable | RxEnable | TxEnable
sei						;; Set Interrupt Enabled

; Referenced from offset 0xdba by breq
; Referenced from offset 0xe14 by rjmp
; Referenced from offset 0xe1c by rjmp
;; Mainloop

Label150:
lds     r24, 0x0061		;; r24 <= (0x0061)
tst     r24				;; r24 == 0x00
breq    Label150
rcall   Start_Timer1

; Referenced from offset 0xdca by brcs
 ;; Wait for 0x48 (72) bytes to be received

Label151:
in      r0, TIFR
sbrc    r0, 6
rjmp    Label158		;; Timer Interrupt Output Compare A Match
lds     r24, 0x0061
cpi     r24, 0x48
brcs    Label151		;; (0x0061) < 0x48
rcall   Stop_Timer1
lds     r24, 0x00a3
cpi     r24, 0x55
brne    _Send_NAK		;; (0x00A3) != 0x55
 
 ;; Command 0x55 (DATA)
 
rcall   Check_CRC
tst     r24
brne    _Send_NAK		;; CRC failed
lds     r24, 0x00a4
clr     r25
cpi     r24, 0x03
cpc     r25, r1
breq    Label154		;; (0x00A4) == 0x03
cpi     r24, 0x04
cpc     r25, r1
brge    Label152		;; (0x00A4) >= 0x04
or      r24, r25
breq    Label153		;; (0x00A4) == 0x00
rjmp    _Send_NAK

; Referenced from offset 0xdec by brge
 ;; Case >= 0x04

Label152:
sbiw    r24, 0x04       ; 4
breq    Label155		;; (0x00A4) == 0x04
rjmp    _Send_NAK

; Referenced from offset 0xdf0 by breq
 ;; Case 0x00 (NORMAL_DATA)
 
Label153:
rcall   Manipulate_Data
rjmp    Label157

; Referenced from offset 0xde6 by breq
 ;; Case 0x03 (ERASE)

Label154:
rcall   Erase_Data
rjmp    Label157

; Referenced from offset 0xdf6 by breq
 ;; Case 0x04 (STATUS)
Label155:
rcall   Send_Status
rjmp    Label157

; Referenced from offset 0xdf2 by rjmp
; Referenced from offset 0xdd4 by brne
; Referenced from offset 0xdda by brne
; Referenced from offset 0xdf8 by rjmp
_Send_NAK:
ldi     r24, 0xf0		;; NAK
rcall   Write_USART		;; Send 0xF0 via USART

; Referenced from offset 0xe04 by rjmp
; Referenced from offset 0xdfc by rjmp
; Referenced from offset 0xe00 by rjmp
Label157:
sts     0x0061, r1		;; Clear (0x0061)
sts     0x00f0, r1		;; Clear (0x00F0)
rcall   Turn_LED_Off	;; Stop red LED
rjmp    Label150

; Referenced from offset 0xdc2 by rjmp
 ;; Timeout
Label158:
rcall   Stop_Timer1
sts     0x0061, r1		;; Reset Input Counter
rjmp    Label150
