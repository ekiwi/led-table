/*
 * uart.h
 *
 *  Created on: Jul 12, 2011
 *      Author: kevin
 */

#ifndef UART_H_
#define UART_H_

// Includes
#include <avr/io.h>
#include "led_graphics.h"

// Settings
#define DEFAULT_ORIENTATION ROTATE_180

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

#define SETUP_LENGTH 4
#define SETUP_SLEEP_PATTERN0 0x0B // 1011 (bit3:0)
#define SETUP_SLEEP_PATTERN1 0x5D //
#define SETUP_SLEEP_PATTERN2 0xA8 //
#define SETUP_SLEEP_PATTERN3 0x93 //

#define p_out_high() PORTD |=  (1<<PD2)
#define p_out_low() PORTD &= ~(1<<PD2)
#define d_out()	  (PIND & (1<<PD3))

typedef struct{
		unsigned send_img : 1;
		unsigned send_setup : 1;
		unsigned : 6;
}uart_flags;

extern uart_flags uart;

typedef enum{
	ROTATE_00,
	ROTATE_90,
	ROTATE_180,
	ROTATE_270
} orientation_type;

void display_set_orientation(orientation_type o);
orientation_type display_get_orientation();
void display_change_orientation();

// UART Functions
uint8_t uart_async_run();
void uart_init_(uint8_t baud);
#define uart_init(baud) uart_init_((F_CPU/(16*baud))-1)

#define uart_img_send() {uart.send_img = 1;}

// Display Sleep
void display_sleep();

// White Balance
void display_wb_set(uint8_t red, uint8_t green, uint8_t blue);	// Sets the white balance to specific values (ATTENTION: does not update the display)
void display_wb_increase(uint8_t color_index);
void display_wb_decrease(uint8_t color_index);
uint8_t display_wb_get(uint8_t color_index);
void display_wb_update();		// Sends the white balance values to the display
void display_wb_save();		// Saves the white balance values to eeprom


#endif /* UART_H_ */
