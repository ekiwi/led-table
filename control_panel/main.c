#include <util/delay.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>
#include "libs/uart.h"
#include "libs/interface.h"
#include "libs/sys_timer.h"
#include "libs/audio.h"
#include "libs/setup.h"
#include "animations.h"

#define BAUDRATE 250000UL 	//250k


void static inline go_to_sleep();


int main(){
	// Initialize Peripherals
	interface_init();
	red_led_on();
	uart_init(BAUDRATE);
	animation_manager_init();
	sys_timer_start();
	audio_init();
	sei();	// enable global interrupts

	// Load Default Animation
	animation_manager_load_animation(START_ANIMATION);

	// Enter Setup if Requested
	_delay_ms(100);
	if(deb_switch_1()){
		setup_wb_run();
	}
	else if(deb_switch_2()){
		setup_orientation_run();
	}

	// Load Default Animation
	animation_manager_load_animation(START_ANIMATION);

	// Set White Balance
	_delay_ms(300);
	display_wb_update();
	while(uart_async_run());	// setup white balance

	// Control Panel is Ready => Signal this by Turning the LED Green
	red_led_off();
	green_led_on();

	while(1){
		// Sleep Mode
		if(!switch_on_off()){	// if switched off
			go_to_sleep();
		}

		// Change animations
		sw_check();
		if(sw_check_pressed(SW_LEFT, 200, true)){
			animation_manager_dec_animation();
		}
		else if(sw_check_pressed(SW_RIGHT, 200, true)){
			animation_manager_inc_animation();
		}
		else if(sw_check_pressed(SW_RAND, 300, true)){
			animation_manager_random_animation();
		}

		// Generate Image
		animation_manager_run(0);

		// Check Audio
		audio_start();
		while(audio_run());
		audio_process();

		// Display Image
		while(uart_async_run()){
			interface_async_run();
		}

	}
}

void static inline go_to_sleep(){
	// stop / disable peripherals
	cli();	// disable interrupts
	sys_timer_stop();
	_delay_us(300);			// small delay in order for the colorduino to be ready for transmission
	display_sleep();
	while(uart_async_run());	// put display to sleep
	_delay_us(60);
	p_out_low();
	interface_disable();		// turn leds + display off

	// disable UART
	UCSRnB = 0x00;	//disable Rx and Tx

	// enable external pin change interrupt on PA7: PCINT7
	PCMSK0 |= (1<<PCINT7);	// set interrupt mask for PA7
	PCICR |= (1<<PCIE0);	// enable pin change interrupt 0

	// go to sleep
	set_sleep_mode(SLEEP_MODE_STANDBY);
	sleep_enable();
	sei();
	sleep_cpu();
	sleep_disable();

	// wake up
	cli();	// disable interrupts
	PCICR = 0x00;	// disable pin change interrupt
	PCMSK0 = 0x00;

	// restart peripherals
	UCSRnB = (1<<RXENn) | (1<<TXENn);	//enable Rx and Tx
	interface_init();
	display_wake_up();
	sys_timer_start();
	green_led_on();
	sei();	// reenable interrupts
}


ISR(PCINT0_vect){	// PCINT7 used to wake up from sleep mode
	/* can be empty*/
}



// FPS
/*	uint32_t fps = 1000 / (sys_timer_get() - last_frame);
last_frame = sys_timer_get();
set_display(fps, 0, 0); */
