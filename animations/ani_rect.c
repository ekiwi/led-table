#include "animations.h"

typedef struct {
	led_color color_0;
	led_color color_1;
	led_color color_2;
	led_color color_3;
	int8_t counter;
} global_data;
global_data *global_vars;


#define colors {led_color_(242,254,29),led_color_(31,239,239),led_color_(249,18,6),led_color_(254,96,205),led_color_(167,48,167),led_color_(125,252,12)}
#define ARRAY_LENGTH 6

void setRectColors();
uint8_t increaseCounter(uint8_t value);

bool ani_rect_init() {
	global_vars = memory_alloc(sizeof(global_data));
    if(global_vars != NULL){
        g.counter = 0;
        setRectColors();
        return true;
    }
    return false;

}

void ani_rect_run(led_image* canvas, uint32_t ticks, uint32_t time_elapsed) {


    led_rect rec_3 = {0,0,7,7};
    led_rect rec_2 = {1,1,6,6};
    led_rect rec_1 = {2,2,5,5};
    led_rect rec_0 = {3,3,4,4};

        setRectColors();


    draw_rect(rec_3, g.color_3, canvas);
    draw_rect(rec_2, g.color_2, canvas);
    draw_rect(rec_1, g.color_1, canvas);
    draw_rect(rec_0, g.color_0, canvas);

}

void setRectColors(){
    led_color tmp[ARRAY_LENGTH] = colors;
    uint8_t tmp_count = g.counter;

    g.color_0 = tmp[tmp_count];
    tmp_count = increaseCounter( tmp_count );

    g.color_1 = tmp[tmp_count];
    tmp_count = increaseCounter( tmp_count );

    g.color_2 = tmp[tmp_count];
    tmp_count = increaseCounter( tmp_count );

    g.color_3 = tmp[tmp_count];

    g.counter = increaseCounter(g.counter);
    return;
}

uint8_t increaseCounter(uint8_t value) {
    return value+1 > ARRAY_LENGTH-1 ? 0 : value+1;
}
