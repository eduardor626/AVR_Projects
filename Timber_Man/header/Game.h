  
#ifndef _GAME_H_
#define _GAME_H_
//GAME BEGIN




enum GameStates {Game_Start, Game_Wait, Game_Play, Game_StopGame} GameState;

int GameStateSM(int GameState){

	unsigned char startPressed = (~PINA & 0x07);



	switch(GameState){
		case Game_Start:
			GameState = Game_Wait;
			break;
		case Game_Wait:
			if(startPressed != 0x04 && Reset != 1){
				GameState = Game_Wait;
				Start = 0;
			}else if(startPressed == 0x04 || Reset == 1){
				GameState = Game_Play;
				Start = 1;
				Reset = 0;
			}else{
				GameState = Game_Wait;
				Start = 0;
			}
			break;

		case Game_Play:
			if(GameOver == 0 && StopClockZero == 0){
				GameState = Game_Play;
			}else if(GameOver == 1 || StopClockZero == 1){
				Start = 0;
				GameState = Game_StopGame;
			}else{
				GameState = Game_Play;
			}
			break;

		case Game_StopGame:
			if(startPressed != 0x04){
				GameState = Game_StopGame;
			}else if(startPressed == 0x04){
				Reset = 1;
				GameState = Game_Wait;
			}else{
				GameState = Game_StopGame;
			}
			break;
		default:break;
	}

	return GameState;


}


//GAME END
#endif