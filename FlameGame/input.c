#include"input.h"
#include<SDL.h>
#include<SDL_mixer.h>
#include"gameObjects.h"
#include"setup.h"
#include"constants.h"
#include"manager.h"

extern int gameRunning;
extern int gamePaused;
extern int levelID;
extern enum GameState gameState;
extern Mix_Music* backgroundMusic;
extern Mix_Chunk* positiveSound;
extern Mix_Chunk* negativeSound;
extern Mix_Chunk* menuMoveSound;

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
		teleport.position.y = player.position.y;
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
			
			tCurrentSelect = &tResume;
			Mix_PauseMusic();
			Mix_PlayChannel(-1, positiveSound, 0);
			gameState = PAUSESCREEN;

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
			lightning.frame = 0;
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

		//devtools section

		if (event.key.keysym.sym == SDLK_LEFT) {

			readLevelData(--levelID);

		}

		if (event.key.keysym.sym == SDLK_RIGHT) {

			readLevelData(++levelID);

		}

		break;

	case SDL_KEYUP:
		if (event.key.keysym.sym == SDLK_a && player.velocity.horizontal < 0) {
			player.velocity.horizontal = 0;
			playerAnimationTime = PLAYER_WALK_ANIMATION_TIME;
			player.frame = 0;
		}
		if (event.key.keysym.sym == SDLK_d && player.velocity.horizontal > 0) {
			player.velocity.horizontal = 0;
			playerAnimationTime = PLAYER_WALK_ANIMATION_TIME;
			player.frame = 0;
		}
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
			Mix_PlayChannel(-1, negativeSound, 0);
			Mix_ResumeMusic();
			gameState = GAMEPLAY;
		}

		//this is a temporary solution
		//in the future make it so all of the buttons on a screen are apart of a linked listed and tSelect can move to a buttons neightbours
		//every button should also have an action attached to it "exit" "resume" "lower volume" and so on

		if (pauseEvent.key.keysym.sym == SDLK_s || pauseEvent.key.keysym.sym == SDLK_DOWN) {

			if (tCurrentSelect->below != NULL) {
				Mix_PlayChannel(-1, menuMoveSound, 0);
				tCurrentSelect = tCurrentSelect->below;
			}

		}

		if (pauseEvent.key.keysym.sym == SDLK_w || pauseEvent.key.keysym.sym == SDLK_UP) {

			if (tCurrentSelect->above != NULL) {
				Mix_PlayChannel(-1, menuMoveSound, 0);
				tCurrentSelect = tCurrentSelect->above;
			}

		}

		if (pauseEvent.key.keysym.sym == SDLK_RETURN || pauseEvent.key.keysym.sym == SDLK_SPACE) {

			Mix_PlayChannel(-1, positiveSound, 0);
			tCurrentSelect->logic();

		}

		break;
	}
}

void deathInput() {

	SDL_Event deathEvent;
	SDL_PollEvent(&deathEvent);

	switch (deathEvent.type) {

	case SDL_QUIT:
		gameRunning = FALSE;
		break;
	case SDL_KEYDOWN:

		if (deathEvent.key.keysym.sym == SDLK_p)
			gameRunning = FALSE;

		if (deathEvent.key.keysym.sym == SDLK_RETURN || deathEvent.key.keysym.sym == SDLK_SPACE) {

			Mix_PlayChannel(-1, positiveSound, 0);
			tTryAgain.logic();

		}

		break;
	}


}

void titleInput() {

	SDL_Event titleEvent;
	SDL_PollEvent(&titleEvent);

	switch (titleEvent.type) {

	case SDL_QUIT:
		gameRunning = FALSE;
		break;
	case SDL_KEYDOWN:

		if (titleEvent.key.keysym.sym == SDLK_p)
			gameRunning = FALSE;

		if (titleEvent.key.keysym.sym == SDLK_s || titleEvent.key.keysym.sym == SDLK_DOWN) {

			if (tCurrentSelect->below != NULL) {
				Mix_PlayChannel(-1, menuMoveSound, 0);
				tCurrentSelect = tCurrentSelect->below;
			}

		}

		if (titleEvent.key.keysym.sym == SDLK_w || titleEvent.key.keysym.sym == SDLK_UP) {

			if (tCurrentSelect->above != NULL) {
				Mix_PlayChannel(-1, menuMoveSound, 0);
				tCurrentSelect = tCurrentSelect->above;
			};

		}

		if (titleEvent.key.keysym.sym == SDLK_RETURN || titleEvent.key.keysym.sym == SDLK_SPACE) {

			Mix_PlayChannel(-1, positiveSound, 0);
			tCurrentSelect->logic();

		}

		break;
	}

}