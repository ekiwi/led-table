/*
 * led_matrix.c
 *
 *  Created on: Apr 7, 2012
 *      Author: kevin
 *
 *      This is the Version adjusted for the Colorduino FW
 */

#include "led_matrix.h"
#include "led_memory.h"
#include "uart.h"
#include <avr/interrupt.h>
#include <util/delay.h>

/*IOs used:
 * PB0, PB1, PB2, PB3, PB4, PB5, PD3, PD4	(lines)
 * PD6 (LED_SCL), PD7 (LED_SDA)
 * PC0 (LED_SELBNK), PC1 (LED_LAT), PC2 (LED_RST)
 *
 * */


// global variables
uint8_t line = 0;
uint8_t update_ws_flag = 0;
uint8_t buffer_moved_flag = 0;


#define RST_BIT (1<<PC2)
#define LAT_BIT (1<<PC1)
#define SLB_BIT (1<<PC0)
#define SCL_BIT (1<<PD6)
#define SDA_BIT (1<<PD7)

#define PORT_RST PORTC
#define PORT_LAT PORTC
#define PORT_SLB PORTC
#define PORT_SDA PORTD
#define PORT_SCL PORTD

#define sda_high() 	PORT_SDA |=	SDA_BIT
#define sda_low() 	PORT_SDA &=	~SDA_BIT
#define scl_high() 	PORT_SCL |=	SCL_BIT
#define scl_low() 	PORT_SCL &=	~SCL_BIT
#define rst_high() 	PORT_RST |=	RST_BIT
#define rst_low() 	PORT_RST &=	~RST_BIT
#define lat_high() 	PORT_LAT |=	LAT_BIT
#define lat_low() 	PORT_LAT &= ~LAT_BIT
#define slb_high() 	PORT_SLB |= SLB_BIT
#define slb_low() 	PORT_SLB &= ~SLB_BIT


#define open_line0()	{PORTB=0x01;}	// PB0
#define open_line1()	{PORTB=0x02;}	// PB1
#define open_line2()	{PORTB=0x04;}	// PB2
#define open_line3()	{PORTB=0x08;}	// PB3
#define open_line4()	{PORTB=0x10;}	// PB4
#define open_line5()	{PORTB=0x20;}	// PB5
#define open_line6()	{PORTD|=0x08;}	// PD3
#define open_line7()	{PORTD|=0x10;}	// PD4
#define close_all_lines()	{PORTD &= ~0x18;PORTB &= ~0x3F;}


// prototypes
static inline void set_white_balance();
static inline void clear_line();


static inline void open_line(uint8_t line){
	switch(line){
	case 0:	open_line0(); break;
	case 1:	open_line1(); break;
	case 2:	open_line2(); break;
	case 3:	open_line3(); break;
	case 4:	open_line4(); break;
	case 5:	open_line5(); break;
	case 6:	open_line6(); break;
	case 7:	open_line7(); break;
	}
}

void matrix_start(void){
	// Initialize IOs
	DDRD |= 0xD8; 	// make PD3, PD4, PD6 and PD7 an output
	DDRC |= 0x07; 	// make PC0, PC1 and PC2 an output
	DDRB |= 0x3F; 	// make PB0, PB1, PB2, PB3, PB4 and PB5 an output
	PORTD &= ~0xD8; // set output low
	PORTC &= ~0x07; // set output low
	PORTB &= ~0x3F; // set output low

	// Initialize the LED driver IC
	rst_high();
	_delay_us(10);
	rst_low();
	_delay_us(10);
	rst_high();
	_delay_us(10);

	line = 0;

	set_white_balance();

	// Initialize and start the Timer 2
	TCCR2B = ((1<<CS22)|(1<<CS20)); // clk/128 => f=125kHz => T=8µs
	TCCR2A = 0x00;   				// Use normal mode
	TIMSK2 = (1<<TOIE2);   			//Timer2 Overflow Interrupt Enable
	TCNT2 = 100;
}

void matrix_stop(void){
	// Stop timer
	TIMSK2 = 0x00;	// disable interrupt
	TCCR2B = 0x00;	// disable timer
	// Stop LED Driver
	clear_line();
	// Stop IOs
	DDRD &= ~(0xD8); 	// make PD3, PD4, PD6 and PD7 an input
	DDRC &= ~(0x03); 	// make PC0, PC1 an input;
	DDRB &= ~(0x3F); 	// make PB0, PB1, PB2, PB3, PB4 and PB5 an input
	PORTD &= ~0xD8; 	// disable pullup
	PORTC &= ~0x07; 	// disable pullup
	PORTB &= ~0x3F; 	// disable pullup
	DDRC |= (1<<PC2);	// reset line kept low
}


/**
 * initializes the matrix display and starts the timer
 */
void matrix_init(void){
	white_balance[0] = DEFAULT_WB_RED;
	white_balance[1] = DEFAULT_WB_GREEN;
	white_balance[2] = DEFAULT_WB_BLUE;

	matrix_start();
}

void matrix_restart(void){
	matrix_start();
}

static inline void set_white_balance(){
	uint8_t temp;
	lat_low();
	slb_low();
	for(uint8_t k=0; k<8; k++){
		for(uint8_t i = 3;i > 0 ;i--){
			temp = white_balance[i-1]<<2;
			for(uint8_t j = 0;j<6;j++){
				if(temp & 0x80){
					sda_high();
				}
				else{
					sda_low();
				}
				temp = temp << 1;
				scl_low();
				scl_high();
			}
		}
	}
	slb_high();
}

static inline void display_line(uint8_t line){
	if(line >= MATRIX_HEIGHT)	return;			// if line is out of range
	uint8_t color_value;
	slb_high();
	lat_low();
	for(uint8_t x = 0; x < MATRIX_WIDTH; x++){
		for(uint8_t color = COLOR_VALUES; color > 0; color--){
			color_value = img_buffer.raw_data[line][x][color-1];
			for(uint8_t bit = 0; bit < 8; bit++){
				if(color_value & 0x80){
					sda_high();
				}
				else{
					sda_low();
				}
				color_value = color_value << 1;
				scl_low();
				scl_high();
			}
		}
	}
	lat_high();
	lat_low();
}

static inline void clear_line(){
	slb_high();
	lat_low();
	for(uint8_t i = 0; i < MATRIX_WIDTH * COLOR_VALUES * 8; i++){
		sda_low();
		scl_low();
		scl_high();
	}
	lat_high();
	lat_low();
}


ISR(TIMER2_OVF_vect)  //Timer2  Service
{
	cli();			// disable interrupts
	d_out_high();	// in interrupt now => can not accept uart data!!
	TCNT2 = 100;	// 8µs*(256-100) = 1.25ms =>  801Hz
	close_all_lines();
	display_line(line);
	open_line(line);
	line++;
	if(line >= MATRIX_HEIGHT){
		line = 0;
	}
	if(update_ws_flag){
		set_white_balance();
		update_ws_flag = 0x00;
	}
	sei();	// enable interrupts
	uart_start_slot(line);	// ready to receive next line
}
