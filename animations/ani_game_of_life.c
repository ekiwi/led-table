#include "animations.h"

#define chanceToLive 50 // 0 - 100

typedef struct {
	led_color living_color;
	led_color dead_color;
	uint8_t cells[8][8];
} global_data;
global_data *global_vars;


bool ani_game_of_life_init(){
	global_vars = memory_alloc(sizeof(global_data));
    if(global_vars != NULL){
        g.living_color = led_color_(0x00,0xFF,0x00);
        g.dead_color = led_color_(0x00,0x00,0x00);

        // randomly insert a couple of living cells
        // TODO randomness
        for( uint8_t x=0; x<LED_WIDTH; x++ ) {
            for( uint8_t y=0; y<LED_WIDTH; y++ ) {
                uint8_t random = led_rand() % (100/chanceToLive);
                g.cells[x][y] = random == 0 ? 1 : 0;
            }
       }
        return true;
    }
    return false;
}

void ani_game_of_life_run(led_image* canvas, uint32_t ticks, uint32_t time_elapsed) {
    clr_image(g.dead_color, canvas);

    // draw
    uint8_t num_neighbors;


    if( ticks%5 == 0 ) {
        uint8_t hasChanged = 0;
        for( uint8_t x=0; x<LED_WIDTH; x++ ) {
            for( uint8_t y=0; y<LED_WIDTH; y++ ) {
                // get number of living neighbors
                num_neighbors = 0;

                // three cells on the left side
                if( x-1 >= 0 && g.cells[x-1][y] ) num_neighbors ++;
                if( x-1 >= 0 && y-1 >= 0 && g.cells[x-1][y-1] ) num_neighbors ++;
                if( x-1 >= 0 && y+1 <= LED_WIDTH && g.cells[x-1][y+1] ) num_neighbors ++;

                // three cells on the right side
                if( x+1 <= LED_WIDTH && g.cells[x+1][y] ) num_neighbors ++;
                if( x+1 <= LED_WIDTH && y-1 >= 0 && g.cells[x+1][y-1] ) num_neighbors ++;
                if( x+1 <= LED_WIDTH && y+1 <= LED_WIDTH && g.cells[x+1][y+1] ) num_neighbors ++;

                // one cell atop
                if( y+1 <= LED_WIDTH && g.cells[x][y+1] ) num_neighbors ++;

                // one cell below
                if( y-1 >= 0 && g.cells[x][y-1] ) num_neighbors ++;

                if( g.cells[x][y] == 1 && ( num_neighbors < 2 || num_neighbors > 3  ) ) {
                    // cell dies due to under-/ overpopulation
                	g.cells[x][y] = 0;
                     hasChanged = true;
                }
                else if ( g.cells[x][y] == 0 && num_neighbors == 3 ) {
                	g.cells[x][y] = 1;
                    hasChanged = true;
                }
            }
        }
    }

        // DRAW on canvas
        for( uint8_t x=0; x<LED_WIDTH; x++ ) {
            for( uint8_t y=0; y<LED_WIDTH; y++ ) {
                set_pixel(led_point_(x,y), (g.cells[x][y] ? g.living_color : g.dead_color), canvas);
            }
        }
    }
