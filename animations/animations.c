#include "animations.h"

// ------------------ Registry Table --------------------------------------
animation ani_table[ANIMATION_NUMBER+1] = { // add +1 for the error animation
    {ani_spirale_init, ani_spirale_run},    			// Animation 00
    {ani_default_init, ani_default_run},    			// Animation 01
    {ani_pong_init, ani_pong_run},    					// Animation 02
    {ani_game_of_life_init, ani_game_of_life_run},    	// Animation 03
    {ani_equalizer_init, ani_equalizer_run},            // Animation 04
    {ani_default_init, ani_default_run},    			// Animation 05
    {ani_color_cycle_init, ani_color_cycle_run},    	// Animation 06
    {ani_diag_verlauf_init, ani_diag_verlauf_run},    	// Animation 07
    {ani_laufschrift_init, ani_laufschrift_run},    	// Animation 08
    {ani_psy_init, ani_psy_run},    					// Animation 09
    {ani_rect_init, ani_rect_run},    					// Animation 10
    {ani_undefined_init, ani_undefined_run},    		// Animation 11
    {ani_jump_init, ani_jump_run},                      // Animation 12
    {ani_default_init, ani_default_run},    // Animation 13
    {ani_default_init, ani_default_run},    // Animation 14
    {ani_default_init, ani_default_run},    // Animation 15
    {ani_default_init, ani_default_run},    // Animation 16
    {ani_default_init, ani_default_run},    // Animation 17
    {ani_default_init, ani_default_run},    // Animation 18
    {ani_default_init, ani_default_run},    // Animation 19
    {ani_error_init, ani_error_run}         // the last animation is always the error animation
};

