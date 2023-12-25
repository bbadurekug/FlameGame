#include"gameObjects.h"
#include<stdio.h>
#include<stdlib.h>

void readLevelData(int levelID) {

	char ID[3];

	snprintf(ID, sizeof(ID), "%d", levelID);

	char levelDirectory[21];

	snprintf(levelDirectory, sizeof(levelDirectory), "./Levels/level%s.txt", ID);

	printf("%s\n", levelDirectory);

	FILE* levelFile;

	fopen_s(&levelFile, levelDirectory, "r");

	if (levelFile == NULL)
		return;

	char levelData[1024];

	extern int nPlatforms;

	fgets(levelData, sizeof(levelData) - 1, levelFile);

	sscanf_s(levelData, "%f %f", &player.position.x, &player.position.y);

	fgets(levelData, sizeof(levelData) - 1, levelFile);

	sscanf_s(levelData, "%f %f", &goal.position.x, &goal.position.y);

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

	boxes = malloc(sizeof(GameObject) * nBoxes);

	for (int i = 0; i < nBoxes; i++) {

		fgets(levelData, sizeof(levelData) - 1, levelFile);

		//printf("%s\n", levelData);

		sscanf_s(levelData, "%f %f %f %f %d", &boxes[i].position.x, &boxes[i].position.y, &boxes[i].width, &boxes[i].height, &boxes[i].frame);

		boxes[i].velocity.horizontal = 0;
		boxes[i].velocity.vertical = 0;

		if (boxes[i].frame == 1)
			boxes[i].grounded = 1;

	}

	fclose(levelFile);

}

void freeMemory() {

	free(platforms);
	free(boxes);
}