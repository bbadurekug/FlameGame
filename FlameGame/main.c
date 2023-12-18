#include"setup.h"
#include"update.h"
#include"input.h"
#include"render.h"

extern int gameRunning;

int main(int argc, char* args[]) {

	gameRunning = initializeWindow();

	setup();

	while (gameRunning) {
		
		update();
		input();
		render();

	}

	destroyWindow();

	return 0;

}