/*	
    Author: Eduardo Rocha
*	Assignment: Final Project Timber Man
*	Exercise Description: 
        This is the main.c (driver) of my Timber Man Variation game.
*
*/
#include <avr/io.h>
#include <stdio.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#include "../header/io.h"
#include <avr/eeprom.h> 
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

//Score Variables
unsigned char newHighScoreFlag = 0x00;
unsigned char Select = 0;
unsigned char deleteHighScoreFlag = 0x00;

//StopClockVariables
unsigned char StopClockZero = 0x00;


//headers for game
#include "nokia5110.h"
#include "../header/DisplayMatrix.h"
#include "../header/scheduler.h"
#include "../header/timer.h"
#include "../header/GameLogic.h"
#include "../header/DisplayScore.h"
#include "../header/DisplayStopClock.h"
#include "../header/Game.h"
#endif



task DisplayTask, GameLogicTask, DisplayScoreTask, DisplayStopClockTask, GameTask;

int main() {

    //Initializing PORTS for Input/Output

    //PORTA for Button Inputs
    DDRA = 0x00;
    PORTA = 0xFF;

    //PORTD for LED Matrix Display
    DDRD = 0xFF;
    PORTD = 0x00;

    //PORTC for LCD HighScore Display
    DDRC = 0xFF; 
    PORTC = 0x00; 

    //PORTB for NOKIA Screen
    DDRB = 0xFF;
    PORTB = 0x00;

    max7219_init(); //init LED Display
    LCD_init(); 	//init LCD 
    nokia_lcd_init(); //init Nokia Screen

    //Setting period
    TimerSet(100);
    TimerOn();


    //Defining our tasks 
    task *tasks[] = {&DisplayTask,&GameLogicTask,&DisplayScoreTask, &DisplayStopClockTask, &GameTask};
    const uint8_t tasksSize = sizeof(tasks)/sizeof(tasks[0]);

    //The LED Display Initializing
    DisplayTask.state = Display_Start;
    DisplayTask.period = 100;
    DisplayTask.elapsedTime = 100;
    DisplayTask.TickFct = &DisplaySM;

    //The Game Logic Initializing
    GameLogicTask.state = GameLogic_Start;
    GameLogicTask.period = 100;
    GameLogicTask.elapsedTime = 100;
    GameLogicTask.TickFct = &GameLogicSM;

    //The LCD Display Score Initializing
    DisplayScoreTask.state = DisplayScore_Start;
    DisplayScoreTask.period = 100;
    DisplayScoreTask.elapsedTime = 100;
    DisplayScoreTask.TickFct = &DisplayScoreSM;


    //The NOKIA LCD Display StopClock Initializing
    DisplayStopClockTask.state = STOPCLOCK_START;
    DisplayStopClockTask.period = 100;
    DisplayStopClockTask.elapsedTime = 100;
    DisplayStopClockTask.TickFct = &DisplayStopClockSM;

    //The Game Logic Initializing
    GameTask.state = Game_Start;
    GameTask.period = 100;
    GameTask.elapsedTime = 100;
    GameTask.TickFct = &GameStateSM;

    unsigned char a;
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
                tasks[i]->elapsedTime += 100;
        }
        while(!TimerFlag);
        TimerFlag = 0;


    }
}
