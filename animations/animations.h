#ifndef ANIMATION_REGISTRY_H
#define ANIMATION_REGISTRY_H


#include "led_graphics.h"

#define ANIMATION_NUMBER 20
#define START_ANIMATION 8

/* Animation Pointers
*  - To add  new Animation, include header file and add define
*  		- add prototypes
*  		- add function pointers to table in "animations.c"
*/

#define ANI_LAUFSCHRIFT 8

// ------------------- Prototypes -----------------------------------------
bool ani_spirale_init();
void ani_spirale_run(led_image* canvas, uint32_t ticks, uint32_t time_elapsed);
bool ani_default_init();
void ani_default_run(led_image* canvas, uint32_t ticks, uint32_t time_elapsed);
bool ani_pong_init();
void ani_pong_run(led_image* canvas, uint32_t ticks, uint32_t time_elapsed);
bool ani_game_of_life_init();
void ani_game_of_life_run(led_image* canvas, uint32_t ticks, uint32_t time_elapsed);
bool ani_equalizer_init();
void ani_equalizer_run(led_image* canvas, uint32_t ticks, uint32_t time_elapsed);
bool ani_default_init();
void ani_default_run(led_image* canvas, uint32_t ticks, uint32_t time_elapsed);
bool ani_color_cycle_init();
void ani_color_cycle_run(led_image* canvas, uint32_t ticks, uint32_t time_elapsed);
bool ani_diag_verlauf_init();
void ani_diag_verlauf_run(led_image* canvas, uint32_t ticks, uint32_t time_elapsed);
bool ani_laufschrift_init();
void ani_laufschrift_run(led_image* canvas, uint32_t ticks, uint32_t time_elapsed);
bool ani_psy_init();
void ani_psy_run(led_image* canvas, uint32_t ticks, uint32_t time_elapsed);
bool ani_rect_init();
void ani_rect_run(led_image* canvas, uint32_t ticks, uint32_t time_elapsed);
bool ani_undefined_init();
void ani_undefined_run(led_image* canvas, uint32_t ticks, uint32_t time_elapsed);
bool ani_error_init();
void ani_error_run(led_image* canvas, uint32_t ticks, uint32_t time_elapsed);
bool ani_jump_init();
void ani_jump_run(led_image* canvas, uint32_t ticks, uint32_t time_elapsed);
// ------------------ Registry Table --------------------------------------
#define ERROR_ANIMATION ANIMATION_NUMBER
extern animation ani_table[ANIMATION_NUMBER+1];

#endif // ANIMATION_REGISTRY_H
