/*
 * uart.c
 *
 *  Created on: Apr 7, 2012
 *      Author: kevin
 *
 *		Colorduino
 *
 *      For 250kbps use MAX3232 instead of MAX232 out of spec
 */

#include <avr/io.h>
#include "uart.h"
#include "led_memory.h"

#define uart_rx_complete (UCSRnA & (1<<RXCn))

uint8_t setup_data[SETUP_LENGTH];

uint8_t uart_y;	// line
uint8_t uart_x;	// x
uint8_t uart_c;	// color
uint8_t uart_s;	// setup

uint8_t sleep_flag;

enum{
	ST_IDLE,
	ST_WAITING_FOR_FIRST_SLOT,
	ST_RECEIVING_HEADER,
	ST_RECEIVING_LINES,
	ST_RECEIVING_SETUP_BYTES,
	ST_WAITING_FOR_END_OF_TRANSMISSION
} uart_state;

static inline void uart_read_setup_data(void);
static inline void uart_set_byte(uint8_t data, uint8_t y, uint8_t x, uint8_t c);

uint8_t uart_async_run(){

	if(!p_out()){				// if p_out goes low, this signals the end of the transmission
		uart_state = ST_IDLE;	// done => go back to idle
	}

	switch(uart_state){
	case ST_IDLE:
		d_out_high();
		if(p_out()){	// if p_out goes high => transmission is requested
			uart_state = ST_WAITING_FOR_FIRST_SLOT;
		}
		break;
	case ST_WAITING_FOR_FIRST_SLOT:
		d_out_high();	// keep d_out high, while waiting for the first slot
		break;
	case ST_RECEIVING_HEADER:
		d_out_low();
		if(uart_rx_complete){	//when header was received
			switch(UDRn) {
			case SETUP_HEADER:
				uart_state = ST_RECEIVING_SETUP_BYTES;
				break;
			case PICTURE_HEADER:
				uart_state = ST_RECEIVING_LINES;
				break;
			default:
				uart_state = ST_IDLE;
				break;
			}
		}
		break;
	case ST_RECEIVING_LINES:
		d_out_low();
		if(uart_rx_complete){	//receive current line
			uart_set_byte(UDRn, uart_y, uart_x, uart_c);	// save color value
			// Increase pointers
			uart_c++;
			if(uart_c >= 3){
				uart_c = 0;
				uart_x++;
				if(uart_x >= 8){
					uart_x = 0;
					uart_y++;
					if(uart_y >=8){	// if this was the last line
						uart_state = ST_WAITING_FOR_END_OF_TRANSMISSION;
					}
				}
			}
		}
		break;
	case ST_RECEIVING_SETUP_BYTES:
		d_out_low();
		if(uart_rx_complete){			//receive setup bytes
			setup_data[uart_s] = UDRn;	// save setup byte
			uart_s++;
			if(uart_s >= SETUP_LENGTH){
				uart_read_setup_data();	// read setup data
				uart_state = ST_WAITING_FOR_END_OF_TRANSMISSION;	// if this was the last setup byte
			}
		}
		break;
	case ST_WAITING_FOR_END_OF_TRANSMISSION:
		d_out_low();
		break;
	default:
		break;
	}
	return uart_state;
}

static inline void uart_set_byte(uint8_t data, uint8_t y, uint8_t x, uint8_t c){
	img_buffer.raw_data[y][x][c] = data;
}

/*
 * Is called by the interrupt when a new slot starts
 */
void uart_start_slot(uint8_t slot){
	if(uart_state == ST_WAITING_FOR_FIRST_SLOT){	// if state machine is waiting for first slot
		if(slot == 0){				// if this is the first slot i.e. Slot0
			uint8_t temp = UDRn;	// reset UART Buffer
			uart_y = 0;				// reset pointers
			uart_x = 0;
			uart_c = 0;
			uart_s = 0;
			d_out_low();	// signal ready to receive
			uart_state = ST_RECEIVING_HEADER;	// start receiving header
		}
	}
	else if(uart_state == ST_RECEIVING_LINES){		// if already receiving lines
		d_out_low();	// signal ready to receive
	}
}

/*
 * Reads all the setups bytes after they were received
 * Byte0:
 * 	Bit7: Update White Balance (3 following bytes contain the values)
 */
static inline void uart_read_setup_data(void){
	if(setup_data[0] == SETUP_SLEEP_PATTERN0){		// if sleep mode is requested
		// only if the correct sleep pattern is sent sleep mode is activated
		if(setup_data[1] == SETUP_SLEEP_PATTERN1 && setup_data[2] == SETUP_SLEEP_PATTERN2 && setup_data[3] == SETUP_SLEEP_PATTERN3){
			sleep_flag = 0xff;	// signal ready to go to sleep
		}
	}
	else if((setup_data[0] & (1<<7))){	// if white balance is to be updated
		white_balance[0] = setup_data[1];
		white_balance[1] = setup_data[2];
		white_balance[2] = setup_data[3];
		update_white_balance();
	}
}

void uart_init_(uint8_t baud){
	DDRC |= (1<<4);		// D_OUT
	// DEBUG: DDRD |= (1<<PD1);	// TX as output
	d_out_low();		// low until initialized and ready
	uart_y = 0;
	uart_x = 0;
	uart_c = 0;
	uart_s = 0;
	sleep_flag = 0x00;
	uart_state = ST_IDLE;	// start in idle state
	UBRRn = baud;						//set baud rate
	UCSRnC = (0<<USBSn) | (3<<UCSZn0);	//frame format: 8data, 1 stop bit
	UCSRnB = (1<<RXENn);				//enable Rx
}

void uart_stop(void){
	DDRC &= ~(1<<4);		// disable D_OUT output
	d_out_low();			// disable pullup
	UCSRnB = 0x00;			// disable uart
}
