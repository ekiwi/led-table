/*
 * main.c
 *
 *  Created on: Apr 7, 2012
 *      Author: kevin
 */

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>
#include "led_matrix.h"
#include "led_memory.h"
#include "uart.h"

#define BAUDRATE 250000UL 	//250k

int main(void){

	for(uint8_t y = 0; y < 8; y++){
		for(uint8_t x = 0; x < 8; x++){
			img_buffer.raw_data[y][x][0] = 0x00;	// R
			img_buffer.raw_data[y][x][1] = 0x00;	// G
			img_buffer.raw_data[y][x][2] = 0x00;	// B
		}
	}

	matrix_init();
	uart_init(BAUDRATE);
	sei();
	_delay_ms(1);	// some start up delay;


	while(1){
		uart_async_run();	// receive picture data via UART
		if(sleep_flag){		// if this is true, the display should go to sleep
			sleep_flag = 0x00;
			// stop / disable peripherals
			cli();	// disable interrupts
			matrix_stop();
			uart_stop();

			// Wait for PC5 to go low before enabling interrupt
			while(PINC & (1 << PC5));

			// enable external pin change interrupt on PC5 (p_out): PCINT13
			// DEBUG: do not enable interrupt => sleep forever
			PCMSK1 |= (1<<PCINT13);	// set interrupt mask for PC5
			PCICR |= (1<<PCIE1);	// enable pin change interrupt 1

			// go to sleep
			set_sleep_mode(SLEEP_MODE_STANDBY);
			sleep_enable();
			sei();
			sleep_cpu();
			sleep_disable();

			// wake up
			cli();	// disable interrupts
			PCICR = 0x00;	// disable pin change interrupt
			PCMSK1 = 0x00;

			// restart peripherals
			matrix_restart();
			uart_init(BAUDRATE);
			sei();	// reenable interrupts
		}
	}
}

ISR(PCINT1_vect){	// PCINT13 used to wake up from sleep mode
	/* can be empty*/
}

