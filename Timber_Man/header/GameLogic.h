  
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


}


//here is where we check if we should end the game
void checkCollisions(){

		for(unsigned char j =0; j<7; j++){
			if(branches[j].row == 14 && (branches[j].side == timbermanSide)){
				GameOver = 1;
			}
    }


}

enum GameLogicStates {GameLogic_Start, GameLogic_Init, GameLogic_WaitForPress, GameLogic_Wait, GameLogic_ShiftCheck} GameLogicState;


int GameLogicSM(int GameLogicState){

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
			GameLogicState = GameLogic_Wait;
			break;
		case GameLogic_Wait:
			GameLogicState= GameLogic_ShiftCheck;
			break;
		case GameLogic_ShiftCheck:
			shiftDown();
			checkCollisions();
			GameLogicState = GameLogic_Wait;
			break;
		default: break;

	}
	return GameLogicState;

}




//GAME LOGIC END
#endif