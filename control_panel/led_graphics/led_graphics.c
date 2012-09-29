/*
 * led_graphics.c
 *
 *  Created on: Apr 10, 2012
 *      Author: kevin
 */

#include "led_graphics.h"
#include "resources/font_ubuntu.h"

led_color led_color_rand(){
    led_color c;
    c.red = led_rand();
    c.green = led_rand();
    c.blue = led_rand();
    return c;
}



// ------------- Trigonometry Functions ----------------

static uint8_t sin_0_90_table[91] =
{
                0, 4, 9, 13, 18, 22, 27, 31, 35, 40, 44, 49, 53, 57, 62, 66, 70, 75, 79, 83, 87, 91, 96,
                100, 104, 108, 112, 116, 120, 124, 127, 131, 135, 139, 143, 146, 150, 153, 157, 160, 164,
                167, 171, 174, 177, 180, 183, 186, 190, 192, 195, 198, 201, 204, 206, 209, 211, 214, 216,
                219, 221, 223, 225, 227, 229, 231, 233, 235, 236, 238, 240, 241, 243, 244, 245, 246, 247,
                248, 249, 250, 251, 252, 253, 253, 254, 254, 254, 255, 255, 255, 255
};


int16_t sin_(uint16_t angle){
        uint8_t quadrant;
        quadrant = (angle / 90) % 4;
        angle %= 90;
        switch(quadrant){
        case 0:
                return sin_0_90_table[angle];
        case 1:
                return sin_0_90_table[90-angle];
        case 2:
                return -sin_0_90_table[angle];
        case 3:
                return -sin_0_90_table[90-angle];
        }
        return 0;
}

int16_t cos_(uint16_t angle){
        return sin_(angle+90);
}


// ----------- Draw Functions----------------
#define compare_color(c1,c2) (c1.red == c2.red && c1.green == c2.green && c1.blue == c2.blue)
#define is_black(c) (c.red == 0 && c.green == 0 && c.blue == 0)
#define copy_pixel(source, dest, trans) if(!compare_color(trans, source)) { dest = source; }


void draw_rect(led_rect rec, led_color color, led_image* img){
    for(int y = max(rec.y1, 0); y < min(rec.y2+1, img->height); y++){
        for(int x = max(rec.x1, 0); x < min(rec.x2+1, img->width); x++){
            if(y == rec.y1 || y == rec.y2 || x == rec.x1 || x == rec.x2){
                img->pixel[x][y] = color;
            }
        }
    }
}

void fill_rect(led_rect rec, led_color color, led_image* img){
    for(int y = max(rec.y1, 0); y < min(rec.y2+1, img->height); y++){
        for(int x = max(rec.x1, 0); x < min(rec.x2+1, img->width); x++){
            img->pixel[x][y] = color;
        }
    }
}

void fill_inner_rect(led_rect rec, led_color color, led_image* img){
    for(int y = max(rec.y1 + 1, 0); y < min(rec.y2, img->height); y++){
        for(int x = max(rec.x1 + 1, 0); x < min(rec.x2, img->width); x++){
            img->pixel[x][y] = color;
        }
    }
}



// "constructors"
led_color led_color_(uint8_t red, uint8_t green, uint8_t blue){
    led_color c;
    c.red = red;
    c.green = green;
    c.blue = blue;
    return c;
}

led_point led_point_(int8_t x,int8_t y){
    led_point p;
    p.x = x;
    p.y = y;
    return p;
}

led_rect led_rect_(int8_t x,int8_t y, uint8_t width, uint8_t height){
    led_rect r;
    r.x1 = x;
    r.y1 = y;
    r.x2 = width - 1 + x;
    r.y2 = height - 1 + y;
    return r;
}

void rect_grow(uint8_t n, led_rect* rec){
    rec->x1 -= n;
    rec->y1 -= n;
    rec->x2 += n;
    rec->y2 += n;
}

void rect_shrink(uint8_t n, led_rect* rec){
    rec->x1 += n;
    rec->y1 += n;
    rec->x2 -= n;
    rec->y2 -= n;
}

void rect_move(uint8_t x, uint8_t y, led_rect* rec){
    rec->x1 += x;
    rec->y1 += y;
    rec->x2 += x;
    rec->y2 += y;
}

led_color hsv_to_rgb(led_color_hsv hsv){
    led_color rgb;
    // find min / max rgb
    return rgb;
}

// make the origin at least zero
static inline void rect_pos(led_rect* rect){
    rect->x1 = max(rect->x1, 0);
    rect->y1 = max(rect->y1, 0);
}

// makes sure that the rect is not going over the image borders
static inline void fit_rect(led_rect* rect, led_image* img){
    rect->x1 = max(rect->x1, 0);
    rect->y1 = max(rect->y1, 0);
    rect->x2 = min(rect->x2, img->width -1);
    rect->y2 = min(rect->y2, img->height-1);
}

#define LETTERS_LENGTH 0x7F

void draw_char(led_point loc, uint8_t c, led_color color, led_image* img){
    if(c < LETTERS_LENGTH){
        led_rect rec = led_rect_(loc.x, loc.y, 8, 8);
        fit_rect(&rec, img);
        // print character
        for(uint8_t y = rec.y1; y <= rec.y2; y++){
            uint8_t line = pgm_read_byte(&ubuntu[c][y-loc.y]);
            for(uint8_t x = rec.x1;  x <= rec.x2; x++){
                if(line & (1<<(x-loc.x))){
                     img->pixel[x][y] = color;
                }
            }
        }
    }
}

void copy_clip_trans(led_rect source_rec, led_point dest_pt, led_image* source_img, led_image* dest_img, led_color transparency_key){
    uint8_t transparency_used = !is_black(transparency_key);
    fit_rect(&source_rec, source_img);  // fit the source rec to the source img
    led_point offset;
    offset.x = dest_pt.x - source_rec.x1;
    offset.y = dest_pt.y - source_rec.y1;
    for(uint8_t y = max(source_rec.y1, -offset.y); y < min(source_rec.y2+1, dest_img->height - offset.y); y++){
        for(uint8_t x = max(source_rec.x1, -offset.x); x < min(source_rec.x2+1, dest_img->width - offset.x); x++){
            if(transparency_used){
                copy_pixel(source_img->pixel[x][y], dest_img->pixel[x+offset.x][y+offset.y], transparency_key);
            }
            else {
                dest_img->pixel[x+offset.x][y+offset.y] = source_img->pixel[x][y];
            }
        }
    }
}

void copy_clip(led_rect source_rec, led_point dest_pt, led_image* source_img, led_image* dest_img){
    copy_clip_trans(source_rec, dest_pt, source_img, dest_img, source_img->transparency_key);
}

void draw_img(led_point dest_point, led_image* source_img, led_image* dest_img){
    uint8_t transparency_used = !is_black(source_img->transparency_key);
    for(uint8_t y = max(0, -dest_point.y); y < min(source_img->height, dest_img->height - dest_point.y); y++){
        for(uint8_t x = max(0, -dest_point.x); x < min(source_img->width, dest_img->width - dest_point.x); x++){
            if(transparency_used){
                copy_pixel(source_img->pixel[x][y], dest_img->pixel[x+dest_point.x][y+dest_point.y], source_img->transparency_key);
            }
            else {
                dest_img->pixel[x+dest_point.x][y+dest_point.y] = source_img->pixel[x][y];
            }
        }
    }
}


void draw_line(led_point p0, led_point p1, led_color color, led_image* img){
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

void set_pixel(led_point pixel, led_color color, led_image* img){
    if(pixel.x < 0 || pixel.x >= img->width) return;
    if(pixel.y < 0 || pixel.y >= img->height) return;
    img->pixel[pixel.x][pixel.y] = color;
}

led_color get_pixel(led_point pixel, led_image* img){
    if(pixel.x < 0 || pixel.x >= img->width) return BLACK;
    if(pixel.y < 0 || pixel.y >= img->height) return BLACK;
    return img->pixel[pixel.x][pixel.y];
}

void clr_image(led_color color, led_image* img){
    for(int y = 0; y < img->height; y++){
        for(int x = 0; x < img->width; x++){
            img->pixel[x][y] = color;
        }
    }
}

void load_image(uint8_t* source, led_image* dest){
    dest->width = pgm_read_byte(source); source++;
    dest->height = pgm_read_byte(source); source++;
    dest->transparency_key.red = pgm_read_byte(source); source++;
    dest->transparency_key.green = pgm_read_byte(source); source++;
    dest->transparency_key.blue = pgm_read_byte(source);
    for(uint8_t y = 0; y < dest->height; y++){
        for(uint8_t x = 0; x < dest->width; x++){
            source++;
            dest->raw_data[x][y][0] = pgm_read_byte(source); source++;
            dest->raw_data[x][y][1] = pgm_read_byte(source); source++;
            dest->raw_data[x][y][2] = pgm_read_byte(source);
        }
    }
}

/* ---------------------------- LED Memmory ---------------------------------- */
uint8_t led_memory[MEMORY_SIZE];
size_t mem_index;

void memory_clear(){
    for(size_t i = 0; i < MEMORY_SIZE; i++){
        led_memory[i] = 0x00;
    }
    mem_index = 0;	// reset memory index
}


void* memory_alloc(size_t length){
    void* ptr = NULL;   // assign null pointer
    if(mem_index + length <= MEMORY_SIZE){	// if there is space left in memory
        ptr = &led_memory[mem_index];
        mem_index += length;
    }
    else{
    	mem_error(mem_index + length, MEMORY_SIZE);
    }
    return ptr;
}

led_image img_buffer;
/* ---------------------------- ---------------------------------------------- */
