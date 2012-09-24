#include "animations.h"

typedef struct {
	led_point ball_pos;
	led_point ball_speed;
	led_point line_0;
	led_point line_1;
	led_image background_pic;
	led_image ball_pic;
	led_color color_0;
	led_color color_1;
	led_color color_2;
	led_color color_3;
	int8_t d_line;
} global_data;
global_data *global_vars;


bool ani_default_init(){
	global_vars = memory_alloc(sizeof(global_data));
    //load_image(test, &g.background_pic);
    //load_image(test4, &g.ball_pic);
    if(global_vars != NULL){

        g.ball_pos.x = 1;
        g.ball_pos.y = 1;
        /*
        g.ball_speed.x = 1;
        g.ball_speed.y = 1;
        g.color_3 = led_color_rand();
        g.color_2 = led_color_rand();
        g.color_1 = led_color_rand();
        g.color_0 = led_color_rand();
        g.line_0.x = 0; g.line_0.y = 0;
        g.line_1.x = 7; g.line_1.y = 7;
        g.d_line = 1;
        */
        return true;
    }
    return false;
}

void ani_default_run(led_image* canvas, uint32_t ticks, uint32_t time_elapsed){
    clr_image(led_color_(0x00,0x00,0x00), canvas);
    draw_char(g.ball_pos, 'E', led_color_(0xFF,0xFF,0x00), canvas);
    //draw_char(led_point_(g.ball_pos.x + 8, g.ball_pos.y), 'b', led_color_(0xFF,0xFF,0x00), canvas);
    //draw_char(ball_pos, 'i', led_color_(0xFF,0xFF,0x00), canvas);
    //draw_char(ball_pos, ' ', led_color_(0xFF,0xFF,0x00), canvas);
    //draw_char(ball_pos, '1', led_color_(0xFF,0xFF,0x00), canvas);
    //draw_char(ball_pos, '2', led_color_(0xFF,0xFF,0x00), canvas);
    if(ticks % 1 == 0){
        /*
    	ball_pos.x += d_line;
        //ball_pos.y += d_line;
        if(ball_pos.x == 10 || ball_pos.x == -7){
            d_line = -d_line;
        }
        */
    }


/*
    led_point punkt =  {1,1};
    led_color color = {0x00, 0xFF, 0x00};
    set_pixel(punkt, color, canvas);

        int test = 6;

        if( test == 2 ) {

        } else {
            led_point punkt =  {1,1};
            led_color color = {0xFF, 0x00, 0x00};
            set_pixel(punkt, color, canvas);
        }
*/

     /* Ball Animation - Uncomment to View

    led_color ball_color = {0xFF, 0x00, 0x00};
    led_point origin_pt = {0,0};

    if(ticks % 2 == 0){
        ball_pos.x += ball_speed.x;
        ball_pos.y += ball_speed.y;
        if(ball_pos.x == 0 || ball_pos.x >= LED_WIDTH-1) ball_speed.x = -ball_speed.x;
        if(ball_pos.y == 0 || ball_pos.y >= LED_HEIGHT-1) ball_speed.y = -ball_speed.y;
    }

    draw_img(origin_pt, &background_pic, canvas);
    set_pixel(ball_pos, ball_color, canvas);*/


    /* Line Animation - Uncomment to View

    led_color line_color = {0xFF, 0xFF, 0xFF};
    led_color back_color = {0x00, 0x00, 0x00};
    line_0.y += d_line;
    line_1.y -= d_line;
    if(d_line > 0 && line_0.y + d_line >= LED_HEIGHT) d_line = -1;
    if(d_line < 0 && line_0.y + d_line < 0) d_line = 1;
    clr_image(back_color, canvas);
    draw_line(line_0, line_1, line_color, canvas);

*/
    /* Rectangle Animation - Uncomment to View
    led_rect rec_3 = {0,0,7,7};
    led_rect rec_2 = {1,1,6,6};
    led_rect rec_1 = {2,2,5,5};
    led_rect rec_0 = {3,3,4,4};
    if(ticks % 2 == 0){
        color_3 = color_2;
        color_2 = color_1;
        color_1 = color_0;
        color_0 = led_color_rand();
    }

    draw_rect(rec_3, color_3, canvas);
    draw_rect(rec_2, color_2, canvas);
    draw_rect(rec_1, color_1, canvas);
    draw_rect(rec_0, color_0, canvas);
*/

    /*---------Stuff---------------*/
    /*
    led_color red = {0xFF, 0x00, 0x00};
    led_color green = {0x00, 0xFF, 0x00};
    led_color blue = {0x00, 0x00, 0xFF};
    led_color c;
    uint32_t time = time_elapsed % 3000;
    if(time < 1000) c = red;
    else if(time < 2000) c = green;
    else c = blue;
    clr_image(c, canvas);
    */
    //led_rect normal_rec = {1,1,6,6};
    //led_point origin_pt = {-3,-3};
    //draw_img(origin_pt, &background_pic, canvas);
    //copy_clip(normal_rec, origin_pt, &background_pic, canvas);
    //draw_img(ball_pos, &ball_pic, canvas);

    //led_color red = {0xFF, 0x00, 0x00};
    //led_color green = {0x00, 0xFF, 0x00};
    //led_color blue = {0x00, 0x00, 0xFF};

}



