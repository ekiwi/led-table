#include "animations.h"

typedef struct {
	led_point ball_start;
	led_point ball_current;
	led_point ball_move;
	int8_t p1_y;
	int8_t p2_y;
} global_data;
global_data *global_vars;

// prototypes
void changePosition(uint8_t left_player /* boolean */, uint8_t up /* boolean */);
void resetGame();
uint8_t calc_y_by_x(uint8_t x);

#define PLAYER_SIZE 3
#define BALL_SPEED 2 /* the more the slower */
#define AI_SPEED 3 /* the more the slower */

bool ani_pong_init() {
	global_vars = memory_alloc(sizeof(global_data));
    if(global_vars != NULL){
        g.ball_start = led_point_(LED_WIDTH/2-1, LED_WIDTH/2);
        resetGame();
        return true;
    }
    return false;

}

void ani_pong_run(led_image* canvas, uint32_t ticks, uint32_t time_elapsed) {

    // draw
    clr_image(led_color_(0x00,0x00,0x00), canvas);

    // player 1
    draw_line( led_point_(0, g.p1_y), led_point_(0, g.p1_y+PLAYER_SIZE-1), led_color_(0xFF,0xFF,0xFF), canvas );

    // player 2
    draw_line( led_point_(LED_WIDTH-1, g.p2_y),  led_point_(LED_WIDTH-1, g.p2_y + PLAYER_SIZE-1), led_color_(0xFF,0xFF,0xFF), canvas );

    // ball
    set_pixel( g.ball_current, led_color_(0xFF,0x00,0x00), canvas );

    // handle ball
    uint8_t next_move;
    if( ticks % BALL_SPEED == 0 ) {

        // ball lost
        if( g.ball_current.x == 0 || (g.ball_current.x == LED_WIDTH-1) ) {
            resetGame();
        } else {

            // ball catched
            next_move  = g.ball_current.y + g.ball_move.y;

            // ... by player left
            if( g.ball_current.x == 1 && next_move >= g.p1_y && next_move <= g.p1_y+PLAYER_SIZE-1 ) {
            	g.ball_move.x *= -1;
            }

            // ... by player right
            if( g.ball_current.x == LED_WIDTH-2 && next_move >= g.p2_y && next_move <= g.p2_y+PLAYER_SIZE-1 ) {
            	g.ball_move.x *= -1;
            }

            // ball is moving further
            g.ball_current.x += g.ball_move.x;
            g.ball_current.y += g.ball_move.y;
            if( g.ball_current.y == 0 || (g.ball_current.y == LED_HEIGHT-1) ) {
            	g.ball_move.y *= -1;
            }
        }
    }

    // handle AI
    if( ticks % AI_SPEED == 0 ) {
        // Player 1 is dumb.
        uint8_t next_move1 = g.ball_current.y + g.ball_move.y;
        if( next_move1 > g.p1_y ) {
            changePosition(true,false);
        } else if( next_move1 < g.p1_y ){
            changePosition(true,true);
        }
        // Player 2 is clever.
        uint8_t next_move2 = calc_y_by_x(LED_WIDTH -1);
        //deb_2("Recommended y-value: ", next_move2);
        if( next_move2 != -1 ) {
            if( next_move2 > g.p2_y ) {
                changePosition(false,false);
            } else if( next_move2 < g.p2_y ){
                changePosition(false,true);
            }
        }
    }

}

void changePosition(uint8_t left_player /* boolean */, uint8_t up /* boolean */) {
    if( left_player ) {
        if( up && g.p1_y > 0) {
        	g.p1_y --;
        } else if( g.p1_y < LED_HEIGHT-PLAYER_SIZE){
        	g.p1_y ++;
        }
    } else {
        if( up && g.p2_y > 0) {
        	g.p2_y --;
        } else if( g.p2_y < LED_HEIGHT-PLAYER_SIZE){
        	g.p2_y ++;
        }
    }
}

void resetGame() {
	g.ball_current = g.ball_start;
	g.p1_y = (LED_HEIGHT-1-PLAYER_SIZE)/2+1;
	g.p2_y = g.p1_y;
	g.ball_move = led_point_( (led_rand() % 2 == 0 ? 1 : -1), (led_rand() % 2 == 0 ? 1 : -1) );
    if( led_rand() % 2 == 0 ) { g.ball_current.x += 1; }
}

uint8_t calc_y_by_x(uint8_t x) {
    led_point temp_ball_current = g.ball_current;
    led_point temp_ball_move = g.ball_move;
    while( 1 ) {

        // same algorithm as top

        // ball lost
        if( temp_ball_current.x == 0 || (temp_ball_current.x == LED_WIDTH-1) ) {
            return -1;
        } else {

            // ball catched
            uint8_t next_move  = temp_ball_current.y + temp_ball_move.y;

            // ... by player left
            if( temp_ball_current.x == 1 && next_move >= g.p1_y && next_move <= g.p1_y+PLAYER_SIZE-1 ) {
                temp_ball_move.x *= -1;
            }

            // ball is moving further
            temp_ball_current.x += temp_ball_move.x;
            temp_ball_current.y += temp_ball_move.y;
            if( temp_ball_current.y == 0 || (temp_ball_current.y == LED_HEIGHT-1) ) {
                temp_ball_move.y *= -1;
            }
        }

        if( temp_ball_current.x == x ) return temp_ball_current.y;
    }
}
