#include"update.h"
#include<SDL.h>
#include"gameObjects.h"
#include"constants.h"
#include"collision.h"

int lastFrameTime = 0;
extern int levelID;

void update() {

	//deltaTime cannot be deleted, makes game run smoothly
	float deltaTime = (SDL_GetTicks() - lastFrameTime) / 1000.0;
	lastFrameTime = SDL_GetTicks();

	//printf("%d\n", array_checkCollisionGround(player, platforms, nPlatforms));

	if (array_checkCollisionGround(player, platforms, nPlatforms) != -1 && player.velocity.vertical > 0) {

		player.grounded = 1;
		player.position.y = platforms[array_checkCollisionGround(player, platforms, nPlatforms)].position.y - player.height;
	}
	else if (array_checkCollisionGround(player, boxes, nBoxes) != -1 && player.velocity.vertical > 0) {

		player.grounded = 1;
		player.position.y = boxes[array_checkCollisionGround(player, boxes, nBoxes)].position.y - player.height;
	}
	else
	{
		player.grounded = 0;
	}

	if (array_checkCollisionCeiling(player, platforms, nPlatforms) != -1)
	{
		player.position.y = platforms[array_checkCollisionCeiling(player, platforms, nPlatforms)].position.y + platforms[array_checkCollisionCeiling(player, platforms, nPlatforms)].height;
		player.velocity.vertical = 0;
	}

	player.position.x += player.velocity.horizontal * deltaTime;

	if (array_checkCollisionWallLeft(player, platforms, nPlatforms) != -1)
	{
		player.position.x = platforms[array_checkCollisionWallLeft(player, platforms, nPlatforms)].position.x - player.width;
	}
	else if (array_checkCollisionWallLeft(player, boxes, nBoxes) != -1 &&
		boxes[array_checkCollisionWallLeft(player, boxes, nBoxes)].grounded == 1)
	{
		player.position.x = boxes[array_checkCollisionWallLeft(player, boxes, nBoxes)].position.x - player.width;
	}

	if (array_checkCollisionWallRight(player, platforms, nPlatforms) != -1)
	{
		player.position.x = platforms[array_checkCollisionWallRight(player, platforms, nPlatforms)].position.x + platforms[array_checkCollisionWallRight(player, platforms, nPlatforms)].width;
	}
	else if (array_checkCollisionWallRight(player, boxes, nBoxes) != -1 &&
		boxes[array_checkCollisionWallRight(player, boxes, nBoxes)].grounded == 1)
	{
		player.position.x = boxes[array_checkCollisionWallRight(player, boxes, nBoxes)].position.x + boxes[array_checkCollisionWallRight(player, boxes, nBoxes)].width;
	}

	//printf("%f %f\n", player.position.x, player.velocity.horizontal);

	if (player.grounded == 0) {
		player.position.y += player.velocity.vertical * deltaTime;
		player.velocity.vertical += (STANDARD_GRAVITY * 2) * deltaTime;
	}

	for (int i = 0; i < nBoxes; i++) {

		if (array_checkCollisionGround(boxes[i], platforms, nPlatforms) != -1) {
			boxes[i].position.y = platforms[array_checkCollisionGround(boxes[i], platforms, nPlatforms)].position.y - boxes[i].height;
		}
		else if (array_checkCollisionGround(boxes[i], boxes, nBoxes) != -1) {
			boxes[i].position.y = boxes[array_checkCollisionGround(boxes[i], boxes, nBoxes)].position.y - boxes[i].height;
		}
		else
		{
			boxes[i].position.y += boxes[i].velocity.vertical * deltaTime;
			boxes[i].velocity.vertical += STANDARD_GRAVITY * deltaTime;
		}

		//printf("box:%d pos:%f vel:%f \n", i, boxes[i].position.x, boxes[i].velocity.horizontal);

		//boxes kinda clip through walls, it is caused by the collision detection being kinda ass
		//can be changed by making a seperate collision detection for the player or just making a new one

		if(!boxes[i].grounded)
			boxes[i].position.x += boxes[i].velocity.horizontal * deltaTime;

		if (array_checkCollisionWallLeft(boxes[i], platforms, nPlatforms) != -1 ||
			array_checkCollisionWallRight(boxes[i], platforms, nPlatforms) != -1)
		{
			boxes[i].grounded = 1;
		}
		//maybe in the future make it, so player can push two boxes at once, but thats a big maybe
		else if (array_checkCollisionWallLeft(boxes[i], boxes, nBoxes) != -1)
		{
			boxes[i].grounded = 1;
		}
		else if (array_checkCollisionWallRight(boxes[i], boxes, nBoxes) != -1)
		{
			boxes[i].velocity.horizontal = 0;
			boxes[i].grounded = 1;
		}
		else if ((checkCollisionWallLeft(boxes[i], lightningChildLeft) ||
				  checkCollisionWallRight(boxes[i], lightningChildLeft)) &&
				  lightningChildLeft.isActive)
		{
			boxes[i].velocity.horizontal = lightningChildLeft.velocity.horizontal;
		}
		else if ((checkCollisionWallLeft(boxes[i], lightningChildRight) ||
				  checkCollisionWallRight(boxes[i], lightningChildRight)) &&
				  lightningChildRight.isActive)
		{
			boxes[i].velocity.horizontal = lightningChildRight.velocity.horizontal;
		}
		else if ((checkCollisionWallLeft(boxes[i], player) && player.velocity.horizontal < 0) ||
			(checkCollisionWallRight(boxes[i], player) && player.velocity.horizontal > 0))
		{
			boxes[i].velocity.horizontal = player.velocity.horizontal;
		}
		else if(boxes[i].frame != 1)
		{
			boxes[i].velocity.horizontal = 0;
			boxes[i].grounded = 0;
		}

	}

	if (fireball.isActive)
		fireball.position.x += fireball.velocity.horizontal * deltaTime;

	if (checkOutOfBounds(fireball) ||
		array_checkCollisionWallLeft(fireball, platforms, nPlatforms) != -1 ||
		array_checkCollisionWallRight(fireball, platforms, nPlatforms) != -1)
	    fireball.isActive = 0;
	else if (array_checkCollisionWallRight(fireball, boxes, nBoxes) != -1)
	{
		fireball.isActive = 0;
		boxes[array_checkCollisionWallRight(fireball, boxes, nBoxes)].frame = 0;
		fireball.position.y = -50;
	}
	else if (array_checkCollisionWallLeft(fireball, boxes, nBoxes) != -1)
	{
		fireball.isActive = 0;
		boxes[array_checkCollisionWallLeft(fireball, boxes, nBoxes)].frame = 0;
		fireball.position.y = -50;
	}

	if (lightningActiveTime <= 0.0) {
		lightning.isActive = 0;
	}
	else {

		lightningActiveTime -= deltaTime;
		lightning.position.y += lightning.velocity.vertical * deltaTime;

		if (array_checkCollisionGround(lightning, platforms, nPlatforms) != -1) {

			lightning.velocity.vertical = 0;
			lightning.position.y = platforms[array_checkCollisionGround(lightning, platforms, nPlatforms)].position.y - lightning.height - 0.001;

			lightningChildLeft.position.y = lightning.position.y + lightningChildLeft.height;
			lightningChildLeft.position.x = lightning.position.x - lightning.width;
			lightningChildLeft.isActive = 1;

			lightningChildRight.position.y = lightningChildLeft.position.y;
			lightningChildRight.position.x = lightning.position.x + lightning.width;
			lightningChildRight.isActive = 1;

		}
		else if (array_checkCollisionGround(lightning, boxes, nBoxes) != -1) {

			lightning.velocity.vertical = 0;
			lightning.position.y = boxes[array_checkCollisionGround(lightning, boxes, nBoxes)].position.y - lightning.height - 0.001;

			lightningChildLeft.position.y = lightning.position.y + lightningChildLeft.height;
			lightningChildLeft.position.x = lightning.position.x - lightning.width;
			lightningChildLeft.isActive = 1;

			lightningChildRight.position.y = lightningChildLeft.position.y;
			lightningChildRight.position.x = lightning.position.x + lightning.width;
			lightningChildRight.isActive = 1;

		}

	}

	if (lightningChildLeft.isActive)
	{
		lightningChildLeft.position.x += lightningChildLeft.velocity.horizontal * deltaTime;

		if ((array_checkCollisionWallLeft(lightningChildLeft, boxes, nBoxes) != -1 ||
			array_checkCollisionWallRight(lightningChildLeft, boxes, nBoxes) != -1) &&
			lightningChildLeft.grounded != 1)
		{
			lightningChildLeft.grounded = 1;
			lightningChildLeftActiveTime = LIGHTNING_PUSH_TIME;
		}

	}

	if (lightningChildLeftActiveTime > 0) {
		lightningChildLeftActiveTime -= deltaTime;
	}
	else if (lightningChildLeft.grounded && lightningChildLeftActiveTime <= 0) {
		lightningChildLeft.isActive = 0;
		lightningChildLeft.grounded = 0;
	}

	//printf("%f %d\n", lightningChildRightActiveTime, lightningChildRight.grounded);

	if (lightningChildRight.isActive)
	{
		lightningChildRight.position.x += lightningChildRight.velocity.horizontal * deltaTime;

		if ((array_checkCollisionWallLeft(lightningChildRight, boxes, nBoxes) != -1 ||
			array_checkCollisionWallRight(lightningChildRight, boxes, nBoxes) != -1) &&
			lightningChildRight.grounded != 1)
		{
			lightningChildRight.grounded = 1;
			lightningChildRightActiveTime = LIGHTNING_PUSH_TIME;
		}

	}

	if (lightningChildRightActiveTime > 0) {
		lightningChildRightActiveTime -= deltaTime;
	}
	else if (lightningChildRight.grounded && lightningChildRightActiveTime <= 0) {
		lightningChildRight.isActive = 0;
		lightningChildRight.grounded = 0;
	}

	if (array_checkCollision(teleport, platforms, nPlatforms) ||
		array_checkCollision(teleport, boxes, nBoxes))
	{
		teleport.grounded = 0;
		teleport.frame = 1;
	}
	else {
		teleport.grounded = 1;
		teleport.frame = 0;
	}

	if (blizzardActiveTime > 0) {
		blizzard.position.x = player.position.x;
		blizzard.position.y = player.position.y - blizzard.height;

		blizzardActiveTime -= deltaTime;

		for (int i = 0; i < nBoxes; boxes[i].grounded = 1, boxes[i].frame = 1, i++);
	}
	else
		blizzard.isActive = 0;

	if (checkCollision(player, goal)) {
		player.position.x = 64;
		player.position.y = 512;
		levelID++;
		readLevelData(levelID);
	}
}