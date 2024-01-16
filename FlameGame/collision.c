#include<SDL.h>
#include<SDL_image.h>
#include"collision.h"
#include"constants.h"

int checkCollisionGround(GameObject entity, GameObject ground) {

	if (entity.position.x + entity.width >= ground.position.x &&
		entity.position.x <= ground.position.x + ground.width &&
		entity.position.y + entity.height >= ground.position.y &&
		entity.position.y + entity.height <= ground.position.y + ground.height - 1.0)
	{
		return 1;
	}

	return 0;
}

int checkCollisionCeiling(GameObject entity, GameObject ground) {

	if (entity.position.x + entity.width > ground.position.x &&
		entity.position.x < ground.position.x + ground.width &&
		entity.position.y < ground.position.y + ground.height &&
		entity.position.y > ground.position.y)
	{
		return 1;
	}
	return 0;
}

int checkCollisionWallLeft(GameObject entity, GameObject ground) {

	if (entity.position.y + entity.height > ground.position.y &&
		entity.position.y < ground.position.y + ground.height &&
		entity.position.x + entity.width > ground.position.x  &&
		entity.position.x < ground.position.x)
	{
		return 1;
	}

	return 0;
}

int checkCollisionWallRight(GameObject entity, GameObject ground) {

	if (entity.position.y + entity.height > ground.position.y &&
		entity.position.y < ground.position.y + ground.height &&
		entity.position.x < ground.position.x + ground.width  &&
		entity.position.x + entity.width > ground.position.x + ground.width)
	{
		return 1;
	}

	return 0;
}

int checkCollision(GameObject entity, GameObject ground) {
	if (checkCollisionGround(entity, ground) ||
		checkCollisionCeiling(entity, ground) ||
		checkCollisionWallLeft(entity, ground) ||
		checkCollisionWallRight(entity, ground))
	{ 
		return 1;
	}

	return 0;
}

int checkOutOfBounds(GameObject entity) {
	
	if(entity.position.x + entity.width < 0 ||
	   entity.position.x > WINDOW_WIDTH ||
	   entity.position.y + entity.height < 0 ||
	   entity.position.y > WINDOW_HEIGHT)
	{
		return 1;
	}

	return 0;
}

int array_checkCollisionGround(GameObject entity, GameObject* ground, int nObjects) {

	for (int i = 0; i < nObjects; i++) {

		if (entity.position.x + entity.width - 5.0 >= ground[i].position.x &&
			entity.position.x + 5.0 <= ground[i].position.x + ground[i].width &&
			entity.position.y + entity.height >= ground[i].position.y &&
			entity.position.y + entity.height <= ground[i].position.y + ground[i].height - 1.0)
		{
			return i;
		}

	}

	return -1;
}

int array_checkCollisionGroundTeleport(GameObject entity, GameObject* ground, int nObjects) {

	for (int i = 0; i < nObjects; i++) {

		if (entity.position.x + entity.width > ground[i].position.x &&
			entity.position.x < ground[i].position.x + ground[i].width &&
			entity.position.y + entity.height > ground[i].position.y &&
			entity.position.y + entity.height < ground[i].position.y + ground[i].height)
		{
			return i;
		}

	}

	return -1;
}

int array_checkCollisionCeiling(GameObject entity, GameObject* ground, int nObjects) {

	for (int i = 0; i < nObjects; i++) {

		if (entity.position.x + entity.width > ground[i].position.x &&
			entity.position.x < ground[i].position.x + ground[i].width &&
			entity.position.y < ground[i].position.y + ground[i].height &&
			entity.position.y > ground[i].position.y)
		{
			return i;
		}

	}

	return -1;
}

int array_checkCollisionWallLeft(GameObject entity, GameObject* ground, int nObjects) {

	for (int i = 0; i < nObjects; i++) {

		if (entity.position.y + entity.height > ground[i].position.y &&
			entity.position.y < ground[i].position.y + ground[i].height &&
			entity.position.x + entity.width > ground[i].position.x &&
			entity.position.x < ground[i].position.x)
		{
			return i;
		}

	}

	return -1;
}

int array_checkCollisionWallRight(GameObject entity, GameObject* ground, int nObjects) {

	for (int i = 0; i < nObjects; i++) {

		if (entity.position.y + entity.height > ground[i].position.y &&
			entity.position.y < ground[i].position.y + ground[i].height &&
			entity.position.x < ground[i].position.x + ground[i].width &&
			entity.position.x + entity.width > ground[i].position.x + ground[i].width)
		{
			return i;
		}

	}

	return -1;
}

int array_checkCollision(GameObject entity, GameObject* ground, int nObjects) {
	if (array_checkCollisionGround(entity, ground, nObjects) != -1 ||
		array_checkCollisionWallLeft(entity, ground, nObjects) != -1 ||
		array_checkCollisionWallRight(entity, ground, nObjects) != -1)
	{
		return 1;
	}

	return 0;
}

int array_checkCollisionTeleport(GameObject entity, GameObject* ground, int nObjects) {
	if (array_checkCollisionGroundTeleport(entity, ground, nObjects) != -1 ||
		array_checkCollisionWallLeft(entity, ground, nObjects) != -1 ||
		array_checkCollisionWallRight(entity, ground, nObjects) != -1)
	{
		return 1;
	}

	return 0;
}

int checkIsInsideObject(GameObject entity, GameObject* ground, int nObjects) {

	for (int i = 0; i < nObjects; i++) {
		if (entity.position.y >= ground[i].position.y &&
			entity.position.y + entity.height <= ground[i].position.y + ground[i].height &&
			entity.position.x >= ground[i].position.x &&
			entity.position.x + entity.width <= ground[i].position.x + ground[i].width)
		{
			return 1;
		}
	}

	return 0;

}

int checkIsInsideObjectSingular(GameObject entity, GameObject ground) {

	if (entity.position.y >= ground.position.y &&
		entity.position.y + entity.height <= ground.position.y + ground.height &&
		entity.position.x >= ground.position.x &&
		entity.position.x + entity.width <= ground.position.x + ground.width)
	{
		return 1;
	}

	return 0;

}