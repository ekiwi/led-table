#include "animations.h"

#define colors {led_color_(242,254,29),led_color_(31,239,239),led_color_(249,18,6),led_color_(254,96,205),led_color_(167,48,167),led_color_(125,252,12)}
#define ARRAY_LENGTH 6

bool ani_psy_init() {
    return true;
}


void ani_psy_run(led_image* canvas, uint32_t ticks, uint32_t time_elapsed){

    led_color c[ARRAY_LENGTH] = colors;

    for(uint8_t x =0; x<LED_WIDTH; x++){
        for(uint8_t y =0; y<LED_HEIGHT; y++){
            set_pixel(led_point_(x,y),c[led_rand()%ARRAY_LENGTH], canvas);
        }
    }



}
