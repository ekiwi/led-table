#include "animations.h"

#define string {'A', 'b', 'i', '2', '0', '1', '2'}
#define ARRAY_LENGTH 7

#define START_Y 1
#define LETTER_COLOR led_color_(0xFF,0xFF,0xFF)
#define BG_COLOR led_color_(0x00,0x00,0x00)
#define MARGIN_Y 1
#define MARGIN_X -3
#define LETTER_WIDTH 8
#define START_X 7

typedef struct {
	int8_t current_x;
} global_data;
global_data *global_vars;


bool ani_laufschrift_init() {
	global_vars = memory_alloc(sizeof(global_data));
    if(global_vars != NULL){
        g.current_x = START_X;
        return true;
    }
    return false;
}

void ani_laufschrift_run(led_image* canvas, uint32_t ticks, uint32_t time_elapsed) {
    if(ticks % 10 == 0){
        clr_image(BG_COLOR, canvas);
        uint8_t s[ ARRAY_LENGTH ] = string;

        // get letters
        uint8_t sec_index = abs(g.current_x)/(LETTER_WIDTH+MARGIN_X);
        uint8_t sec_letter = s[ sec_index ];

        if(sec_letter) {
            draw_char(led_point_(g.current_x + (sec_index+1) * (LETTER_WIDTH+MARGIN_X),MARGIN_Y), sec_letter, LETTER_COLOR, canvas);
        }

        uint8_t first_letter = sec_letter ? s[ sec_index-1 ] : 0;

        if(first_letter) {
            draw_char(led_point_(g.current_x + sec_index * (LETTER_WIDTH+MARGIN_X),MARGIN_Y), first_letter, LETTER_COLOR, canvas);
        }

        if( ticks%20 == 0 ) {
        	g.current_x--;

            if( g.current_x == -ARRAY_LENGTH * (LETTER_WIDTH+MARGIN_X)) {
            	g.current_x = START_X;
            }

        }
    }
}

