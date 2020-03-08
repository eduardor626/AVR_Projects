#ifndef _SPEAKER_H_
#define _SPEAKER_H_

#include "pwm.h"

enum SpeakerStates {Speaker_Start, Speaker_Init, Speaker_Wait, Speaker_Sound, Speaker_WaitForNextGame} SpeakerState;
unsigned char counter = 0;
unsigned char countdownCounter = 0;


void speakerSequence(double value){
	if(countdownCounter  == 0){
		set_PWM(value);
		countdownCounter++;
	}
}


int SpeakerStatesSM(int SpeakerState){

	switch(SpeakerState)
	{
		case Speaker_Start:
			SpeakerState = Speaker_Init;
			break;
		case Speaker_Init:
			SpeakerState = Speaker_Wait;
			break;
		case Speaker_Wait:
			if(countdownComplete != 1){
				unsigned char temp = countdownFrom;
				switch(temp)
				{
					case 5:
						if(countdownCounter > 8){
            				temp--;
            				set_PWM(261.63);
            				countdownCounter = 0;
            			}else{
                			countdownCounter ++;
                			set_PWM(0);
            			}
						break;
					case 4:
						if(countdownCounter > 8){
            				temp--;
            				set_PWM(293.66);
            				countdownCounter = 0;
            			}else{
                			countdownCounter ++;
                			set_PWM(0);
            			}
						break;
					case 3: 
						if(countdownCounter > 8){
            				temp--;
            				set_PWM(329.63);
            				countdownCounter = 0;
            			}else{
                			countdownCounter ++;
                			set_PWM(0);
            			}
						break;
					case 2:
						if(countdownCounter > 8){
            				temp--;
            				set_PWM(349.23);
            				countdownCounter = 0;
            			}else{
                			countdownCounter ++;
                			set_PWM(0);
            			}
						break;
					case 1:
						if(countdownCounter > 8){
            				temp--;
            				set_PWM(392.00);
            				countdownCounter = 0;
            			}else{
                			countdownCounter ++;
                			set_PWM(0);
            			}
						break;
					default: break;
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
			if(counter > 2){
				set_PWM(0);
				counter = 0;
				SpeakerState = Speaker_WaitForNextGame;
			}else{
				SpeakerState = Speaker_Sound;
				counter++;

			}

			break;
		case Speaker_WaitForNextGame:
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