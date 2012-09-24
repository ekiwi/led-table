#include "animations.h"


#define MAP_WIDTH 24
#define SKY_COLOR led_color_(0x00,0x00,0xFF)
#define GROUND_COLOR led_color_(36,36,0)
#define PLATFORM_COLOR led_color_(0xFF,0x00,0x00)


typedef struct {
    uint8_t map[MAP_WIDTH];
    uint8_t map_x;
    uint8_t point_y;
	
} global_data;
global_data *global_vars;




bool ani_jump_init(){
    global_vars = memory_alloc(sizeof(global_data));
    if(global_vars != NULL){
        g.map_x = 0;
        g.point_y = 0;
        for(uint8_t i = 0; i < 5; i++){
            g.map[i] = (1<<4);
        }
        for(uint8_t i = 5; i < 12; i++){
        	g.map[i] = (1<<6);
        }
        for(uint8_t i = 12; i < 19; i++){
        	g.map[i] = (1<<0);
        }
        for(uint8_t i = 19; i < MAP_WIDTH; i++){
        	g.map[i] = (1<<3);
        }
        return true;
    }
    return false;
}

// prototype
void create_map(uint8_t* map, uint8_t x);
void draw_map(uint8_t* map, uint8_t x);
//uint8_t add_platform(uint8_t* map, uint8_t y, uint8_t start_length);

void ani_jump_run(led_image* canvas, uint32_t ticks, uint32_t time_elapsed){	
    draw_map(g.map, g.map_x);
}

void draw_map(uint8_t* map, uint8_t x){
    for(uint8_t i = 0; i < LED_WIDTH; i++){
        uint8_t y = 3;

    }
}


void move_map(){
    g.map_x++;
    if(g.map_x > MAP_WIDTH - LED_WIDTH){
        g.map_x = 0;
        // copy map

    }
}

void create_map(uint8_t* map, uint8_t x){
    uint8_t lp_length = 0, lp_y = 2;
    if(x > 0){  // if map already exists
        // Analyze map
        for(uint8_t y = 0; y < 8; y++){
            if(map[x-1] == (1<<y)){
                lp_y = y;
            }
        }
        lp_length = 1;
        while(x > 0 && map[x-1] == (1<<lp_y)){
            x++;
            lp_length++;
        }
    }
    /*
    if(lp_length < 3){  // if the last platform was small => extend
        add_platform(map, lp_y, )
    }
    */
}
/*
uint8_t add_platform(uint8_t* map, uint8_t y, uint8_t start_length){
    uint8_t length;
    while((length = led_rand()>>5-start_length) < 3);    // => max length 7; min length 3
    for(uint8_t i = y; i < y + length; i++){
        if(i < MAP_WIDTH){
            map[i] = (1<<y);
        }
    }
    return length;
}
*/
