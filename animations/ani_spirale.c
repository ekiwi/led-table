#include "animations.h"

typedef struct {
	int8_t step;
	int8_t depth;
	int8_t flag_first_line;
	int8_t depthTrigger;
	led_point cur_point;
	led_color line_color;
	led_image last_image;
} global_data;
global_data *global_vars;


void handleEndPoint();


bool ani_spirale_init() {
	global_vars = memory_alloc(sizeof(global_data));
    if(global_vars != NULL){
        g.cur_point = led_point_(0,0);
        g.depth = 0;
        g.step = 0;
        g.flag_first_line = 1;
        g.line_color = led_color_(0xFF,0x00,0x00);
        g.last_image.height = 8;
        g.last_image.width = 8;
        clr_image(led_color_(0x00,0x00,0x00), &g.last_image);
        return true;
    }
    return false;
}

void ani_spirale_run(led_image* canvas, uint32_t ticks, uint32_t time_elapsed) {

    draw_img(led_point_(0,0), &g.last_image, canvas);

    uint8_t max_l = LED_WIDTH - 1 - g.depth;

    if( g.flag_first_line ) {

        set_pixel( g.cur_point, g.line_color, canvas );

        g.cur_point.x += 1;

        if( g.cur_point.x > max_l ) {
        	g.flag_first_line = 0;
        	g.step = 1;
        	g.cur_point.x -= 1;
        }
        draw_img(led_point_(0,0), canvas, &g.last_image);
        return;
    }

    switch( g.step ) {
        case 0: // horizontal right
        	g.cur_point.x += 1;
            set_pixel( g.cur_point, g.line_color, canvas );

            if( g.cur_point.x == max_l) {
            	g.step += 1;
            }
        break;
        case 1: // vertical down
        	g.cur_point.y += 1;
            set_pixel( g.cur_point, g.line_color, canvas );

            if( g.cur_point.y == LED_HEIGHT - 1 - g.depth) {
            	g.step += 1;
            }

        break;
        case 2: // horizontal left
        	g.cur_point.x -= 1;
            set_pixel( g.cur_point, g.line_color, canvas );

            if( g.cur_point.x == g.depth) {
            	g.step += 1;
            	g.depth++;
            }

        break;
        case 3: // vertical up
        	g.cur_point.y -= 1;
            set_pixel( g.cur_point, g.line_color, canvas );

            if( g.cur_point.y == g.depth) {
            	g.step = 0;
            }
        break;
    }

    draw_img(led_point_(0,0), canvas, &g.last_image);

    if( g.cur_point.x == 3 && g.cur_point.y == 4 ) {
        clr_image(led_color_(0x00,0x00,0x00), &g.last_image);
        g.depth = 0;
        g.step = 0;
        g.flag_first_line = 1;
        g.cur_point = led_point_(0,0);
    }

}
