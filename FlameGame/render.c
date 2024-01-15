#include"render.h"
#include<SDL.h>
#include<SDL_image.h>
#include"constants.h"
#include"gameObjects.h"
#include"setup.h"
#include"manager.h"

extern SDL_Renderer* renderer;
extern enum GameState gameState;

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

void renderTextBox(SDL_Renderer* renderer, TextBox textBox) {

	SDL_Rect textBoxLeftBoxRect = {

			(int)textBox.position.x - 10,
			(int)textBox.position.y - 10,
			5,
			(int)textBox.height + 20

	};

	SDL_Rect textBoxLeftFrameRect = {

		0,
		0,
		1,
		64
	};

	SDL_RenderCopy(renderer, textBox.backgroundTexture, &textBoxLeftFrameRect, &textBoxLeftBoxRect);

	SDL_Rect textBoxMiddleBoxRect = {

		(int)textBox.position.x - 5,
		(int)textBox.position.y - 10,
		(int)textBox.width + 5,
		(int)textBox.height + 20

	};

	SDL_Rect textBoxMiddleFrameRect = {

		1,
		0,
		1,
		64
	};

	SDL_RenderCopy(renderer, textBox.backgroundTexture, &textBoxMiddleFrameRect, &textBoxMiddleBoxRect);

	SDL_Rect textBoxShadeBoxRect = {

		(int)(textBox.position.x + textBox.width - 5),
		(int)textBox.position.y - 10,
		5,
		(int)textBox.height + 20

	};

	SDL_Rect textBoxShadeFrameRect = {

		2,
		0,
		1,
		64
	};

	SDL_RenderCopy(renderer, textBox.backgroundTexture, &textBoxShadeFrameRect, &textBoxShadeBoxRect);

	SDL_Rect textBoxRightBoxRect = {

		(int)(textBox.position.x + textBox.width),
		(int)textBox.position.y - 10,
		5,
		(int)textBox.height + 20

	};

	SDL_Rect textBoxRightFrameRect = {

		0,
		0,
		1,
		64
	};

	SDL_RenderCopy(renderer, textBox.backgroundTexture, &textBoxRightFrameRect, &textBoxRightBoxRect);

	SDL_Rect textBoxTextRect = {

		(int)textBox.position.x,
		(int)textBox.position.y,
		(int)textBox.width,
		(int)textBox.height

	};

	SDL_RenderCopy(renderer, textBox.textTexture, NULL, &textBoxTextRect);

}

void render() {

	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
	SDL_RenderClear(renderer);

	//SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);

	renderMultipleObjectsTexture(renderer, platforms, nPlatforms, platformTexture);

	renderObjectsTextureFrame(renderer, boxes, nBoxes, boxTexture);

	SDL_Rect goalRect = {
		(int)goal.position.x,
		(int)goal.position.y,
		(int)goal.width,
		(int)goal.height
	};

	SDL_Rect goalFrameRect = {
		(goal.width / 2.0) * goal.frame,
		0,
		goal.width / 2.0,
		goal.height / 2.0
	};

	SDL_RenderCopy(renderer, goal.texture, &goalFrameRect, &goalRect);

	if (goal.frame == 0) {
		SDL_Rect doorKeyRect = {
			(int)doorKey.position.x,
			(int)doorKey.position.y,
			(int)doorKey.width,
			(int)doorKey.height
		};

		SDL_RenderCopy(renderer, doorKey.texture, NULL, &doorKeyRect);
	}

	SDL_Rect fireballRect = {
		(int)fireball.position.x,
		(int)fireball.position.y,
		(int)fireball.width,
		(int)fireball.height
	};

	SDL_Rect fireballFrameRect = {
		(fireball.width / 2.0) * fireball.frame,
		0,
		fireball.width / 2.0,
		fireball.height / 2.0
	};

	if (fireball.isActive && fireball.flip)
		SDL_RenderCopyEx(renderer, fireball.texture, &fireballFrameRect, &fireballRect, 0.0, NULL, SDL_FLIP_HORIZONTAL);
	else if (fireball.isActive && !fireball.flip)
		SDL_RenderCopyEx(renderer, fireball.texture, &fireballFrameRect, &fireballRect, 0.0, NULL, SDL_FLIP_NONE);

	SDL_Rect playerRect = {
		(int)player.position.x,
		(int)player.position.y,
		(int)player.width,
		(int)player.height
	};

	SDL_Rect playerFrameRect = {
		(player.width / 2.0) * player.frame,
		0,
		player.width / 2.0,
		player.height / 2.0
	};

	if (!player.flip)
		SDL_RenderCopyEx(renderer, player.texture, &playerFrameRect, &playerRect, 0.0, NULL, SDL_FLIP_NONE);
	else
		SDL_RenderCopyEx(renderer, player.texture, &playerFrameRect, &playerRect, 0.0, NULL, SDL_FLIP_HORIZONTAL);

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

	SDL_Rect lightningRect = {
		(int)lightning.position.x,
		(int)lightning.position.y,
		(int)lightning.width,
		(int)lightning.height
	};

	SDL_Rect lightningFrameRect = {
		(lightning.width / 2.0) * lightning.frame,
		0,
		lightning.width / 2.0,
		lightning.height / 2.0
	};

	if (lightning.isActive)
		SDL_RenderCopy(renderer, lightning.texture, &lightningFrameRect, &lightningRect);

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

	if (gameState == PAUSESCREEN) {

		SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 128);

		SDL_Rect tBlackScreen = {

			0,
			0,
			WINDOW_WIDTH,
			WINDOW_HEIGHT

		};

		SDL_RenderFillRect(renderer, &tBlackScreen);

		renderTextBox(renderer, tPaused);
		renderTextBox(renderer, tResume);
		renderTextBox(renderer, tExit);

		SDL_Rect tSelectRect = {

			(int)tSelect.position.x,
			(int)tSelect.position.y,
			(int)tSelect.width,
			(int)tSelect.height

		};

		SDL_RenderCopy(renderer, tSelect.texture, NULL, &tSelectRect);

	}
	else if (gameState == DEATHSTATE) {

		SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
		SDL_SetRenderDrawColor(renderer, 255, 0, 0, 128);

		SDL_Rect tRedScreen = {

			0,
			0,
			WINDOW_WIDTH,
			WINDOW_HEIGHT

		};

		SDL_RenderFillRect(renderer, &tRedScreen);

		renderTextBox(renderer, tDead);
		renderTextBox(renderer, tTryAgain);

		SDL_Rect tSelectRect = {

			(int)tSelect.position.x,
			(int)tSelect.position.y,
			(int)tSelect.width,
			(int)tSelect.height

		};

		SDL_RenderCopy(renderer, tSelect.texture, NULL, &tSelectRect);

	}

	SDL_RenderPresent(renderer);
}

void titleRender() {

	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
	SDL_RenderClear(renderer);

	//temporary solution, will make a background later

	for (int j = 0; j < (WINDOW_WIDTH / BRICK_WIDTH); j++) {

		for (int k = 0; k < (WINDOW_HEIGHT / BRICK_HEIGHT); k++) {

			SDL_Rect objectsRect = {

				0 + j * BRICK_WIDTH,
				0 + k * BRICK_HEIGHT,
				BRICK_WIDTH,
				BRICK_HEIGHT

			};

			SDL_RenderCopy(renderer, platformTexture, NULL, &objectsRect);
		}
	}

	SDL_Rect tSelectRect = {

			(int)tSelect.position.x,
			(int)tSelect.position.y,
			(int)tSelect.width,
			(int)tSelect.height

	};

	SDL_RenderCopy(renderer, tSelect.texture, NULL, &tSelectRect);

	renderTextBox(renderer, tTitle);
	renderTextBox(renderer, tPlay);
	renderTextBox(renderer, tQuit);

	SDL_RenderPresent(renderer);

}