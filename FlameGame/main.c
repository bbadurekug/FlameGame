#include"setup.h"
#include"update.h"
#include"input.h"
#include"render.h"

extern int gameRunning;
extern int gamePaused;

int main(int argc, char* args[]) {

	gameRunning = initializeWindow();

	setup();

	while (gameRunning) {

		deltaTimeCalculation();

		if (!gamePaused) {
			update();
			input();
		}
		else {
			pauseInput();
		}

		render();

	}

	destroyWindow();

	return 0;

}