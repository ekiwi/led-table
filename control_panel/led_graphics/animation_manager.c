/*
 * animation_manager.c
 *
 *  Created on: Apr 10, 2012
 *      Author: kevin
 */


#include "animations.h"
#include "animation_manager.h"

// variables
uint8_t ani_index = 0;
uint8_t can_run_animation = 0;  // indicates whether an animation can be displayed; needs to be zero if memory is being accessed
uint32_t ticks = 0;

void animation_manager_init(void){
    img_buffer.height = 8;
    img_buffer.width = 8;
    can_run_animation = 1;
}

void animation_manager_run(uint32_t time_elapsed){
    if(can_run_animation && ani_index < ANIMATION_NUMBER + 1){  // +1 for error animation to be able to run
        ani_table[ani_index].run(&img_buffer, ticks, time_elapsed);
        ticks++;    		// increase ticks
        uart_img_send();	// send image
    }
}

void animation_manager_dec_animation(){
    if(ani_index == 0){
        animation_manager_load_animation(ANIMATION_NUMBER - 1);
    }
    else {
        animation_manager_load_animation(ani_index-1);
    }
}

void animation_manager_inc_animation(){
    if(ani_index == ANIMATION_NUMBER - 1){
        animation_manager_load_animation(0);
    }
    else {
        animation_manager_load_animation(ani_index+1);
    }
}

void animation_manager_random_animation(){
	uint8_t ani = ((uint16_t)(led_rand() + 1) * (ANIMATION_NUMBER-1)) >> 8;
	animation_manager_load_animation(ani);
}

void animation_manager_load_animation(uint8_t index){
    if(index >= ANIMATION_NUMBER || index == ani_index) return;
    can_run_animation = false;
    memory_clear();             	// clear memory
    ticks = 0;
    if(ani_table[index].init()){   	// load animation
        ani_index = index;          	// save new index
        set_display(index + 1, 0, 0);   // show new index on 7 segment display (readable for "regular humans" who are used to starting to count with 1)
        can_run_animation = true;
    }
    else{   // if it fails to load, load error animation
        ani_index = ERROR_ANIMATION;
        ani_table[ani_index].init();
        can_run_animation = true;
        set_display(99, DISPLAY_SHOW_ERROR, DISPLAY_SHOW_ERROR);
    }
}
