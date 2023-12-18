#include"input.h"
#include<SDL.h>
#include"gameObjects.h"
#include"setup.h"
#include"constants.h"

extern int gameRunning;

void input() {

	Uint8* keystate = SDL_GetKeyboardState(NULL);

	if (keystate[SDL_SCANCODE_A]) {
		player.velocity.horizontal = -250.0;
		player.flip = 1;
	}
	if (keystate[SDL_SCANCODE_D]) {
		player.velocity.horizontal = 250.0;
		player.flip = 0;
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
		if (event.key.keysym.sym == SDLK_ESCAPE)
			gameRunning = FALSE;

		if (event.key.keysym.sym == SDLK_w /* && player.grounded*/) {
			player.grounded = 0;
			player.velocity.vertical = -600.0;
		}

		if (event.key.keysym.sym == SDLK_j && !fireball.isActive) {
			fireball.position.x = player.position.x - fireball.width;
			fireball.position.y = player.position.y;
			//not a big fan of nested loops, can be solved by creating a spawner object
			if (!player.flip) {
				fireball.position.x += fireball.width + player.width;
				fireball.velocity.horizontal = 500;
			}
			else {
				fireball.velocity.horizontal = -500;
			}
			fireball.flip = player.flip;
			fireball.isActive = 1;
		}

		if (event.key.keysym.sym == SDLK_l /* && !lightning.isActive*/) {
			if (!player.flip)
				lightning.position.x = player.position.x + 2.0 * player.width;
			else
				lightning.position.x = player.position.x - 2.0 * player.width;
			lightning.isActive = 1;
			lightningActiveTime = 0.5;
			lightning.position.y = 0;
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