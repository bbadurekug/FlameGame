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

void readLevelData(int levelID);

void freeMemory();

GameObject player;

GameObject teleport;

GameObject fireball;

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

#endif 