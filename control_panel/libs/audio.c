/*
 * audio.c
 *
 *  Created on: May 27, 2012
 *      Author: kevin
 */

#ifndef AUDIO_C_
#define AUDIO_C_

#include "audio.h"

//#define RESET_TO_STROBE 72
//#define STROBE_HIGH 18
//#define OUTPUT_SETTILNG_TIME 36


#define RESET_TO_STROBE 90
#define STROBE_HIGH 30
#define OUTPUT_SETTILNG_TIME 55

#define ON_THRESHOLD 500

// Audio DC: 	PA0
// Strobe:		PA1
// MSG Reset:	PA2


#define adc_complete !(ADCSRA & (1 << ADSC))
#define adc_start_conversion() { ADCSRA |= (1 << ADSC); }

#define strobe_high() PORTA |= (1<<PA1)
#define strobe_low() PORTA &= ~(1<<PA1)
#define reset_high() PORTA |= (1<<PA2)
#define reset_low() PORTA &= ~(1<<PA2)

#define MAX_MIN_DATA_NUMBER 20


enum{
	ST_IDLE,			// 000
	ST_RESET_HIGH,		// 001
	ST_RESET_LOW,		// 010
	ST_STROBE_HIGH,		// 011
	ST_STROBE_LOW,		// 100
	ST_SAMPELING		// 101

} audio_state;

uint8_t bandpass_index;
uint8_t max_min_index;
uint16_t max_data[MAX_MIN_DATA_NUMBER];
uint16_t min_data[MAX_MIN_DATA_NUMBER];
uint16_t raw_data[BANDPASS_MAX];

uint8_t audio_on = 0;

void audio_init(){
	DDRA |= (1<<PA1) | (1<<PA2);
	ADMUX = 0x00;	// using ADC Channel0 on PA0
	ADCSRA = (1<<ADEN) | 0x4; 	// ADC enabled; prescaler 16 = 2^4
	adc_start_conversion();
	while(!(adc_complete));
	uint8_t temp = ADCW;
	strobe_high();
}

void audio_stop(){
	PORTA &= ~((1<<PA1) | (1<<PA2));
	DDRA &= ~((1<<PA1) | (1<<PA2));
}

void audio_start(){
	reset_high();
	// strobe_low();
	TCCR1B = (1<<CS10);				// start timer, no prescaler
	bandpass_index = 0;
	audio_state = ST_RESET_HIGH;	// set state
}

void audio_process(){
	uint16_t data, max, min;
	uint32_t temp;
	// find max and min
	max = 0;
	min = 0x400;
	for(uint8_t i = 0; i < BANDPASS_MAX; i++){
		data = raw_data[i];
		if(data > max){
			max = raw_data[i];
		}
		if(data < min){
			min = data;
		}
	}
	// save max
	max_data[max_min_index] = max;
	min_data[max_min_index] = min;
	max_min_index++;
	if(max_min_index >= MAX_MIN_DATA_NUMBER){
		max_min_index = 0;
	}
	// calculate overall max and min
	max = 0;
	min = 0x400;
	for(uint8_t i = 0; i < MAX_MIN_DATA_NUMBER; i++){
		if(max_data[i] > max){
			max = max_data[i];
		}
		if(min_data[i] < min){
			min = min_data[i];
		}
	}
	// calculate 8bit data
	max -= min;
	for(uint8_t i = 0; i < BANDPASS_MAX; i++){
		temp = raw_data[i] - min;	// subtract minimum
		temp <<= 8;	// multiply by 256
		audio_data[i] = temp / max;
	}
	// calculate on off
	temp = 0;
	for(uint8_t i = 0; i < BANDPASS_MAX; i++){	// add up raw data
		temp += raw_data[i];
	}
	if(temp > ON_THRESHOLD){
		audio_on = 1;
	}
	else{
		audio_on = 0;
	}
}

uint8_t audio_run(){
	switch(audio_state){
	case ST_IDLE:
		break;
	case ST_RESET_HIGH:
		audio_state = ST_RESET_LOW;	// go to the next state; there is no condition, cause reset only needs to be high for 100ns
		strobe_high();
		reset_low();
		TCNT1 = 0;	// reset timer
		break;
	case ST_RESET_LOW:
		if(TCNT1 >= RESET_TO_STROBE * 20){
			strobe_low();
			audio_state = ST_STROBE_LOW;
			TCNT1 = 0;	// reset timer
		}
		break;
	case ST_STROBE_HIGH:
		if(TCNT1 >= STROBE_HIGH * 20){
			strobe_low();
			audio_state = ST_STROBE_LOW;
			TCNT1 = 0;	// reset timer
		}
		break;
	case ST_STROBE_LOW:
		if(TCNT1 >= OUTPUT_SETTILNG_TIME * 20){	// wait for output to settle
			strobe_low();	// keep strobe low
			adc_start_conversion();	// start conversion
			PORTB |= (1<<PB0);
			audio_state = ST_SAMPELING;
		}
		break;
	case ST_SAMPELING:
		if(adc_complete){
			raw_data[bandpass_index] = ADCW;	// save value (10bit accuracy)

			PORTB &= ~(1<<PB0);

			bandpass_index++;	// increase index counter
			strobe_high();
			if(bandpass_index < BANDPASS_MAX){
				audio_state = ST_STROBE_HIGH;
				TCNT1 = 0;	// reset timer
			}
			else{
				audio_state = ST_IDLE;
				TCCR1B = 0x00;		// stop timer
			}
		}
		break;
	default:
		break;
	}
	return audio_state;
}

#endif /* AUDIO_C_ */
