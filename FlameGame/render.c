#include"render.h"
#include<SDL.h>
#include<SDL_image.h>
#include"constants.h"
#include"gameObjects.h"
#include"setup.h"

extern SDL_Renderer* renderer;

void renderObjects(SDL_Renderer* renderer, GameObject* objects, int nObjects) {

	for (int i = 0; i < nObjects; i++) {

		SDL_Rect objectsRect = {

			(int)objects[i].position.x,
			(int)objects[i].position.y,
			(int)objects[i].width,
			(int)objects[i].height

		};

		SDL_RenderFillRect(renderer, &objectsRect);
	}

}

void renderObjectsTextureFrame(SDL_Renderer* renderer, GameObject* objects, int nObjects, SDL_Texture* texture) {

	for (int i = 0; i < nObjects; i++) {

		SDL_Rect objectsRect = {

			(int)objects[i].position.x,
			(int)objects[i].position.y,
			(int)objects[i].width,
			(int)objects[i].height

		};

		SDL_Rect objectsFrameRect = {
		(objects[i].width / 2.0) * objects[i].frame,
		0,
		objects[i].width / 2.0,
		objects[i].height / 2.0
		};

		SDL_RenderCopy(renderer, texture, &objectsFrameRect, &objectsRect);
	}

}

void renderMultipleObjectsTexture(SDL_Renderer* renderer, GameObject* objects, int nObjects, SDL_Texture* texture) {

	for (int i = 0; i < nObjects; i++) {

		for (int j = 0; j < (objects[i].width / BRICK_WIDTH); j++) {

			for (int k = 0; k < (objects[i].height / BRICK_HEIGHT); k++) {

				SDL_Rect objectsRect = {

					objects[i].position.x + j * BRICK_WIDTH,
					objects[i].position.y + k * BRICK_HEIGHT,
					BRICK_WIDTH,
					BRICK_HEIGHT

				};

				SDL_RenderCopy(renderer, texture, NULL, &objectsRect);
			}
		}
	}

}

void render() {

	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
	SDL_RenderClear(renderer);

	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);

	renderMultipleObjectsTexture(renderer, platforms, nPlatforms, platformTexture);

	renderObjectsTextureFrame(renderer, boxes, nBoxes, boxTexture);

	SDL_Rect goalRect = {
		(int)goal.position.x,
		(int)goal.position.y,
		(int)goal.width,
		(int)goal.height
	};

	SDL_RenderCopy(renderer, goal.texture, NULL, &goalRect);

	SDL_Rect playerRect = {
		(int)player.position.x,
		(int)player.position.y,
		(int)player.width,
		(int)player.height
	};

	if (!player.flip)
		SDL_RenderCopyEx(renderer, player.texture, NULL, &playerRect, 0.0, NULL, SDL_FLIP_NONE);
	else
		SDL_RenderCopyEx(renderer, player.texture, NULL, &playerRect, 0.0, NULL, SDL_FLIP_HORIZONTAL);

	SDL_Rect teleportRect = {
		(int)teleport.position.x,
		(int)teleport.position.y,
		(int)teleport.width,
		(int)teleport.height
	};

	SDL_Rect teleportFrameRect = {
		(teleport.width / 2.0) * teleport.frame,
		0,
		teleport.width / 2.0,
		teleport.height / 2.0
	};

	if (teleport.isActive && !teleport.flip)
		SDL_RenderCopyEx(renderer, teleport.texture, &teleportFrameRect, &teleportRect, 0.0, NULL, SDL_FLIP_NONE);
	else if (teleport.isActive && teleport.flip)
		SDL_RenderCopyEx(renderer, teleport.texture, &teleportFrameRect, &teleportRect, 0.0, NULL, SDL_FLIP_HORIZONTAL);

	SDL_Rect fireballRect = {
		(int)fireball.position.x,
		(int)fireball.position.y,
		(int)fireball.width,
		(int)fireball.height
	};

	if (fireball.isActive && fireball.flip)
		SDL_RenderCopyEx(renderer, fireball.texture, NULL, &fireballRect, 0.0, NULL, SDL_FLIP_HORIZONTAL);
	else if (fireball.isActive && !fireball.flip)
		SDL_RenderCopyEx(renderer, fireball.texture, NULL, &fireballRect, 0.0, NULL, SDL_FLIP_NONE);

	SDL_Rect lightningRect = {
		(int)lightning.position.x,
		(int)lightning.position.y,
		(int)lightning.width,
		(int)lightning.height
	};

	if (lightning.isActive)
		SDL_RenderCopy(renderer, lightning.texture, NULL, &lightningRect);

	SDL_Rect lightningChildLeftRect = {

		(int)lightningChildLeft.position.x,
		(int)lightningChildLeft.position.y,
		(int)lightningChildLeft.width,
		(int)lightningChildLeft.height
	};

	if (lightningChildLeft.isActive)
		SDL_RenderCopyEx(renderer, lightningChildLeft.texture, NULL, &lightningChildLeftRect, 0.0, NULL, SDL_FLIP_NONE);

	SDL_Rect lightningChildRightRect = {

		(int)lightningChildRight.position.x,
		(int)lightningChildRight.position.y,
		(int)lightningChildRight.width,
		(int)lightningChildRight.height
	};

	if (lightningChildRight.isActive)
		SDL_RenderCopyEx(renderer, lightningChildRight.texture, NULL, &lightningChildRightRect, 0.0, NULL, SDL_FLIP_HORIZONTAL);

	SDL_Rect blizzardRect = {

		(int)blizzard.position.x,
		(int)blizzard.position.y,
		(int)blizzard.width,
		(int)blizzard.height
	};

	if (blizzard.isActive)
		SDL_RenderCopy(renderer, blizzard.texture, NULL, &blizzardRect);

	SDL_RenderPresent(renderer);
}