#include "animations.h"

#define BG_COLOR led_color_(0x00,0x00,0x00)
#define LINE_COLOR led_color_(0xFF,0xFF,0xFF)

typedef struct {
	int8_t count;
	led_image bg;
	led_color color0;
	led_color color1;
	led_color color2;
	led_color color3;
	led_rect test_rec;
} global_data;
global_data *global_vars;


void colorizeHalfLine(led_point p0, led_point p1, led_color color, led_image* img);
void setColorCycleColors();

bool ani_color_cycle_init() {
	global_vars = memory_alloc(sizeof(global_data));
    if(global_vars != NULL){
        g.count = 0;
        g.bg.height = 8;
        g.bg.width = 8;
        setColorCycleColors();
        return true;
    }
    return false;
}

void ani_color_cycle_run(led_image* canvas, uint32_t ticks, uint32_t time_elapsed) {
    clr_image(BG_COLOR, canvas);

    led_point line1_start = led_point_(g.count,0);
    led_point line1_end = led_point_(7-g.count,7);

    led_point line2_start = led_point_(7,g.count);
    led_point line2_end = led_point_(0,7-g.count);

    // draw background
    draw_img(led_point_(0,0),&g.bg,canvas);

    colorizeHalfLine(line1_start, line1_end, g.color0, &g.bg);
    colorizeHalfLine(line2_start, line2_end, g.color1, &g.bg);

    // draw lines
    draw_line( line1_start, line1_end, LINE_COLOR, canvas );
    draw_line( line2_start, line2_end, LINE_COLOR, canvas );

    if( g.count != 7 ) {
    	g.count ++;
    } else {
    	g.count = 1;
        setColorCycleColors();
    }

}


void colorizeHalfLine(led_point p0, led_point p1, led_color color, led_image* img){
    int8_t dx, dy, sx, sy, error, error2;
    dx = abs(p1.x - p0.x);
    dy = abs(p1.y - p0.y);
    if(p0.x < p1.x) sx = 1; else sx = -1;
    if(p0.y < p1.y) sy = 1; else sy = -1;
    error = dx-dy;
    while(1){
        if(p0.x >= 0 && p0.x < img->width && p0.y >= 0 && p0.y < img->height){
            img->pixel[p0.x][p0.y] = color;
        }
        if(p0.x == p1.x && p0.y == p1.y){
            return;
        }
        error2 = error << 1;
        if(error2 > -dy){
            error -= dy;
            p0.x = p0.x + sx;
        }
        if(error2 < dx){
            error += dx;
            p0.y = p0.y + sy;
        }
    }
}

void setColorCycleColors() {
	g.color0 = led_color_rand();
	g.color1 = led_color_rand();
	g.color2 = led_color_rand();
	g.color3 = led_color_rand();
}
