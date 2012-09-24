#include "animations.h"


bool ani_equalizer_init(){
    return true;
}


void ani_equalizer_run(led_image* canvas, uint32_t ticks, uint32_t time_elapsed){
	clr_image(led_color_(0x00,0x00,0x00), canvas);
	led_color c1 = led_color_(0xFF,0xFF,0x00);
	led_color c2 = led_color_(0xFF,0x00,0x00);
    uint8_t i;
    for(i = 0; i < 7; i++){
		uint16_t y = audio_get(i);
		y = ((y * 8) >> 8);
        set_pixel(led_point_(i, 7 - y), c2, canvas);
		while(y > 0){
			y--;
            set_pixel(led_point_(i, 7 - y), c1, canvas);
		}
	}
}



