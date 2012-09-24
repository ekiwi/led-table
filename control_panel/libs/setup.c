/*
 * setup.c
 *
 *  Created on: Jun 14, 2012
 *      Author: kevin
 */
#include "setup.h"
#include "../animations/animations.h"
#include "sys_timer.h"
#include "interface.h"

void setup_wb_run(){
	uint8_t running = 1;
	uint32_t led_time_stamp = sys_timer_get();
	uint8_t color = RED_INDEX;
	// Display White Screen
	clr_image(led_color_(0xFF,0xFF,0xFF), &img_buffer);
	uart_img_send();
	while(uart_async_run());
	// Update 7-Segment Display
	set_display(color, 0, 0);
	// Send White Balance
	display_wb_update();
	while(uart_async_run());	// setup white balance
	// Set LEDs
	red_led_off();
	green_led_on();
	// Loop
	while(running){
		// Check Switches
		sw_check();
		if(sw_check_pressed(SW_LEFT, 0, false) && sw_check_pressed(SW_RIGHT, 0, false)){
			running = 0;
		}
		else if(sw_check_pressed(SW_LEFT, 200, true)){
			display_wb_decrease(color);
			set_display(display_wb_get(color), 0, 0);
			display_wb_update();
		}
		else if(sw_check_pressed(SW_RIGHT, 200, true)){
			display_wb_increase(color);
			set_display(display_wb_get(color), 0, 0);
			display_wb_update();
		}
		else if(sw_check_pressed(SW_RAND, 300, true)){
			color++;
			if(color > BLUE_INDEX){
				color = 0;
			}
			set_display(color, 0, 0);
		}


		//Toggle LEDs
		if((sys_timer_get() - led_time_stamp) > SETUP_LED_TOGGLE_MS){
			led_time_stamp = sys_timer_get();
			green_led_toggle();
			red_led_toggle();
		}

		// Refresh WB
		_delay_ms(2);	// wait in order for the table to be ready to receive
		while(uart_async_run());
		while(interface_async_run());

	}
	display_wb_save();
}

void setup_orientation_run(){
	uint8_t running = 1;
	uint32_t led_time_stamp = sys_timer_get();
	// Load Laufschrift Animation to Visualize Orientation
	animation_manager_load_animation(ANI_LAUFSCHRIFT);
	// Show Number
	set_display(display_get_orientation(), 0 , 0);
	// Set LEDs
	red_led_off();
	green_led_on();
	// Loop
	while(running){
		// Check Switches
		sw_check();
		if(sw_check_pressed(SW_LEFT, 0, false) && sw_check_pressed(SW_RIGHT, 0, false)){
			running = 0;
		}
		else if(sw_check_pressed(SW_LEFT, 200, true) || sw_check_pressed(SW_RIGHT, 200, true)){
			display_change_orientation();
			set_display(display_get_orientation(), 0 , 0);
		}

		//Toggle LEDs
		if((sys_timer_get() - led_time_stamp) > SETUP_LED_TOGGLE_MS){
			led_time_stamp = sys_timer_get();
			green_led_toggle();
			red_led_toggle();
		}

		// Refresh Image
		animation_manager_run(0);
		_delay_us(200);	// wait in order for the table to be ready to receive
		while(uart_async_run()){
			interface_async_run();
		}
	}
}
