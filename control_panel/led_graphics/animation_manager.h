/*
 * animation_manager.h
 *
 *  Created on: Apr 10, 2012
 *      Author: kevin
 */

#ifndef ANIMATION_MANAGER_H
#define ANIMATION_MANAGER_H

#include "led_graphics.h"

void animation_manager_init(void);
void animation_manager_run(uint32_t time_elapsed);
void animation_manager_load_animation(uint8_t index);
void animation_manager_dec_animation();
void animation_manager_inc_animation();
void animation_manager_random_animation();
void animation_manager_error(char* str);

extern uint8_t img_buf_index_send;



#endif // ANIMATION_MANAGER_H
