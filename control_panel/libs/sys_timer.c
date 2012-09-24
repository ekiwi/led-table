/*
 * sys_timer.c
 *
 *  Created on: Apr 13, 2012
 *      Author: kevin
 */


#include "sys_timer.h"
#include <avr/interrupt.h>

uint32_t system_time = 0;


/*
 * Initialize Timer2 to function as system timer
 */
void sys_timer_start(){
	TCCR2A = (1<<WGM21);	// CTC Mode
	TCCR2B = (1<<CS22) | (1<<CS21);// Prescaler: 256 => needs to count to: 20MHz / 256 / 1kHZ = 78
	OCR2A = 78;				// counter maximum (ctc mode!)
	TIMSK2 = (1<<OCIE2A);		// enable compare match interrupt
	system_time = 0;		// reset system time
}

void sys_timer_stop(){
	// Stop timer
	TIMSK2 = 0x00;	// disable interrupt
	TCCR2B = 0x00;	// disable timer
}


ISR(TIMER2_COMPA_vect ){  // is called on compare match
	system_time++;
}
