  
#ifndef _GAMELOGIC_H_
#define _GAMELOGIC_H_
//GAME LOGIC BEGIN


//shifting down the values of branches so they look like they are falling
void shiftDown(){
	for(unsigned char j =0; j<7; j++){
		branches[j].row = branches[j].row +1;
		if(branches[j].row == 15){
			branches[j].row = 0;
		}
    }
}



//moving timberMan character
void moveTimberMan(){
	if(rightMove == 0x01 && leftMove == 0x00){
		timberMan = 0x02;
		timbermanSide = 'R';
	}else if(leftMove == 0x02 && rightMove == 0x00){
		timberMan = 0x40;
		timbermanSide = 'L';
	}
}


//here is where we check if we should end the game
void checkCollisions(){

		for(unsigned char j =0; j<7; j++){
			if(branches[j].row == 14 && (branches[j].side == timbermanSide)){
				GameOver = 1;
				return;
			}
			if(StopClockZero == 1){
				GameOver =1;
			}
    }


}

enum GameLogicStates {GameLogic_Start, GameLogic_Init, GameLogic_WaitForPress, GameLogic_Wait, GameLogic_ShiftCheck, GameLogic_Stop} GameLogicState;


int GameLogicSM(int GameLogicState){

	unsigned int readInput = (~PINA & 0x03);

	//Transitions
	switch(GameLogicState)
	{
		case GameLogic_Start:
			GameLogicState = GameLogic_Init;
			break;
		case GameLogic_Init:
			GameLogicState = GameLogic_WaitForPress;
			GameOver = 0;
			collision = 0;
			timbermanSide = 'L';
			break;
		case GameLogic_WaitForPress:
			move = 0;
			if(readInput == 0x01 || readInput == 0x02 && GameOver != 1){
				rightMove = (readInput & 0x01);
				leftMove = (readInput & 0x02);
				GameLogicState = GameLogic_Wait;
			}
			else{
				GameLogicState = GameLogic_WaitForPress;

			}
			break;
		case GameLogic_Wait:
			if(readInput == 0x00){
				GameLogicState= GameLogic_ShiftCheck;
			}else{
				GameLogicState = GameLogic_Wait;
			}
			break;
		case GameLogic_ShiftCheck:
			moveTimberMan();
			checkCollisions();
			shiftDown();
			checkCollisions();
			if(GameOver == 1 || StopClockZero == 1){
				GameLogicState = GameLogic_Stop;
			}else{
				GameLogicState = GameLogic_WaitForPress;

			}
			break;
		case GameLogic_Stop:
			GameLogicState = GameLogic_Stop;
		default: break;

	}
	return GameLogicState;

}




//GAME LOGIC END
#endif