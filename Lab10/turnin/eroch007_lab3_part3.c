/*	Author: Eduardo Rocha
 *  Partner(s) Name: Arturo Alvarado
 *	Lab Section:
 *	Assignment: Lab #10  Exercise #3
 *	Exercise Description: 

	
	

	Modify the above example so the threeLEDs light for 300 ms, while blinkingLED’s LED still blinks 1 second on and 1 second off.
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 */



#include <avr/io.h>
#include <avr/interrupt.h>
#include "../header/timer.h"



unsigned char ThreeLEDs_value=0x00;

enum ThreeLEDs_STATES { ThreeLEDs_start, b0, b1, b2 } ThreeLEDs_state;
	
void Tick_ThreeLEDs() {
	switch(ThreeLEDs_state){
		case ThreeLEDs_start:	ThreeLEDs_state=b0; break;
		case b0:	ThreeLEDs_state=b1; ThreeLEDs_value=0x01; break;
		case b1:	ThreeLEDs_state=b2; ThreeLEDs_value=0x02; break;
		case b2:	ThreeLEDs_state=b0; ThreeLEDs_value=0x04; break;
		default:	ThreeLEDs_state=ThreeLEDs_start; break;
	}
}





unsigned char BlinkingLEDs_value=0x00;

enum BlinkingLEDs_STATES { BlinkingLEDs_start, b3_on, b3_off } BlinkingLEDs_state;

void Tick_BlinkingLEDs() {
	switch(BlinkingLEDs_state){
		case BlinkingLEDs_start:	BlinkingLEDs_state=b3_on; break;
		case b3_on:	BlinkingLEDs_state=b3_off; BlinkingLEDs_value=0x08;	break;
		case b3_off: BlinkingLEDs_state=b3_on; BlinkingLEDs_value=0x00; break;
		default:	ThreeLEDs_state=BlinkingLEDs_start; break;
	}
	
}


unsigned char speaker=0;

enum SPEAK_STATES{ speaker_start, off, on } SPEAK_state;
void Tick_Speaker(){
	
	unsigned char button=~PINA&0x01;
	
	switch( SPEAK_state){
		case speaker_start: SPEAK_state=off;     break;
		case off: SPEAK_state=(button) ? on : off; break;
		case on: SPEAK_state=off;  break;
		default: break;
	}
	
	switch( SPEAK_state){
		case speaker_start:  break;
		case off:  speaker=0x00;  break;
		case on:  speaker=0x10;  break;
		default: break;
	}

}



enum CombineLEDs_STATES { CombineLEDs_start, light_up_LEDs } CombineLEDs_state;

void Tick_CombineLEDs() {
	
	unsigned char output_B=0;
	
	switch(CombineLEDs_state){
		case CombineLEDs_start:	CombineLEDs_state=light_up_LEDs; break;
		case light_up_LEDs:	//BlinkingLEDs_state=light_up_LEDs;   why adding this will cause problem?
							output_B = ThreeLEDs_value | BlinkingLEDs_value | speaker;	break;
		default:	CombineLEDs_state=CombineLEDs_start; break;
	}
	
	PORTB=output_B;

}





// -----------------------------------------------------------------------------------------------------------------------------------




typedef struct task {
	int state;                  // Task's current state
	unsigned long period;       // Task period
	unsigned long elapsedTime;  // Time elapsed since last task tick
	int (*TickFct)(int);        // Task tick function
} task;

task tasks[4];
const unsigned short tasksNum = 4;


int main(void)
{
    /* Replace with your application code */
   /* Replace with your application code */
   DDRA=0x00; PORTA=0xFF;
   DDRB=0XFF; PORTB=0x00;

   
   TimerSet(2);
   TimerOn();
   
   

   
    unsigned char i = 0;
    tasks[i].state = ThreeLEDs_start;
    tasks[i].period = 300;
    tasks[i].elapsedTime = tasks[i].period;
    tasks[i].TickFct = &Tick_ThreeLEDs;
    i++;
	
    tasks[i].state = BlinkingLEDs_start;
    tasks[i].period = 1000;
    tasks[i].elapsedTime = tasks[i].period;
    tasks[i].TickFct = &Tick_BlinkingLEDs;
	i++;
	
	tasks[i].state = speaker_start;
    tasks[i].period = 2;
    tasks[i].elapsedTime = tasks[i].period;
    tasks[i].TickFct = &Tick_Speaker;
	i++;
    
	tasks[i].state = CombineLEDs_start;
	tasks[i].period = 2;
	tasks[i].elapsedTime = tasks[i].period;
	tasks[i].TickFct = &Tick_CombineLEDs;
	
	
   while (1)
   {
		
		for (i = 0; i < tasksNum; ++i) { // Heart of the scheduler code
			if ( tasks[i].elapsedTime >= tasks[i].period ) { // Ready
				tasks[i].state = tasks[i].TickFct(tasks[i].state);
				tasks[i].elapsedTime = 0;
			}
			tasks[i].elapsedTime += 2;
		}
		  
	   while(!TimerFlag);
	   TimerFlag=0;
   }
}