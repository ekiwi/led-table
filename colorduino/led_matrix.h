/*
 * led_matrix.h
 *
 *  Created on: Apr 7, 2012
 *      Author: kevin
 */

#ifndef LED_MATRIX_H_
#define LED_MATRIX_H_

#include <avr/io.h>


#define DEFAULT_WB_RED 		20		// max is 0x3F
#define DEFAULT_WB_GREEN 	63
#define DEFAULT_WB_BLUE 	63

#define MATRIX_WIDTH 8
#define MATRIX_HEIGHT 8
#define COLOR_VALUES 3

#define BUFFER_SIZE 5
#define DISPLAY_START_BUFFER 0
#define RECEIVE_START_BUFFER 1


// "public" functions
void matrix_init(void);		// initializes the matrix display
void matrix_restart(void);	//
void matrix_stop(void);		//


#endif /* LED_MATRIX_H_ */
