/*	
    Author: Eduardo Rocha
*	Assignment: Final Project Timber Man
*	Exercise Description: 
    
    This is the main.c of my Timber Man Variation game.
*
*	I acknowledge all content contained herein, excluding template or example
*	code, is my own original work.
*/
#include <avr/io.h>
#include <stdio.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#include <util/delay.h>

//======== Shared Variables ========
typedef struct _branch{
    unsigned char row;        
    unsigned char side;       
    unsigned char value; 
    unsigned char whichIC; 
} branch;


//Tree Trunk Variable
const unsigned char treeTrunk = 0x18;
//Timber Man character position
unsigned char timberMan = 0x40;
//Branches 
const unsigned char leftBranch = (0x60 | treeTrunk); 
const unsigned char rightBranch = (0x06 | treeTrunk); 
const unsigned char emptyBranch = (0x00| treeTrunk);
//Array of Branches to choose from
unsigned char whichBranch[4] = {emptyBranch,leftBranch,rightBranch};

branch branches[7];

branch branch1;
branch branch2;
branch branch3;
branch branch4;
branch branch5;
branch branch6;
branch branch7;
branch branch8;


//GameLogic Variables
unsigned char GameOver;
unsigned char collision;
unsigned char move;
unsigned char timbermanSide;

//Game Variables
unsigned char Reset;
unsigned char Start;
unsigned char leftMove;
unsigned char rightMove;


//headers for game
#include "../header/DisplayMatrix.h"
#include "../header/scheduler.h"
#include "../header/timer.h"
#include "../header/GameLogic.h"
#endif

task DisplayTask, GameLogicTask;

int main() {


    //Initializing PORTS for Input/Output
    DDRA = 0x00;
    PORTA = 0xFF;

    //PORTD for LCD Display
    DDRD = 0xFF;
    PORTD = 0x00;




    max7219_init(); //init LCD Display

    //Setting period
    TimerSet(200);
    TimerOn();

    //Defining our tasks 
    task *tasks[] = {&DisplayTask,&GameLogicTask};
    const uint8_t tasksSize = sizeof(tasks)/sizeof(tasks[0]);

    //The Display Initializing
    DisplayTask.state = Display_Start;
    DisplayTask.period = 200;
    DisplayTask.elapsedTime = 200;
    DisplayTask.TickFct = &DisplaySM;

    //The Game Logic Initializing
    GameLogicTask.state = GameLogic_Start;
    GameLogicTask.period = 200;
    GameLogicTask.elapsedTime = 200;
    GameLogicTask.TickFct = &GameLogicSM;



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
                tasks[i]->elapsedTime += 200;
        }
        while(!TimerFlag);
        TimerFlag = 0;


    }
}
