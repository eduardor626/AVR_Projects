#ifndef _DISPLAY_MATRIX_H_
#define _DISPLAY_MATRIX_H_

#include "max7219.h"
#include <util/delay.h>


//Countdown Variables that display the countdown
unsigned char five[] = { 0x00, 0x7C, 0x60, 0x7E, 0x06, 0x06, 0x66, 0x3C };
unsigned char four[] = { 0x00, 0x0C, 0x1C, 0x2C, 0x4C, 0x7E, 0x0C, 0x0C };
unsigned char three[] = { 0x00, 0x3C, 0x66, 0x06, 0x1C, 0x0C, 0x66, 0x3C };
unsigned char two[] = { 0x00, 0x3C, 0x66, 0x06, 0x0C, 0x30, 0x60, 0x7E };
unsigned char one[] = { 0x00, 0x18, 0x18, 0x38, 0x18, 0x18, 0x18, 0x7E };
unsigned char frown[] = { 0x3C, 0x42, 0xA5, 0x81, 0x99, 0xA5, 0x42, 0x3C};
unsigned char smile[]= { 0x3C, 0x42, 0xA5, 0x81, 0xA5, 0x99, 0x42, 0x3C};


unsigned char i = 0;
unsigned char ticks = 0;


branch branches[7];

//Creating Branches at the start of the game
void initBranches(){
    branch1.row= 0;
    branch1.side = 'L';
    branch1.value = leftBranch;
    branch1.whichIC = 0;

    branch2.row= 2;
    branch2.side = 'R';
    branch2.value = rightBranch;
    branch2.whichIC = 0;

    branch3.row= 4;
    branch3.side = 'R';
    branch3.value = rightBranch;
    branch3.whichIC = 0;

    branch4.row= 6;
    branch4.side = 'L';
    branch4.value = leftBranch;
    branch4.whichIC = 0;

    branch5.row= 8;
    branch5.side = 'L';
    branch5.value = leftBranch;
    branch5.whichIC = 1;

    branch6.row= 10;
    branch6.side = 'R';
    branch6.value = rightBranch;
    branch6.whichIC = 1;

    branch7.row= 12;
    branch7.side = 'L';
    branch7.value = leftBranch;
    branch7.whichIC = 1;

    timberMan = 0x40;

    branches[0] = branch1;
    branches[1] = branch2;
    branches[2] = branch3;
    branches[3] = branch4;
    branches[4] = branch5;
    branches[5] = branch6;
    branches[6] = branch7;

}

//Function that will display a countdown on the LED Matrix 5,4,3,2,1 etc.
void countdown(){

    switch(countdownFrom)
    {
        case 6:
            countdownComplete = 0;
            break;
        case 5:
            max7219_clearDisplay(0);
            for (unsigned char j = 0; j < 8; j++) {
                max7219_digit(0, j, five[j]); //first LCD at row , equal value at five[row]
            }
            break;
        case 4:
            max7219_clearDisplay(0);
            for (unsigned char j = 0; j < 8; j++) {
                max7219_digit(0, j, four[j]); 
            }
            break;
        case 3:
            max7219_clearDisplay(0);
            for (unsigned char j = 0; j < 8; j++) {
                max7219_digit(0, j, three[j]); 
            }
            break;
        case 2:
            max7219_clearDisplay(0);
            for (unsigned char j = 0; j < 8; j++) {
                max7219_digit(0, j, two[j]); 
            }
            break;
        case 1:
            for (unsigned char j = 0; j < 8; j++) {
                max7219_digit(0, j, one[j]); 
            }
            break;
        case 0:
            countdownComplete = 1;
        default:
            break;
    }
}


//Initializing the initial start display of the game
void initializeMatrix(){
    max7219_clearDisplay(0);
    max7219_clearDisplay(1);

    initBranches();

    //creating the trunk
    for (unsigned char j = 0; j < 8; j++) {
        max7219_digit(0, j,treeTrunk); 
        max7219_digit(1,j,treeTrunk);
    }

    //creating the branches
    for(unsigned char j =0; j<7; j++){
        if(branches[j].row > 7){
            unsigned int r = branches[j].row - 8;
            max7219_digit(1,r,branches[j].value);
        }else{
            max7219_digit(0,branches[j].row,branches[j].value);
        }
    }

    //placing timber man
    max7219_digit(1,6,(treeTrunk|timberMan));
}   


void displayMatrix(){
    //creating the trunk
    for (unsigned char j = 0; j < 8; j++) {
        max7219_digit(0, j,treeTrunk); 
        max7219_digit(1,j,treeTrunk);
    }

    //creating the branches
    for(unsigned char j =0; j<7; j++){
        if(branches[j].row > 7){
            unsigned int r = branches[j].row - 8;
            max7219_digit(1,r,branches[j].value);
        }else{
            max7219_digit(0,branches[j].row,branches[j].value);
        }
    }
    //placing timber man
    unsigned int timberManRow = max7219_getdigit6(1);
    max7219_digit(1,6,(timberMan | timberManRow));
}

void displayFrown(){
    for (unsigned char j = 0; j < 8; j++) {
        max7219_digit(0, j, frown[j]); 
    }
}

void displayHappy(){
    for (unsigned char j = 0; j < 8; j++) {
        max7219_digit(0, j, smile[j]); 
    }
}


//DISPLAY BEGIN
enum DisplayStates {Display_Start, Display_Init, Display_Countdown, Display_CountdownWait, Display_Print,Display_WaitForNextGame} DisplayState;

int DisplaySM(int DisplayState)
{

    //transitions
    switch(DisplayState)
    {
        case Display_Start:
            DisplayState = Display_Init;
            break;
        case Display_Init:
            max7219_clearDisplay(0);
            max7219_clearDisplay(1);
            DisplayState = Display_Init;
            break;
        case Display_Countdown:
            DisplayState = Display_Countdown;
            break;
        case Display_Print:
            DisplayState = Display_Print;
            break;
        case Display_WaitForNextGame:
            DisplayState = Display_WaitForNextGame;
            break;
        default:
            break;
    }

    //actions
    switch(DisplayState)
    {
        case Display_Start:
            break;
        case Display_Init:
            if(Start == 1 || Reset == 1){
                countdownComplete = 0;
                i = 0;
                DisplayState = Display_Countdown;
            }else{
                DisplayState = Display_Init;
            }
            break;
        case Display_Countdown:
            if(countdownComplete != 1){
                countdown();
                DisplayState = Display_CountdownWait;
            }
            if(countdownComplete == 1){
                initializeMatrix();
                DisplayState = Display_Print;
            }
            break;
        case Display_CountdownWait:
            if(i > 10){
                countdownFrom--;
                max7219_clearDisplay(0);
                i = 0;
                DisplayState = Display_Countdown;
            }else{
                i++;
                DisplayState = Display_CountdownWait;
            }
            break;
        case Display_Print:
            if(GameOver == 1 || StopClockZero == 1){
                max7219_clearDisplay(1);
                max7219_clearDisplay(0);
                DisplayState = Display_WaitForNextGame;
            }else{
                DisplayState = Display_Print;
                displayMatrix();

            }
            break;
        case Display_WaitForNextGame:
            if(newHighScoreFlag != 0x00){
                displayHappy();
                if(Start == 1 || Reset == 1){
                    countdownFrom = 6;
                    ticks = 0;
                    DisplayState = Display_Start;
                }else{
                    DisplayState = Display_WaitForNextGame;
                }

            }else{
                displayFrown();
                if(Start == 1 || Reset == 1){
                    countdownFrom = 6;
                    ticks = 0;
                    DisplayState = Display_Start;
                }else{
                    DisplayState = Display_WaitForNextGame;

                }
            }
            break;
        default:
            break;
    }
    return DisplayState;
}


//DISPLAY END
#endif
