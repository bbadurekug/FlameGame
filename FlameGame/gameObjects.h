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

void writeLevelData(int levelID);

void tExitLogic();
void tResumeLogic();

void tTryAgainLogic();

void tPlayLogic();
void tEditorLogic();
void tQuitLogic();

GameObject player;
float playerAnimationTime;

GameObject editorCursor;
SDL_Texture* cursorTextureArray[6];
int cursorTextureIndex;

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

SDL_Texture* backgroundTexture;

GameObject *platforms;
SDL_Texture* platformTexture;
int nPlatforms;

GameObject *boxes;
SDL_Texture* boxTexture;
int nBoxes;
float* targetPosBox;

GameObject goal;
GameObject doorKey;

TextBox tPaused;
TextBox tResume;
TextBox tExit;

TextBox tDead;
TextBox tTryAgain;

TextBox tVersion;
TextBox tTitle;
TextBox tPlay;
TextBox tEditor;
TextBox tQuit;

GameObject tSelect;
TextBox *tCurrentSelect;

#endif 