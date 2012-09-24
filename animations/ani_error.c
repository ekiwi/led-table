#include "animations.h"


// this time the global varibale is an exception, because
// the error animation should not access the dynamic
// memory
uint8_t counter;

#define CHAR_WIDTH 7



bool ani_error_init(){
    counter = 0;
    return true;
}


void ani_error_run(led_image* canvas, uint32_t ticks, uint32_t time_elapsed){
	clr_image(led_color_(0x00,0x00,0x00), canvas);
    counter++;
    if(counter <= 15){
        draw_char(led_point_(1,1), 'E', led_color_(counter << 4,0x00,0x00), canvas);
    }
    else if(counter <= 50){
        draw_char(led_point_(1,1), 'E', led_color_(0xFF,0x00,0x00), canvas);
    }
    else if(counter <= 65){
        draw_char(led_point_(1,1), 'E', led_color_((65 - counter) << 4,0x00,0x00), canvas);
    }
    else if(counter == 70){
        counter = 0;
    }
}





