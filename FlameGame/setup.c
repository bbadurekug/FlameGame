#include"setup.h"
#include<SDL.h>
#include<SDL_image.h>
#include"constants.h"
#include"gameObjects.h"

SDL_Window* window = NULL;
SDL_Renderer* renderer = NULL;

int gameRunning = FALSE;
int levelID = 1;

int initializeWindow() {

	SDL_Init(SDL_INIT_EVERYTHING);

	IMG_Init(IMG_INIT_PNG);

	window = SDL_CreateWindow(
		"FlameGame",
		SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED,
		WINDOW_WIDTH,
		WINDOW_HEIGHT,
		SDL_WINDOW_BORDERLESS
	);

	renderer = SDL_CreateRenderer(
		window,
		-1,
		0
	);

}

void destroyWindow() {

	freeMemory();
	IMG_Quit();
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();

}

SDL_Texture* loadTexture(const char* directory) {

	SDL_Surface* surface = IMG_Load(directory);
	SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
	SDL_FreeSurface(surface);

	return texture;

}

void setup() {

	readLevelData(levelID);

	player.width = 64;
	player.height = 64;
	player.flip = 0;
	player.texture = loadTexture("./Textures/mageTest.png");

	platformTexture = loadTexture("./Textures/brickTest.png");
	boxTexture = loadTexture("./Textures/boxTest.png");

	goal.width = 64;
	goal.height = 64;
	goal.texture = loadTexture("./Textures/goalTest.png");

	teleport.position.x = 0;
	teleport.position.y = 0;
	teleport.width = 64;
	teleport.height = 64;
	teleport.isActive = 0;
	teleport.texture = loadTexture("./Textures/teleportTest.png");
	teleport.frame = 0;

	fireball.position.x = 0;
	fireball.position.y = 0;
	fireball.width = 64;
	fireball.height = 64;
	fireball.isActive = 0;
	fireball.texture = loadTexture("./Textures/fireBallTest.png");

	lightning.position.x = 0;
	lightning.position.y = 0;
	lightning.width = 64;
	lightning.height = 128;
	lightning.isActive = 0;
	lightning.texture = loadTexture("./Textures/lightningTest.png");

	lightningChildLeft.texture = loadTexture("./Textures/lightningChildTest.png");
	lightningChildLeft.velocity.horizontal = -1000.0;
	lightningChildLeft.width = 64;
	lightningChildLeft.height = 64;
	lightningChildLeftActiveTime = 0;

	lightningChildRight.texture = lightningChildLeft.texture;
	lightningChildRight.velocity.horizontal = 1000.0;
	lightningChildRight.width = 64;
	lightningChildRight.height = 64;
	lightningChildRightActiveTime = 0;

	blizzard.position.x = 0;
	blizzard.position.y = 0;
	blizzard.width = 64;
	blizzard.height = 64;
	blizzard.isActive = 0;
	blizzard.texture = loadTexture("./Textures/blizzardTest.png");
	blizzardActiveTime = 0;
}