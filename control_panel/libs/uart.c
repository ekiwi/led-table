/*
 * uart.c
 *
 *  Created on: Jul 12, 2011
 *      Author: kevin
 *
 *
 *      For 250kbps use MAX3232 instead of MAX232 out of spec
 */
#include "uart.h"
#include "interface.h"
#include <util/delay.h>
#include <avr/eeprom.h>

enum {
	ST_IDLE = 0x00,							// 000
	ST_WAITING_FOR_FIRST_SLOT = 0x01,		// 001
	ST_WAITING_FOR_SLOT = 0x02,				// 010
	ST_TRANSMITTING_LINE = 0x03,			// 011
	ST_TRANSMITTING_SETUP = 0x04,			// 100
	ST_WAITING_FOR_END_OF_SLOT = 0x05,		// 101
	ST_WAITING_FOR_UART_SENT = 0x06			// 110
} uart_state;

#define uart_udr_empty (UCSRnA & (1<<UDREn))

#define SETUP_HEADER 0x56
#define PICTURE_HEADER 0x89
#define SETUP_LENGTH 4

#define WB_MAX 0x3F

#define uart_setup_send() {uart.send_setup = 1;}

// UART
uint8_t setup_data[SETUP_LENGTH];
uart_flags uart;
uint8_t uart_y;	// line
uint8_t uart_x;	// x
uint8_t uart_c;	// color
uint8_t uart_s;	// setup

// Orientation
orientation_type  EEMEM eeprom_orientation;
orientation_type orientation;

// White Balance
uint8_t EEMEM eeprom_wb_red;
uint8_t EEMEM eeprom_wb_green;
uint8_t EEMEM eeprom_wb_blue;
uint8_t wb_red, wb_green, wb_blue;


enum {
	ST_AWAKE,
	ST_SLEEPING
}display_state;

// prototype
static inline uint8_t uart_get_byte(uint8_t y, uint8_t x, uint8_t c);
void display_set_orientation(orientation_type o);
void display_wb_set(uint8_t red, uint8_t green, uint8_t blue);


//------ Receive Pics ----------
uint8_t uart_async_run(){
	if(display_state == ST_SLEEPING){	// if the display is sleeping there is no point in sending uart stuff
		p_out_low();	// p_out is kept low
		return 0;
	}
	else{
		switch(uart_state){
		case ST_IDLE:
			p_out_low();
			if((uart.send_img || uart.send_setup) && d_out()){// if flag is set and the CD is ready
				uart_state = ST_WAITING_FOR_FIRST_SLOT;	// signal ready for transmission and wait for first slot
			}
			break;
		case ST_WAITING_FOR_FIRST_SLOT:
			p_out_high();	// signal ready for transmission
			if(!d_out() && uart_udr_empty){	// if d_in goes low and the udr is empty => start of slot
				if(uart.send_setup){
					uart.send_setup = 0;	// only send once
					UDRn = SETUP_HEADER;	// since this is the first slot, a header has to be sent
					uart_state = ST_TRANSMITTING_SETUP;
				}
				else{
					uart.send_img = 0;	// only send once
					UDRn = PICTURE_HEADER;	// since this is the first slot, a header has to be sent
					uart_state = ST_TRANSMITTING_LINE;
				}
			}
			break;
		case ST_WAITING_FOR_SLOT:
			p_out_high();	// signal ready for transmission
			if(!d_out()){	// if d_in goes low => start of slot
				uart_state = ST_TRANSMITTING_LINE;
			}
			break;
		case ST_TRANSMITTING_LINE:
			p_out_high();		// keep high during transmission
			if(uart_udr_empty){	// if byte can be sent
				// Send color value
				UDRn = uart_get_byte(uart_y, uart_x, uart_c);
				// Increase pointers
				uart_c++;
				if(uart_c >= 3){
					uart_c = 0;
					uart_x++;
					if(uart_x >= 8){
						uart_x = 0;
						uart_y++;
						uart_state = ST_WAITING_FOR_END_OF_SLOT;	// if line was sent => wait for next slot
					}
				}
			}
			break;
		case ST_TRANSMITTING_SETUP:
			p_out_high();		// keep high during transmission
			if(uart_udr_empty){	// if byte can be sent
				UDRn = setup_data[uart_s];
				uart_s++;
				if(uart_s >= SETUP_LENGTH){
					uart_y = 8;
					if(setup_data[0]  == SETUP_SLEEP_PATTERN0){	// if display is going to sleep
						uart_state = ST_WAITING_FOR_UART_SENT;	// no need to wait for the end of the slot after last byte was transmitted
					}
					else{
						uart_state = ST_WAITING_FOR_END_OF_SLOT;
					}
				}
			}
			break;
		case ST_WAITING_FOR_END_OF_SLOT:
			if(d_out()){		// rising edge signals end of slot
				if(uart_y >= 8){	// if the last line was sent
					uart_y = 0;
					uart_s = 0;
					p_out_low();
					uart_state = ST_IDLE;
				}
				else{				// if some lines still need to be sent
					p_out_high();		// keep high during transmission
					uart_state = ST_WAITING_FOR_SLOT;
				}
			}
			break;
		case ST_WAITING_FOR_UART_SENT:
			if(uart_udr_empty){	// wait for uart to be sent
				display_state = ST_SLEEPING;			// it should be asleep soon
				uart_state = ST_IDLE;
			}
			break;
		default:	// should not get here
			p_out_low();
			uart_y = 0;
			uart_x = 0;
			uart_c = 0;
			uart_s = 0;
			uart_state = ST_IDLE;
			break;
		}
		return uart_state;
	}
}


// ------------ White Balance -------------------------------
void display_wb_set(uint8_t red, uint8_t green, uint8_t blue){
	wb_red = red;
	wb_green = green;
	wb_blue = blue;
}

void display_wb_increase(uint8_t color_index){
	switch(color_index){
	default:
	case RED_INDEX:
		if(wb_red < WB_MAX) wb_red++;
		else wb_red = 0;
		break;
	case GREEN_INDEX:
		if(wb_green < WB_MAX) wb_green++;
		else wb_green = 0;
		break;
	case BLUE_INDEX:
		if(wb_blue < WB_MAX) wb_blue++;
		else wb_blue = 0;
		break;
	}
}

void display_wb_decrease(uint8_t color_index){
	switch(color_index){
	default:
	case RED_INDEX:
		if(wb_red > 0) wb_red--;
		else wb_red = WB_MAX;
		break;
	case GREEN_INDEX:
		if(wb_green > 0) wb_green--;
		else wb_green = WB_MAX;
		break;
	case BLUE_INDEX:
		if(wb_blue > 0) wb_blue--;
		else wb_blue = WB_MAX;
		break;
	}
}

uint8_t display_wb_get(uint8_t color_index){
	switch(color_index){
	default:
	case RED_INDEX:
		return wb_red;
		break;
	case GREEN_INDEX:
		return wb_green;
		break;
	case BLUE_INDEX:
		return wb_blue;
		break;
	}
}

void display_wb_update(){
	display_wb_send(wb_red, wb_green, wb_blue);
}

void display_wb_send(uint8_t red, uint8_t green, uint8_t blue){
	uart_s = 0;
	setup_data[0]  |= (1<<7);	// bit7 means that white balance needs to be updated
	setup_data[1] = red;
	setup_data[2] = green;
	setup_data[3] = blue;
	uart_setup_send();	// send
}

void display_wb_save(){
	eeprom_write_byte(&eeprom_wb_red, wb_red);
	eeprom_write_byte(&eeprom_wb_green, wb_green);
	eeprom_write_byte(&eeprom_wb_blue, wb_blue);
}

// ------------ Display Sleep -------------------------------
void display_sleep(){
	uart_s = 0;
	setup_data[0] = SETUP_SLEEP_PATTERN0;	// set sleep pattern (and nothing else!)
	setup_data[1] = SETUP_SLEEP_PATTERN1;
	setup_data[2] = SETUP_SLEEP_PATTERN2;
	setup_data[3] = SETUP_SLEEP_PATTERN3;
	uart_setup_send();						// send setup immediately
}

void display_wake_up(){
	p_out_high();		// trigger interrupt to wake up
	_delay_us(50);		// wait for display to wake up
	display_state = ST_AWAKE;	// back among the living!
}

// ------------ Orientation -------------------------------
void display_set_orientation(orientation_type o){
	orientation = o;
	eeprom_write_byte(&eeprom_orientation, orientation); // save orientation to eeprom
}

orientation_type display_get_orientation(){
	return orientation;
}

void display_change_orientation(){
	orientation_type o = orientation + 1;
	if(o > ROTATE_270){
		o = ROTATE_00;
	}
	display_set_orientation(o);
}

static inline uint8_t uart_get_byte(uint8_t y, uint8_t x, uint8_t c){
	switch(orientation){
	default:
	case ROTATE_00:
		return img_buffer.raw_data[y][x][c];
	case ROTATE_90:
		return img_buffer.raw_data[7-x][y][c];
	case ROTATE_180:
		return img_buffer.raw_data[7-y][7-x][c];
	case ROTATE_270:
		return img_buffer.raw_data[x][7-y][c];
	}
}

// ------------ UART Initialize -------------------------------
void uart_init_(uint8_t baud){
	DDRD |= (1<<PD2);
	// Load Orientation from EEPROM
	orientation = eeprom_read_byte(&eeprom_orientation);
	if(orientation > ROTATE_270){
		display_set_orientation(DEFAULT_ORIENTATION);
	}
	// Load White Balance Settings from EEPROM
	wb_red = eeprom_read_byte(&eeprom_wb_red) & WB_MAX;
	wb_green = eeprom_read_byte(&eeprom_wb_green) & WB_MAX;
	wb_blue = eeprom_read_byte(&eeprom_wb_blue) & WB_MAX;
	// Set Default Values
	uart_y = 0;
	uart_x = 0;
	uart_c = 0;
	uart_s = 0;
	uart.send_img = 0;
	UBRRn = baud;						//set baud rate
	UCSRnC = (0<<USBSn) | (3<<UCSZn0);	//frame format: 8data, 1 stop bit
	UCSRnB = (1<<RXENn) | (1<<TXENn);	//enable Rx and Tx
}
