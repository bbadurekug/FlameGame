#pragma once
#ifndef GAMEOBJECTS_H
#define	GAMEOBJECTS_H

#include<SDL.h>
#include<SDL_image.h>

typedef struct position {

	float x;
	float y;

}Position;

typedef struct velocity {

	float vertical;
	float horizontal;

}Velocity;

typedef struct gameObject {

	Position position;
	float width;
	float height;
	Velocity velocity;
	int isActive;
	int flip;
	int grounded;
	SDL_Texture* texture;
	int frame;

} GameObject;

typedef struct textBox {
	
	Position position;
	float width;
	float height;
	SDL_Texture* textTexture;
	SDL_Texture* backgroundTexture;
	struct TextBox* above;
	struct TexTBox* below;
	void (*logic) ();

} TextBox;

void readLevelData(int levelID);

void freeMemory();

void tExitLogic();
void tResumeLogic();

void tTryAgainLogic();

void tPlayLogic();
void tQuitLogic();

GameObject player;
float playerAnimationTime;

GameObject teleport;

GameObject fireball;
float fireballActiveTime;

GameObject lightning;
float lightningActiveTime;

GameObject lightningChildLeft;
float lightningChildLeftActiveTime;

GameObject lightningChildRight;
float lightningChildRightActiveTime;

GameObject blizzard;
float blizzardActiveTime;

GameObject *platforms;
SDL_Texture* platformTexture;
int nPlatforms;

GameObject *boxes;
SDL_Texture* boxTexture;
int nBoxes;

GameObject goal;
GameObject doorKey;

TextBox tPaused;
TextBox tResume;
TextBox tExit;

TextBox tDead;
TextBox tTryAgain;

TextBox tTitle;
TextBox tPlay;
TextBox tQuit;

GameObject tSelect;
TextBox *tCurrentSelect;

#endif 