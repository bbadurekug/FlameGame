#include"setup.h"
#include"update.h"
#include"input.h"
#include"render.h"
#include"manager.h"

extern int gameRunning;
extern int gamePaused;
extern enum GameState gameState;

int main(int argc, char* args[]) {

	gameRunning = initializeWindow();

	setup();

	while (gameRunning) {

		deltaTimeCalculation();
	
		switch (gameState) {

		case TITLESCREEN:

			break;

		case GAMEPLAY:
			update();
			input();
			render();
			break;

		case PAUSESCREEN:
			pauseUpdate();
			pauseInput();
			render();
			break;

		case LOADSCREEN:

			break;

		case DEATHSTATE:

			break;
		}

		

	}

	destroyWindow();

	return 0;

}