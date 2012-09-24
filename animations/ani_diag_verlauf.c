#include "animations.h"

typedef struct {
	led_color from_color;
	led_color to_color;
	int8_t gradToggle;
	int8_t base_step;
	int8_t step_inc;
	int8_t step_inc2;
	int8_t grad_red;
	int8_t grad_green;
	int8_t grad_blue;
} global_data;
global_data *global_vars;


#define STEPS 15

//prototypes
led_color getColor( int8_t step );
uint8_t increaseStep(uint8_t step);
void increaseBaseStep();

bool ani_diag_verlauf_init() {
	global_vars = memory_alloc(sizeof(global_data));
    if(global_vars != NULL){
        g.from_color = led_color_(0xFF, 0x00, 0x00);
        g.to_color = led_color_(0x00, 0xFF, 0x00);
        g.base_step = 0;
        g.step_inc = 1;
        g.step_inc2 = 1;
        g.gradToggle = 1;

        g.grad_red = (g.to_color.red - g.from_color.red)/STEPS;
        g.grad_green = (g.to_color.green - g.from_color.green)/STEPS;
        g.grad_blue = (g.to_color.blue - g.from_color.blue)/STEPS;
        return true;
    }
    return false;
}

void ani_diag_verlauf_run(led_image* canvas, uint32_t ticks, uint32_t time_elapsed) {
    uint8_t i;
    uint8_t step = g.base_step;

    if( g.base_step == STEPS ) {
    	g.step_inc2 = -1;
    } else if ( g.base_step == 0 ) {
    	g.step_inc2 = 1;
    } else {
        if( g.gradToggle ) {
        	g.step_inc2 = 1;
        } else {
        	g.step_inc2 = -1;
        }
    }
    for(i = 0; i < LED_WIDTH; i++) {
         // draw first 8 diagonals
         draw_line(led_point_(0,i), led_point_(i,0), getColor( step ), canvas);
         step = increaseStep(step);
     }

     for(i = 0; i <= LED_WIDTH-1; i++) {
         // draw last 8 diagonals
         draw_line(led_point_(1+i,LED_WIDTH-1), led_point_(LED_WIDTH-1,i+1), getColor( step ), canvas);
         step = increaseStep(step);
     }

     increaseBaseStep();
}

led_color getColor( int8_t step ) {
    if( step == STEPS )
    {
    	g.gradToggle = 1;
        return g.to_color;
    } else {
        if( step == 0) {
        	g.gradToggle = 0;
            return g.from_color;
        } else {
            return led_color_(
            		g.from_color.red + g.grad_red * step,
            		g.from_color.green + g.grad_green * step,
            		g.from_color.blue + g.grad_blue * step
                     );
        }
   }
}

uint8_t increaseStep(uint8_t step) {
    step += g.step_inc2;
    if( step == STEPS || step == 0 ) {
    	g.step_inc2 = -g.step_inc2;
    }
    return step;
}

void increaseBaseStep() {
	g.base_step += g.step_inc;
    if( g.base_step == STEPS || g.base_step == 0 ) {
    	g.step_inc = -g.step_inc;
    }
}
