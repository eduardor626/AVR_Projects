#ifndef _SPEAKER_H_
#define _SPEAKER_H_

#include "pwm.h"

enum SpeakerStates {Speaker_Start, Speaker_Init, Speaker_Wait, Speaker_Sound, Speaker_WaitForNextGame} SpeakerState;
unsigned char counter = 0;
unsigned char countdownCounter = 0;
unsigned char fiveFlag = 0, fourFlag = 0 , threeFlag = 0 , twoFlag = 0 , oneFlag = 0;


void speakerSequence(double value){
	if(countdownCounter  == 0){
		set_PWM(value);
		countdownCounter++;
	}
}

int SpeakerStatesSM(int SpeakerState){

	unsigned char temp = countdownFrom;

	switch(SpeakerState)
	{
		case Speaker_Start:
			SpeakerState = Speaker_Init;
			break;
		case Speaker_Init:
			SpeakerState = Speaker_Wait;
			counter = 0;
			countdownCounter = 0;
			set_PWM(0);
			break;
		case Speaker_Wait:
			if(countdownComplete != 1 && Start == 1){
				if(countdownFrom == 5){
					set_PWM(392.00);
				}else if(countdownFrom == 4){
					set_PWM(440.00);
				}else if(countdownFrom == 3){
					set_PWM(293.66);
				}else if(countdownFrom == 2){
					set_PWM(392.00);
				}else if(countdownFrom == 1){
					set_PWM(440.00);
				}else{
					set_PWM(0);
				}

			}
			else if(GameOver == 1 || StopClockZero == 1){
				set_PWM(349.23);
				SpeakerState = Speaker_Sound;
				countdownCounter = 0;

			}else{
				set_PWM(0);
				SpeakerState = Speaker_Wait;
			}
			break;
		case Speaker_Sound:
			if(counter > 1){
				set_PWM(0);
				counter = 0;
				SpeakerState = Speaker_WaitForNextGame;
			}else{
				SpeakerState = Speaker_Sound;
				counter++;

			}
			break;
		case Speaker_WaitForNextGame:
			set_PWM(0);
			if(Reset == 1 || Start == 1){
				SpeakerState = Speaker_Wait;
				countdownCounter = 0;
			}else{
				SpeakerState = Speaker_WaitForNextGame;
			}
			break;
		default:break;
	}
	return SpeakerState;
}

//SPEAKER END
#endif