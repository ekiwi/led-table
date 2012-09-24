/*
 * audio.h
 *
 *  Created on: May 27, 2012
 *      Author: kevin
 */

#ifndef AUDIO_H_
#define AUDIO_H_

#include <avr/io.h>
#include <util/delay.h>

#define BANDPASS_MAX 7

#define BP63 0
#define BP160 1
#define BP400 2
#define BP1000 3
#define BP2500 4
#define BP6250 5
#define BP16000 6

uint8_t audio_data[BANDPASS_MAX];
extern uint8_t audio_on;

#define audio_get(index) audio_data[index]

void audio_init();
void audio_start();
void audio_stop();
uint8_t audio_run();

#endif /* AUDIO_H_ */
