/*
 * led_memory.c
 *
 *  Created on: Apr 7, 2012
 *      Author: kevin
 */

#include "led_memory.h"

led_image img_buffer;

// white balance
uint8_t white_balance[3];
uint8_t update_ws_flag;

// "constructors"
led_color led_color_(uint8_t red, uint8_t green, uint8_t blue){
    led_color c;
    c.red = red;
    c.green = green;
    c.blue = blue;
    return c;
}

void clr_image(led_color color, led_image* img){
    for(int y = 0; y < img->height; y++){
        for(int x = 0; x < img->width; x++){
            img->pixel[x][y] = color;
        }
    }
}
