#include"setup.h"
#include<SDL.h>
#include<SDL_image.h>
#include<SDL_ttf.h>
#include"constants.h"
#include"gameObjects.h"

SDL_Window* window = NULL;
SDL_Renderer* renderer = NULL;
TTF_Font* font = NULL;

int gameRunning = FALSE;
int gamePaused = FALSE;
int levelID = 1;

int initializeWindow() {

	SDL_Init(SDL_INIT_EVERYTHING);

	IMG_Init(IMG_INIT_PNG);

	TTF_Init();

	window = SDL_CreateWindow(
		"FlameGame",
		SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED,
		WINDOW_WIDTH,
		WINDOW_HEIGHT,
		SDL_WINDOW_FULLSCREEN
	);

	renderer = SDL_CreateRenderer(
		window,
		-1,
		0
	);

}

SDL_Texture* loadTexture(const char* directory) {

	SDL_Surface* surface = IMG_Load(directory);
	SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
	SDL_FreeSurface(surface);

	return texture;

}

SDL_Texture* createTextTexture(TTF_Font* font, const char* text, SDL_Color color) {

	SDL_Surface* textSurface = TTF_RenderText_Solid(font, text, color);
	SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
	SDL_FreeSurface(textSurface);

	return textTexture;

}

void setup() {

	font = TTF_OpenFont("./Fonts/PressStart2P-Regular.ttf", 24);

	tPaused.textTexture = createTextTexture(font, "Paused", (SDL_Color){ 255, 255, 255, 255 });
	tPaused.width = 300;
	tPaused.height = 100;
	tPaused.position.x = (WINDOW_WIDTH / 2) - (tPaused.width) / 2;
	tPaused.position.y = (WINDOW_HEIGHT / 4) - (tPaused.height) / 2;

	tResume.textTexture = createTextTexture(font, "Resume", (SDL_Color) { 255, 255, 255, 255 });
	tResume.width = 250;
	tResume.height = 75;
	tResume.position.x = (WINDOW_WIDTH / 2) - (tResume.width) / 2;
	tResume.position.y = (WINDOW_HEIGHT / 2) - (tResume.height) / 2;
	//tResume.buttonBelow = &tExit;

	tExit.textTexture = createTextTexture(font, "Exit", (SDL_Color) { 255, 255, 255, 255 });
	tExit.width = 200;
	tExit.height = 75;
	tExit.position.x = (WINDOW_WIDTH / 2) - (tExit.width) / 2;
	tExit.position.y = (WINDOW_HEIGHT - (WINDOW_HEIGHT / 3)) - (tExit.height) / 2;
	//tExit.buttonAbove = &tResume;

	tSelect.position.x = tResume.position.x;
	tSelect.position.y = tResume.position.y;
	tSelect.width = tResume.width;
	tSelect.height = tResume.height;
	
	readLevelData(levelID);

	player.width = 64;
	player.height = 64;
	player.flip = 0;
	player.texture = loadTexture("./Textures/mageTest.png");
	player.frame = 0;

	platformTexture = loadTexture("./Textures/brickTest.png");
	boxTexture = loadTexture("./Textures/boxTest.png");

	//to zmenic
	tPaused.backgroundTexture = platformTexture;
	tResume.backgroundTexture = platformTexture;
	tExit.backgroundTexture = platformTexture;

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
	fireball.frame = 0;

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

void destroyWindow() {

	freeMemory();
	TTF_CloseFont(font);
	TTF_Quit();
	SDL_DestroyTexture(player.texture);
	SDL_DestroyTexture(platformTexture);
	SDL_DestroyTexture(boxTexture);
	SDL_DestroyTexture(goal.texture);
	SDL_DestroyTexture(teleport.texture);
	SDL_DestroyTexture(fireball.texture);
	SDL_DestroyTexture(lightning.texture);
	SDL_DestroyTexture(lightningChildLeft.texture);
	SDL_DestroyTexture(blizzard.texture);
	IMG_Quit();
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();

}