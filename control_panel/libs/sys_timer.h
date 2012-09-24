/*
 * sys_timer.h
 *
 *  Created on: Apr 13, 2012
 *      Author: kevin
 */

#ifndef SYS_TIMER_H_
#define SYS_TIMER_H_

#include <avr/io.h>

extern uint32_t system_time;

#define sys_timer_get() (system_time)

// Timer/Counter 2 is the System Timer
void sys_timer_start();
void sys_timer_stop();



#endif /* SYS_TIMER_H_ */
