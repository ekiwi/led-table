/*
 * led_memory.h
 *
 *  Created on: Apr 7, 2012
 *      Author: kevin
 *
 *      This is the Version adjusted for the Colorduino FW
 */

#ifndef LED_MEMORY_H
#define LED_MEMORY_H

#include <avr/io.h>

#define LED_HEIGHT 8
#define LED_WIDTH 8

// ---------------------- Colors ---------------------------
typedef struct{
    uint8_t red;
    uint8_t green;
    uint8_t blue;
} led_color;



#define RED led_color_(0xFF,0x00,0x00)
#define GREEN led_color_(0x00,0xFF,0x00)
#define BLUE led_color_(0x00,0x00,0xFF)
#define BLACK led_color_(0x00,0x00,0x00)


typedef struct{
    union{
        led_color pixel [LED_WIDTH][LED_HEIGHT];
        uint8_t   raw_data [LED_WIDTH][LED_HEIGHT][3];
    };
    uint8_t width;
    uint8_t height;
    led_color transparency_key;
} led_image;


// "constructors"
led_color led_color_(uint8_t red, uint8_t green, uint8_t blue);
void clr_image(led_color color, led_image* img);

// buffer memory
extern led_image img_buffer;


// white balance
#define update_white_balance() update_ws_flag = 1
extern uint8_t white_balance[3];
extern uint8_t update_ws_flag;

#endif /* LED_MEMORY_H_ */
