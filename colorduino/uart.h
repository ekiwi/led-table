/*
 * uart.h
 *
 *  Created on: Apr 7, 2012
 *      Author: kevin
 */

#ifndef UART_H_
#define UART_H_

// UART Hardware Defines
#define UBRRn UBRR0
#define UCSRnA UCSR0A
#define UCSRnC UCSR0C
#define USBSn USBS0
#define UCSZn0 UCSZ00
#define RXENn RXEN0
#define TXENn TXEN0
#define UCSRnB UCSR0B
#define RXCIEn RXCIE0
#define TXCIEn TXCIE0
#define UDRIEn UDRIE0
#define RXCn RXC0
#define UDREn UDRE0
#define UDRn UDR0

// Header Defines
#define SETUP_HEADER 0x56
#define PICTURE_HEADER 0x89

// Setup Defines
#define SETUP_LENGTH 4
#define SETUP_SLEEP_PATTERN0 0x0B // 1011 (bit3:0)
#define SETUP_SLEEP_PATTERN1 0x5D //
#define SETUP_SLEEP_PATTERN2 0xA8 //
#define SETUP_SLEEP_PATTERN3 0x93 //


// Functions
#define d_out_high() PORTC |=  (1<<PC4)
#define d_out_low()  PORTC &= ~(1<<PC4)
#define p_out()		 (PINC & (1<<PC5))

void uart_init_(uint8_t baud);
uint8_t uart_async_run();
void uart_start_slot(uint8_t slot);
#define uart_init(baud) uart_init_((F_CPU/(16*baud))-1)
void uart_stop(void);

extern uint8_t sleep_flag;		// is > 0 if display is supposed to go to sleep
#endif /* UART_H_ */
