/*
 * hal.h
 *
 *  Created on: May 28, 2012
 *      Author: kevin
 */

#ifndef HAL_H_
#define HAL_H_

#ifndef F_CPU
	#define USING_QT
#endif

#ifdef USING_QT
	// Qt Headers for Debugging and for Data Types
    #include <QtGlobal>
    #include <stdbool.h>

	// Integer Types
	typedef qint8 int8_t;
	typedef quint8 uint8_t;
	typedef qint16 int16_t;
	typedef quint16 uint16_t;
	typedef qint32 int32_t;
	typedef quint32 uint32_t;

    #define PROGMEM
    #define pgm_read_byte(x) (*(x))

    #define DISPLAY_SHOW_ERROR 0xD0

	// Define Debug Functions
    #define deb(x) printf("Deb: %d \n", x) //qDebug() << "Deb: " << x << endl
    #define deb_2(x,y) qDebug() << x << y << endl
    #define deb_color(c) qDebug() << "Color: {" << c.red << ", " << c.green << ", " << c.blue << "}" << endl

	// Define Memory Error Functions
    #define mem_error(x,s)
    //#define mem_error(x,s) printf("Memory Error: Not enough space! Space Requested: %d. Maximum Memory Size: %d.", (int)x, (int)s)

	// Include Hardware Simulation Funcitons
    typedef void (*send_ptr)(void);
    send_ptr uart_img_send;
    typedef void (*dsp_ptr)(uint8_t, uint8_t, uint8_t);
    dsp_ptr set_display;
    typedef uint8_t (*audio_ptr)(uint8_t);
    audio_ptr audio_get;
    typedef uint8_t (*rand_ptr)(void);
    rand_ptr led_rand;


#else
	// Include AVR IO and StdBool Files
	#include <avr/io.h>
	#include <avr/pgmspace.h>
	#include <stdbool.h>
	// Define deb functions
	#define deb(x)
	#define deb_2(x,y)
	#define deb_color(c)
    // Define Memory Error Functions
	#define mem_error(x,s)
    // Define Null pointer
	#define NULL 0
	// Include Hardware Functions
	#include "../libs/audio.h"
	#include "../libs/interface.h"
	#include "../libs/uart.h"

	#define led_rand() rand()
#endif

#endif /* HAL_H_ */
