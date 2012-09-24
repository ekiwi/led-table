/*
 * interface.c
 *
 *  Created on: Apr 4, 2012
 *      Author: kevin
 */

#include "sys_timer.h"
#include "interface.h"

#define BCD_0 0xFC	// 11111100
#define BCD_1 0x60	// 01100000
#define BCD_2 0xDA	// 11011010
#define BCD_3 0xF2	// 11110010
#define BCD_4 0x66	// 01100110
#define BCD_5 0xB6	// 10110110
#define BCD_6 0x3E	// 00111110
#define BCD_7 0xE0	// 11100000
#define BCD_8 0xFE	// 11111110
#define BCD_9 0xE6	// 11100110
#define BCD_E 0x9E	// 10011110


switch_data sw[NUMBER_SWITCHES];

struct{
	uint8_t display_number;
	struct {
			unsigned decimal_point_on :1;
			unsigned updating_digit_1 :1;
			unsigned updating_digit_2 :1;
			unsigned display_used :1;
			unsigned :4;
		};
} interface;



void interface_init(void){
	DDRB |= (1<<PB2) | (1<<PB1) | (1<<PB0);		// LED outputs
	DDRD |= (1<<PD7) | (1<<PD6);	// interface LEDs
	PORTD |= (1<<PD6) | (1<<PD7);	// interface LEDs are low active
	PORTA |= (1<<PA4) | (1<<PA5) | (1<<PA6) | (1<<PA7);	// enable pull ups for switches
	PORTB |= (1<<PB3) | (1<<PB4);			// enable pull ups for debug switches
	i2c_async_init(SCL_CLOCK);		// initialize i2c
	// initialize display controller
	i2c_async_start(DISPLAY_ADDRESS + I2C_WRITE);
	i2c_async_write_2(DISPLAY_CONFIG, DISPLAY_SETUP_03MA);
	i2c_async_stop();
	while(i2c_async_run());
	interface.display_used = true;
}

void interface_disable(void){
	DDRB &= ~((1<<PB2) | (1<<PB1) | (1<<PB0));		// LED outputs
	DDRD &= ~((1<<PD7) | (1<<PD6));					// interface LEDs
	PORTD &= ~((1<<PD7) | (1<<PD6));			// disable pull ups
	PORTA &= ~((1<<PA4) | (1<<PA5) | (1<<PA6));	// disable pull ups; except for PA7 => is needed for re enabling
	PORTB &= ~((1<<PB3) | (1<<PB4));			// disable pull ups
	// disable display controller
	i2c_async_start(DISPLAY_ADDRESS + I2C_WRITE);
	i2c_async_write_2(DISPLAY_CONFIG, DISPLAY_SETUP_OFF);
	i2c_async_stop();
	while(i2c_async_run());
	interface.display_used = false;
	// disable i2c
	i2c_async_disable();
}

uint8_t bcd_decoder(uint8_t number, bool decimal_point){
	if(decimal_point){
		decimal_point = 0x01;
	}
	switch(number){
	case 0:
			return BCD_0 | decimal_point;
			break;
	case 1:
			return BCD_1 | decimal_point;
			break;
	case 2:
			return BCD_2 | decimal_point;
			break;
	case 3:
			return BCD_3 | decimal_point;
			break;
	case 4:
			return BCD_4 | decimal_point;
			break;
	case 5:
			return BCD_5 | decimal_point;
			break;
	case 6:
			return BCD_6 | decimal_point;
			break;
	case 7:
			return BCD_7 | decimal_point;
			break;
	case 8:
			return BCD_8 | decimal_point;
			break;
	case 9:
			return BCD_9 | decimal_point;
			break;
	default:
			return 0x00 | decimal_point;
			break;
	}
	return BCD_0 | decimal_point;
}

void set_display(uint8_t number, uint8_t decimal_point_on, uint8_t yellow_led_on){
	if(interface.display_used){
		interface.display_number = number % 10;
		interface.decimal_point_on = decimal_point_on;
		i2c_async_start(DISPLAY_ADDRESS + I2C_WRITE);
		if(yellow_led_on == DISPLAY_SHOW_ERROR){
			interface.display_number = 10;
			i2c_async_write_2(DISPLAY_DIGIT_1, BCD_E);
		}
		else{
			i2c_async_write_2(DISPLAY_DIGIT_1, bcd_decoder((number / 10) % 10, yellow_led_on));
		}
		i2c_async_stop();
		interface.updating_digit_1 = true;
	}
}

bool interface_async_run(void){
	// Update Display
	if(interface.updating_digit_1){
		if(!i2c_async_run()){
			interface.updating_digit_1 = false;
			i2c_async_start(DISPLAY_ADDRESS + I2C_WRITE);
			i2c_async_write_2(DISPLAY_DIGIT_2, bcd_decoder(interface.display_number, interface.decimal_point_on ));
			i2c_async_stop();
			interface.updating_digit_2 = true;
		}
	}
	else if(interface.updating_digit_2){
		if(!i2c_async_run()){
			interface.updating_digit_2 = false;
		}
	}
	return interface.updating_digit_1 | interface.updating_digit_2;
}

// Check Switches
void sw_check(void){
	// Check Switches
	if(sw[SW_LEFT].pressed != switch_left()){
		sw[SW_LEFT].pressed = switch_left();
		if(sw[SW_LEFT].pressed) sw[SW_LEFT].time_stamp_pressed = sys_timer_get();
		else sw[SW_LEFT].time_stamp_released = sys_timer_get();
	}
	if(sw[SW_RAND].pressed != switch_rand()){
		sw[SW_RAND].pressed = switch_rand();
		if(sw[SW_RAND].pressed) sw[SW_RAND].time_stamp_pressed = sys_timer_get();
		else sw[SW_RAND].time_stamp_released = sys_timer_get();
	}
	if(sw[SW_RIGHT].pressed != switch_right()){
		sw[SW_RIGHT].pressed = switch_right();
		if(sw[SW_RIGHT].pressed) sw[SW_RIGHT].time_stamp_pressed = sys_timer_get();
		else sw[SW_RIGHT].time_stamp_released = sys_timer_get();
	}
}

bool sw_check_pressed(uint8_t sw_id, uint8_t pressed_for_ms, uint8_t refresh_time_stamp){
	if(sw_id >= NUMBER_SWITCHES) return false;
	if(sw[sw_id].pressed && (sys_timer_get() - sw[sw_id].time_stamp_pressed) > pressed_for_ms){
		if(refresh_time_stamp){
			sw[sw_id].time_stamp_pressed = sys_timer_get();
		}
		return true;
	}
	return false;
}


