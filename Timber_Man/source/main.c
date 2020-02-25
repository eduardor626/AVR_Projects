/*	Author: eduardorocha
*  Partner(s) Name: 
*	Lab Section:
*	Assignment: Lab #  Exercise #
*	Exercise Description: [optional - include for your own benefit]
*
*	I acknowledge all content contained herein, excluding template or example
*	code, is my own original work.
*/
#include <avr/io.h>
#include <stdio.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#include <util/delay.h>
//#include "../header/max7219.h"
#include "../header/DisplayMatrix.h"
#include "../header/scheduler.h"
#include "../header/timer.h"

#endif

task DisplayTask;


int main(void) {

    //Initialize PORTD for LCD Display
    DDRD = 0xFF;
    PORTD = 0x00;

    unsigned int a;
    max7219_init(); //init LCD Display


    //Setting period
    TimerSet(1000);
    TimerOn();

    //Defining our tasks 
    task *tasks[] = {&DisplayTask};
    const uint8_t tasksSize = sizeof(tasks)/sizeof(tasks[0]);

    //The Display Initializing
    DisplayTask.state = Display_Start;
    DisplayTask.period = 1000;
    DisplayTask.elapsedTime = 1000;
    DisplayTask.TickFct = &DisplaySM;

    while(1)
    {
        for(uint8_t i = 0; i< tasksSize; ++i)
        {
            //if task is ready to exexute
            if(tasks[i]->elapsedTime >= tasks[i]->period)
            {
                //execute tick function and update state
                tasks[i]->state = tasks[i]->TickFct(tasks[i]->state);
                //reset elasped time
                tasks[i]->elapsedTime = 0;
            }
            //update elasped time
                tasks[i]->elapsedTime += 1000;
        }
        while(!TimerFlag);
        TimerFlag = 0;


    }
}
