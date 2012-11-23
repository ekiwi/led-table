#include "animations.h"

/*
 * struct that contains all global variables
 */
typedef struct {

} global_data;
global_data *global_vars;

/*
 * Is called once when the animation is loaded (i.e. selected by the user).
 */
bool ani_default_init(){
    global_vars = memory_alloc(sizeof(global_data));    // allocate memory for the global variables

    if(global_vars != NULL){
        /* initialize global vars here */
        return true;
    }

    return false;
}

/*
 * Is called every time a new image is needed. This happens about 50 times a second.
 */
void ani_default_run(led_image* canvas, uint32_t ticks, uint32_t time_elapsed){
    clr_image(led_color_(0x00,0x00,0x00), canvas);  // clear screen with black
    draw_rect(led_rect_(0,0,8,8), led_color_(0xFF,0x00,0x00), canvas);  // draws a red rectangle on the screen
}



