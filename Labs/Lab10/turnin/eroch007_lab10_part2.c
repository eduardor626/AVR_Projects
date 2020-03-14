/*	Author: Eduardo Rocha
 *  Partner(s) Name: Arturo Alvarado
 *	Lab Section:
 *	Assignment: Lab #10  Exercise #2
 *	Exercise Description: 

	Connect LEDs to PB0, PB1, PB2, and PB3. 

	In one state machine (ThreeLEDsSM), output to a shared variable (threeLEDs) the following behavior: 
		set only bit 0 to 1, then only bit 1, then only bit 2 in sequence for 1 second each. 

	In a second state machine (BlinkingLEDSM), output to a shared variable (blinkingLED) the following behavior: 
		set bit 3 to 1 for 1 second, then 0 for 1 second. 
	
	In a third state machine (CombineLEDsSM), combine both shared variables and output to the PORTB. 
		Note: only one SM is writing to outputs. Do this for the rest of the quarter.

	Modify the above example so the threeLEDs light for 300 ms, while blinkingLED’s LED still blinks 1 second on and 1 second off.



 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 */
#include <avr/io.h>
#include "../header/timer.h"
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#endif


unsigned char threeLEDs = 0x00;
unsigned char blinkingLED = 0x00;

enum TL_States { TL_SMStart, TL_T0, TL_T1, TL_T2 } TL_State;
void TickFct_ThreeLeds() {

	switch(TL_State)
	{
		case TL_SMStart:
			TL_State = TL_T0;
			break;
		case TL_T0:
			threeLEDs = 0x01;
			TL_State = TL_T1;
			break;
		case TL_T1:
			threeLEDs = 0x02;
			TL_State = TL_T2;
			break;
		case TL_T2:
			threeLEDs = 0x04;
			TL_State = TL_T0;
			break;
		default:break;
	}
}

enum BL_States { BL_SMStart, BL_LedOff, BL_LedOn } BL_State;
void TickFct_BlinkLed() {
	switch(BL_State)
	{
		case BL_SMStart:
				BL_State = BL_LedOn;
				break;
		case BL_LedOn:
				blinkingLED = 0x08;
				BL_State = BL_LedOff;
				break;
		case BL_LedOff:
				blinkingLED = 0x00;
				BL_State = BL_LedOn;
				break;
		default: break;


	}

}


enum TLBL_States {TLBL_SMStart, OUTPUT} CombineLEDsSM;
void combineStates(){

	switch(CombineLEDsSM)
	{

		case TLBL_SMStart:
			CombineLEDsSM = OUTPUT;
			break;
		case OUTPUT:
			PORTB = 0x0F & (threeLEDs | blinkingLED);
			break;
		default: break;
	}

}

int main(void) {

	//Initializing our output port
	DDRB = 0xFF; PORTB = 0x00;

	//Making sure the times for each state machine follow the project description
	unsigned long BL_elapsedTime = 1000;
   	unsigned long TL_elapsedTime = 300;
   	const unsigned long timerPeriod = 100;


	TimerSet(timerPeriod);
	TimerOn();

	TL_State = TL_SMStart;
	BL_State = BL_SMStart;
	CombineLEDsSM = TLBL_SMStart;

    while (1) {

      	if (TL_elapsedTime >= 300) { // 300 ms period
         	TickFct_ThreeLeds(); // Execute one tick of the ThreeLeds synchSM
         	TL_elapsedTime = 0;
      	}
      	if (BL_elapsedTime >= 1000) { // 1500 ms period
         	TickFct_BlinkLed(); // Execute one tick of the BlinkLed synchSM
         	BL_elapsedTime = 0;           
      	}
     	combineStates();
      	while (!TimerFlag){}   // Wait for timer period
      	TimerFlag = 0;         // Lower flag raised by timer
      	BL_elapsedTime += timerPeriod;
      	TL_elapsedTime += timerPeriod;

    }
    return 1;
}
