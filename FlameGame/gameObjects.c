#include"gameObjects.h"
#include"constants.h"
#include"setup.h"
#include"manager.h"
#include<SDL_mixer.h>
#include<stdio.h>
#include<stdlib.h>

extern int levelID;
extern Mix_Music* backgroundMusic;

void readLevelData(int levelID) {

	char ID[3];

	snprintf(ID, sizeof(ID), "%d", levelID);

	char levelDirectory[21];

	snprintf(levelDirectory, sizeof(levelDirectory), "./Levels/level%s.txt", ID);

	printf("%s\n", levelDirectory);

	FILE* levelFile;

	fopen_s(&levelFile, levelDirectory, "r");

	if (levelFile == NULL) {
		printf("wtf bro ten poziom nie istnieje\n");
		return;
	}

	char levelData[1024];

	extern int nPlatforms;

	fgets(levelData, sizeof(levelData) - 1, levelFile);

	sscanf_s(levelData, "%f %f", &player.position.x, &player.position.y);

	fgets(levelData, sizeof(levelData) - 1, levelFile);

	sscanf_s(levelData, "%f %f %d", &goal.position.x, &goal.position.y, &goal.frame);

	fgets(levelData, sizeof(levelData) - 1, levelFile);

	sscanf_s(levelData, "%f %f", &doorKey.position.x, &doorKey.position.y);

	fgets(levelData, sizeof(levelData) - 1, levelFile);

	sscanf_s(levelData, "%d", &nPlatforms);

	platforms = malloc(sizeof(GameObject) * nPlatforms);

	for(int i = 0; i < nPlatforms; i++){

		fgets(levelData, sizeof(levelData) - 1, levelFile);

		sscanf_s(levelData, "%f %f %f %f", &platforms[i].position.x, &platforms[i].position.y, &platforms[i].width, &platforms[i].height);

	}

	fgets(levelData, sizeof(levelData) - 1, levelFile);

	//printf("%s\n", levelData);

	sscanf_s(levelData, "%d", &nBoxes);

	targetPosBox = malloc(sizeof(float) * nBoxes);
	boxes = malloc(sizeof(GameObject) * nBoxes);

	for (int i = 0; i < nBoxes; i++) {

		fgets(levelData, sizeof(levelData) - 1, levelFile);

		//printf("%s\n", levelData);

		sscanf_s(levelData, "%f %f %f %f %d", &boxes[i].position.x, &boxes[i].position.y, &boxes[i].width, &boxes[i].height, &boxes[i].frame);

		boxes[i].velocity.horizontal = 0;
		boxes[i].velocity.vertical = 0;
		boxes[i].isActive = 0;

		if (boxes[i].frame == 1)
			boxes[i].grounded = 1;
		else
			boxes[i].grounded = 0;

		targetPosBox[i] = boxes[i].position.x;

	}

	fclose(levelFile);

}

void freeMemory() {

	if (platforms != NULL) free(platforms);
	free(targetPosBox);
	if (boxes != NULL) free(boxes);
}

void writeLevelData(levelID) {

	char ID[3];

	snprintf(ID, sizeof(ID), "%d", levelID);

	char levelDirectory[21];

	snprintf(levelDirectory, sizeof(levelDirectory), "./Levels/level%s.txt", ID);

	printf("writing to %s\n", levelDirectory);

	FILE* levelFile;

	fopen_s(&levelFile, levelDirectory, "w");

	if (levelFile == NULL) {
		printf("wtf bro ten poziom nie istnieje\n");
		return;
	}

	extern int nPlatforms;

	fprintf_s(levelFile, "%d %d\n", (int)player.position.x, (int)player.position.y);

	fprintf_s(levelFile, "%d %d %d\n", (int)goal.position.x, (int)goal.position.y, goal.frame);

	fprintf_s(levelFile, "%d %d\n", (int)doorKey.position.x, (int)doorKey.position.y);

	fprintf_s(levelFile, "%d\n", nPlatforms);

	for (int i = 0; i < nPlatforms; i++) {

		fprintf_s(levelFile, "%d %d %d %d\n", (int)platforms[i].position.x, (int)platforms[i].position.y, (int)platforms[i].width, (int)platforms[i].height);

	}

	fprintf_s(levelFile, "%d\n", nBoxes);

	for (int i = 0; i < nBoxes; i++) {

		fprintf_s(levelFile, "%d %d %d %d %d\n", (int)boxes[i].position.x, (int)boxes[i].position.y, (int)boxes[i].width, (int)boxes[i].height, boxes[i].frame);

	}

	fclose(levelFile);

}

extern int gameRunning;

void tExitLogic() {

	gameState = TITLESCREEN;
	tCurrentSelect = &tPlay;

}

void tResumeLogic() {

	player.velocity.horizontal = 0;
	Mix_ResumeMusic();
	gameState = GAMEPLAY;

}

void tTryAgainLogic() {

	player.velocity.horizontal = 0;
	readLevelData(levelID);
	gameState = GAMEPLAY;

}

void tPlayLogic() {

	readLevelData(levelID);
	Mix_PlayMusic(backgroundMusic, -1);
	gameState = GAMEPLAY;

}

void tEditorLogic() {

	readLevelData(levelID);
	gameState = LEVELEDITOR;

}

void tQuitLogic() {

	gameRunning = FALSE;

}