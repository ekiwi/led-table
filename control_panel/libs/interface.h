/*
 * interface.h
 *
 *  Created on: Apr 4, 2012
 *      Author: kevin
 */

#ifndef INTERFACE_H_
#define INTERFACE_H_

#include <avr/io.h>
#include <stdbool.h>
#include "i2c.h"

#define SCL_CLOCK  100000L // i2c clock frequency

// I2C Display Controller
#define DISPLAY_ADDRESS 	0x70
#define DISPLAY_CONFIG 		0x00
#define DISPLAY_DIGIT_1 	0x01
#define DISPLAY_DIGIT_2 	0x02
#define DISPLAY_SETUP_OFF	0x00	// 00000000
#define DISPLAY_SETUP_03MA	0x16	// 00010110
#define DISPLAY_SETUP_06MA	0x26	// 00100110
#define DISPLAY_SETUP_09MA	0x36	// 00110110
#define DISPLAY_SETUP_12MA	0x46	// 01000110
#define DISPLAY_SETUP_21MA	0x76	// 01110110
#define DISPLAY_TEST		0x4E	// 01001110

#define DISPLAY_SHOW_ERROR 0xD0


// Debug LEDs
#define deb_green_led_on() 		PORTB |= (1<<PB2)
#define deb_yellow_led_on() 	PORTB |= (1<<PB1)
#define deb_red_led_on() 		PORTB |= (1<<PB0)
#define deb_green_led_off() 	PORTB &= ~(1<<PB2)
#define deb_yellow_led_off()  	PORTB &= ~(1<<PB1)
#define deb_red_led_off() 		PORTB &= ~(1<<PB0)

// Debug Switches
#define deb_switch_1() (!(PINB & (1 << PB3)))
#define deb_switch_2() (!(PINB & (1 << PB4)))

// Switches
#define switch_left() 	(!(PINA & (1 << PA4)))
#define switch_rand() 	(!(PINA & (1 << PA5)))
#define switch_right() 	(!(PINA & (1 << PA6)))
#define switch_on_off() (!(PINA & (1 << PA7)))

// Interface LEDs
#define green_led_on()	PORTD &= ~(1<<PD7)
#define green_led_off()	PORTD |= (1<<PD7)
#define green_led_toggle() PORTD ^= (1<<PD7)
#define red_led_on()	PORTD &= ~(1<<PD6)
#define red_led_off() 	PORTD |= (1<<PD6)
#define red_led_toggle() PORTD ^= (1<<PD6)


// Switch Data
typedef struct{
	uint8_t pressed;
	uint32_t time_stamp_pressed;
	uint32_t time_stamp_released;
} switch_data;

#define NUMBER_SWITCHES 3
#define SW_LEFT 	0
#define SW_RAND 	1
#define SW_RIGHT 	2

extern switch_data sw[NUMBER_SWITCHES];


void interface_init(void);
void interface_disable(void);
void set_display(uint8_t number, uint8_t decimal_point_on, uint8_t yellow_led_on);
bool interface_async_run(void);
void sw_check(void);
bool sw_check_pressed(uint8_t sw_id, uint8_t pressed_for_ms, uint8_t refresh_time_stamp);


#endif /* INTERFACE_H_ */
