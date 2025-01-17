#include"setup.h"
#include"update.h"
#include"input.h"
#include"render.h"
#include"manager.h"
#include<stdio.h>

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
			titleUpdate();
			titleInput();
			titleRender();
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
			deathUpdate();
			deathInput();
			render();
			break;

		case LEVELEDITOR:
			editorUpdate();
			editorInput();
			editorRender();
			break;
		}

	}

	destroyWindow();

	return 0;

}