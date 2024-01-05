#include"input.h"
#include<SDL.h>
#include"gameObjects.h"
#include"setup.h"
#include"constants.h"

extern int gameRunning;
extern int gamePaused;
extern int levelID;

void input() {

	Uint8* keystate = SDL_GetKeyboardState(NULL);

	if (keystate[SDL_SCANCODE_A] && player.flip == 1) {
		player.velocity.horizontal = -250.0;
		teleport.position.x = player.position.x;
	}
	if (keystate[SDL_SCANCODE_D] && player.flip == 0) {
		player.velocity.horizontal = 250.0;
		teleport.position.x = player.position.x;
	}
	if (keystate[SDL_SCANCODE_I]) {
		teleport.flip = player.flip;
		if (!teleport.flip)
			teleport.position.x = player.position.x + 2.0 * player.width;
		else
			teleport.position.x = player.position.x - 2.0 * player.width;
		teleport.position.y = player.position.y - 0.1;
		teleport.isActive = 1;
	}
	else {
		teleport.isActive = 0;
	}

	SDL_Event event;
	SDL_PollEvent(&event);

	switch (event.type) {

	case SDL_QUIT:
		gameRunning = FALSE;
		break;
	case SDL_KEYDOWN:
		if (event.key.keysym.sym == SDLK_p)
			gameRunning = FALSE;

		if (event.key.keysym.sym == SDLK_ESCAPE) {

			tSelect.position.x = tResume.position.x;
			tSelect.position.y = tResume.position.y;
			tSelect.width = tResume.width;
			tSelect.height = tResume.height;
			gamePaused = TRUE;

		}

		if (event.key.keysym.sym == SDLK_r)
			readLevelData(levelID);

		if (event.key.keysym.sym == SDLK_w && player.grounded) {
			player.grounded = 0;
			player.velocity.vertical = -600.0;
		}

		if (event.key.keysym.sym == SDLK_a && player.flip == 0) {

			player.flip = 1;
			player.frame = 0;
			playerAnimationTime = PLAYER_WALK_ANIMATION_TIME;

		}

		if (event.key.keysym.sym == SDLK_d && player.flip == 1) {

			player.flip = 0;
			player.frame = 0;
			playerAnimationTime = PLAYER_WALK_ANIMATION_TIME;

		}

		if (event.key.keysym.sym == SDLK_j && !fireball.isActive) {
			fireball.grounded = 0;
			fireball.frame = 0;
			fireball.position.x = player.position.x - fireball.width - 2;
			fireball.position.y = player.position.y;
			//not a big fan of nested loops, can be solved by creating a spawner object
			if (!player.flip) {
				fireball.position.x += fireball.width + player.width + 4;
				fireball.velocity.horizontal = 500;
			}
			else {
				fireball.velocity.horizontal = -500;
			}
			fireball.flip = player.flip;
			fireball.isActive = 1;
		}

		if (event.key.keysym.sym == SDLK_l && !lightning.isActive) {
			if (!player.flip)
				lightning.position.x = player.position.x + 2.0 * player.width;
			else
				lightning.position.x = player.position.x - 2.0 * player.width;
			lightning.isActive = 1;
			lightningActiveTime = 0.5;
			lightning.position.y = -128;
			lightning.velocity.vertical = 10000;
		}

		if (event.key.keysym.sym == SDLK_k) {
			blizzardActiveTime = 1;
			blizzard.isActive = 1;

			for (int i = 0; i < nBoxes; i++)
				boxes[i].frame = 1;
		}

		break;
	case SDL_KEYUP:
		if (event.key.keysym.sym == SDLK_a && player.velocity.horizontal < 0)
			player.velocity.horizontal = 0;
		if (event.key.keysym.sym == SDLK_d && player.velocity.horizontal > 0)
			player.velocity.horizontal = 0;
		if (event.key.keysym.sym == SDLK_i && teleport.isActive && teleport.grounded)
		{
			player.position.x = teleport.position.x;
			player.position.y = teleport.position.y;
		}
	}

}

void pauseInput() {

	SDL_Event pauseEvent;
	SDL_PollEvent(&pauseEvent);

	switch (pauseEvent.type) {

	case SDL_QUIT:
		gameRunning = FALSE;
		break;
	case SDL_KEYDOWN:

		if (pauseEvent.key.keysym.sym == SDLK_p)
			gameRunning = FALSE;

		if (pauseEvent.key.keysym.sym == SDLK_ESCAPE) {
			player.velocity.horizontal = 0;
			gamePaused = FALSE;
		}

		//this is a temporary solution
		//in the future make it so all of the buttons on a screen are apart of a linked listed and tSelect can move to a buttons neightbours
		//every button should also have an action attached to it "exit" "resume" "lower volume" and so on

		if (pauseEvent.key.keysym.sym == SDLK_s || pauseEvent.key.keysym.sym == SDLK_DOWN && tSelect.position.y == tResume.position.y) {

			tSelect.position.x = tExit.position.x;
			tSelect.position.y = tExit.position.y;
			tSelect.width = tExit.width;
			tSelect.height = tExit.height;

		}

		if (pauseEvent.key.keysym.sym == SDLK_w || pauseEvent.key.keysym.sym == SDLK_UP && tSelect.position.y == tExit.position.y) {

			tSelect.position.x = tResume.position.x;
			tSelect.position.y = tResume.position.y;
			tSelect.width = tResume.width;
			tSelect.height = tResume.height;

		}

		if (pauseEvent.key.keysym.sym == SDLK_RETURN) {

			if (tSelect.position.y == tResume.position.y) {

				player.velocity.horizontal = 0;
				gamePaused = FALSE;

			}
			else if (tSelect.position.y == tExit.position.y) {

				gameRunning = FALSE;

			}

		}

		break;
	}
}