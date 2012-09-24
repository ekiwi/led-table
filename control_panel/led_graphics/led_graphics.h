/*
 * led_graphics.h
 *
 *  Created on: Apr 10, 2012
 *      Author: kevin
 */

#ifndef LED_GRAPHICS_H
#define LED_GRAPHICS_H

// Include Hardware Abstraction Layer
#include "hal.h"


/* ---------------------------- LED Graphics Settings ------------------------ */
#define LED_HEIGHT 8
#define LED_WIDTH 8
#define MEMORY_SIZE 2048
/* ---------------------------- ---------------------------------------------- */


// ---------------------- LED Graphics Macros ---------------------------
#define TRIG_INT_SIN_90 255
#define max(a,b)    (((a) > (b)) ? (a) : (b))
#define min(a,b)    (((a) < (b)) ? (a) : (b))
#define abs(a)      (((a) < 0) ? -(a) : (a))
#define max3(a,b,c) (((b) >= (c)) ? (((a) >= (b)) ? (a) : (b)) : (((a) >= (c)) ? (a) : (c)))
#define min3(a,b,c) (((b) <= (c)) ? (((a) <= (b)) ? (a) : (b)) : (((a) <= (c)) ? (a) : (c)))

// ---------------------- Colors ---------------------------
typedef struct{
    uint8_t red;
    uint8_t green;
    uint8_t blue;
} led_color;

typedef struct{
    uint8_t hue;
    uint8_t saturation;
    uint8_t value;
} led_color_hsv;

// ---------------------- Default Colors ---------------------------
#define RED led_color_(0xFF,0x00,0x00)
#define GREEN led_color_(0x00,0xFF,0x00)
#define BLUE led_color_(0x00,0x00,0xFF)
#define BLACK led_color_(0x00,0x00,0x00)

#define RED_INDEX 		0
#define GREEN_INDEX 	1
#define BLUE_INDEX 		2

typedef struct{
    union{
        led_color pixel [LED_WIDTH][LED_HEIGHT];
        uint8_t   raw_data [LED_WIDTH][LED_HEIGHT][3];
    };
    uint8_t width;
    uint8_t height;
    led_color transparency_key;
} led_image;

typedef struct{
    int8_t x1;
    int8_t y1;
    int8_t x2;
    int8_t y2;
} led_rect;

typedef struct{
    int8_t x;
    int8_t y;
} led_point;


/* ------------------------- Animation Pointer Definitions ------------------------ */
typedef bool (*ani_init_ptr)(void);
typedef void (*ani_run_ptr)(led_image*, uint32_t, uint32_t);

typedef struct{
    ani_init_ptr init;
    ani_run_ptr run;
}animation;


/* ---------------------------- "Constructors" ---------------------------------- */
led_color led_color_(uint8_t red, uint8_t green, uint8_t blue);
led_rect led_rect_(int8_t x,int8_t y, uint8_t width, uint8_t height);
led_point led_point_(int8_t x,int8_t y);
led_color hsv_to_rgb(led_color_hsv hsv);


/* ---------------------------- Rect Functions ---------------------------------- */
void rect_grow(uint8_t n, led_rect* rec);
void rect_shrink(uint8_t n, led_rect* rec);
void rect_move(uint8_t x, uint8_t y, led_rect* rec);
void load_image(uint8_t* source, led_image* dest);



/* ---------------------------- Graphics Functions ---------------------------------- */
led_color led_color_rand();     // returns a random color
int16_t sin_(uint16_t angle);   // returns the sine of the angle specified; range: 0...255
int16_t cos_(uint16_t angle);   // returns the cosine of the angle specified; range: 0...255
void draw_rect(led_rect rec, led_color color, led_image* img);  // draws a rect angle
void fill_rect(led_rect rec, led_color color, led_image* img);
void fill_inner_rect(led_rect rec, led_color color, led_image* img);
void draw_char(led_point loc, uint8_t c, led_color color, led_image* img);
void copy_clip(led_rect source_rec, led_point dest_pt, led_image* source_img, led_image* dest_img);
void copy_clip_trans(led_rect source_rec, led_point dest_pt, led_image* source_img, led_image* dest_img, led_color transparency_key);
void draw_img(led_point dest_pint, led_image* source_img, led_image* dest_img);
void draw_line(led_point begin, led_point end, led_color color, led_image* img);
void set_pixel(led_point pixel, led_color color, led_image* img);
void clr_image(led_color color, led_image* img);
led_color get_pixel(led_point pixel, led_image* img);

/* ---------------------------- LED Memmory ---------------------------------- */
#define g (*global_vars)

extern uint8_t led_memory[MEMORY_SIZE];

void memory_clear();    // clears the led_memory block
void* memory_alloc(size_t length);

// Image Buffer
extern led_image img_buffer;
/* --------------------------------------------------------------------------- */



#endif // LED_GRAPHICS_H
