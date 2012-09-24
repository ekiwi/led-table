#include "animations.h"

typedef struct {
	led_point start_pos;
	led_point end_pos;
	led_point start_pos2;
	led_point end_pos2;
	led_image last_img;
	led_color first_color;
	led_color second_color;
} global_data;
global_data *global_vars;


bool ani_undefined_init() {
	global_vars = memory_alloc(sizeof(global_data));
    if(global_vars != NULL){
        /* animation 1*/
        g.start_pos = led_point_(0,0);
        g.end_pos = led_point_(0,0);
        g.start_pos2 = led_point_(7,0);
        g.end_pos2 = led_point_(7,0);


        g.last_img.height = 8;
        g.last_img.width = 8;
        clr_image(led_color_(0x00,0x00,0x00),&g.last_img);


        g.first_color = led_color_(255,255,0);
        g.second_color = led_color_(7,241,254);
        return true;
    }
    return false;
}
void ani_undefined_run(led_image* canvas, uint32_t ticks, uint32_t time_elapsed){

     //clr_image(led_color_(0x00,0x00,0x00),canvas);
      //draw_line(diag_point1,diag_point2,led_color_(0xFF,0x00,0x00),canvas);

    draw_img(led_point_(0,0), &g.last_img, canvas);
    draw_line(g.start_pos,g.end_pos,g.first_color,canvas);

    g.start_pos.y += 2;
    g.end_pos.x += 2;

    if(g.start_pos.y>(LED_WIDTH + 7))
    {
    	g.start_pos.y=0;
    	g.end_pos.x=0;

    }

    draw_line(g.start_pos2, g.end_pos2, g.second_color, canvas);

    g.start_pos2.x -= 2;
    g.end_pos2.y += 2;

    if(g.start_pos2.x<-7){
    	g.start_pos2.x = 7;
    	g.end_pos2.y = 0;

    }

    if(g.start_pos.y == 0){
           clr_image(led_color_(0x00,0x00,0x00),&g.last_img);
           uint8_t delta = 0;
           while( delta < 230 ) {
        	   g.first_color = led_color_rand();
        	   g.second_color = led_color_rand();
               delta = abs( g.first_color.red - g.second_color.red );
               delta += abs( g.first_color.green - g.second_color.green );
               delta += abs( g.first_color.blue - g.second_color.blue );
           }

    }
    else {
        draw_img(led_point_(0,0), canvas, &g.last_img);
    }
}



