#include"update.h"
#include<SDL.h>
#include"gameObjects.h"
#include"constants.h"
#include"collision.h"
#include"manager.h"
#include<stdio.h>

int lastFrameTime = 0;
extern int levelID;
float deltaTime;
char fireballCollisionIndex = 0;

void deltaTimeCalculation() {

	while (!SDL_TICKS_PASSED(SDL_GetTicks(), lastFrameTime + FRAME_TARGET_TIME));
	//deltaTime cannot be deleted, makes game run smoothly
	deltaTime = (SDL_GetTicks() - lastFrameTime) / 1000.0;
	lastFrameTime = SDL_GetTicks();
	//printf("%d\n", lastFrameTime);

}

void update() {

	//printf("%f %f %f %f\n", player.position.x, player.position.y, player.velocity.horizontal, player.velocity.vertical);

	//printf("%d\n", array_checkCollisionGround(player, platforms, nPlatforms));

	if (array_checkCollisionGround(player, platforms, nPlatforms) != -1 && player.velocity.vertical > 0) {

		player.grounded = 1;
		//I have no clue why, but the player vertical velocity seems to gravitate to 6 for some reason
		if (player.velocity.vertical > 0) player.velocity.vertical = 6;
		player.position.y = platforms[array_checkCollisionGround(player, platforms, nPlatforms)].position.y - player.height;
		if(player.frame > 1) player.frame = 0;
	}
	else if (array_checkCollisionGround(player, boxes, nBoxes) != -1 && player.velocity.vertical > 0) {

		player.grounded = 1;
		//I have no clue why, but the player vertical velocity seems to gravitate to 6 for some reason
		if (player.velocity.vertical > 0) player.velocity.vertical = 6;
		player.position.y = boxes[array_checkCollisionGround(player, boxes, nBoxes)].position.y - player.height;
		if (player.frame > 1)  player.frame = 0;
	}
	else
	{
		player.grounded = 0;
	}

	if (array_checkCollisionCeiling(player, platforms, nPlatforms) != -1)
	{
		player.position.y = platforms[array_checkCollisionCeiling(player, platforms, nPlatforms)].position.y + platforms[array_checkCollisionCeiling(player, platforms, nPlatforms)].height;
		player.velocity.vertical += 10;
	}

	player.position.x += player.velocity.horizontal * deltaTime;

	for (int i = 0; i < nBoxes; i++) {

		if (checkCollisionCeiling(player, boxes[i]))
		{
			if (boxes[i].velocity.vertical > 0) {
				//this means that the player has died
				tCurrentSelect = &tTryAgain;
				gameState = DEATHSTATE;
				return;
			}
			else if (player.velocity.vertical < 0) {
				//this is a problematic part, explained on line 160
				player.velocity.vertical += 10;
			}
		}

	}

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

	if (player.grounded == 0) {
		player.position.y += player.velocity.vertical * deltaTime;
		player.velocity.vertical += (STANDARD_GRAVITY * 2) * deltaTime;
	}

	//printf("time: %f\n", playerAnimationTime);

	if (player.grounded && player.velocity.horizontal != 0) {

		if (playerAnimationTime > 0) {

			playerAnimationTime -= deltaTime;

		}
		else if (playerAnimationTime <= 0) {

			player.frame = ++player.frame % 2;
			playerAnimationTime = PLAYER_WALK_ANIMATION_TIME;

		}

	}
	else if(!player.grounded) {

		if (player.velocity.vertical > 0) {

			player.frame = 3;

		}
		else if (player.velocity.vertical < 0) {

			player.frame = 2;
		}
	}

	for (int i = 0; i < nBoxes; i++) {

		//printf("%d\n", array_checkCollisionGroundBoxes(boxes[i], platforms, nPlatforms));

		//printf("box:%d posx:%f poy:%f velx:%f vely:%f \n", i, boxes[i].position.x, boxes[i].position.y, boxes[i].velocity.horizontal, boxes[i].velocity.vertical);

		//boxes[i].isActive = 0;

		//printf("box:%d grounded:%d \n", i, boxes[i].grounded);

		//printf("%f\n", boxes[i].velocity.vertical);

		if (boxes[i].isActive == 0) {
			if ((checkCollisionWallLeft(boxes[i], player) && player.velocity.horizontal < 0) ||
				(checkCollisionWallRight(boxes[i], player) && player.velocity.horizontal > 0))
			{
				//printf("push\n");
				boxes[i].velocity.horizontal = player.velocity.horizontal;
			}
			else {
				//printf("bruh\n");
				boxes[i].velocity.horizontal = 0;
			}
		}

		if (boxes[i].frame == 0) {
			if (checkCollisionWallLeft(boxes[i], lightningChildLeft) && lightningChildLeft.isActive ||
				checkCollisionWallRight(boxes[i], lightningChildLeft) && lightningChildLeft.isActive) {

				//printf("light hit %d\n", boxes[i].isActive);

				//important note! 
				//the isActive variable is used to determine if the lightning push has started!

				if (boxes[i].position.x > targetPosBox[i]) {

					boxes[i].isActive = 1;

				}
				else if (boxes[i].position.x <= targetPosBox[i] && !boxes[i].isActive) {

					boxes[i].isActive = 1;

					targetPosBox[i] = boxes[i].position.x - 64.0;
					lightningChildLeftActiveTime = -0.1;

				}

			}
			else if (checkCollisionWallRight(boxes[i], lightningChildRight) && lightningChildRight.isActive ||
					 checkCollisionWallLeft(boxes[i], lightningChildRight) && lightningChildRight.isActive) {

				//printf("light hit %d\n", boxes[i].isActive);

				if (boxes[i].position.x < targetPosBox[i]) {

					boxes[i].isActive = 1;

				}
				else if (boxes[i].position.x >= targetPosBox[i] && !boxes[i].isActive) {

					boxes[i].isActive = 1;

					targetPosBox[i] = boxes[i].position.x + 64.0;
					lightningChildRightActiveTime = -0.1;

				}

			}

			if (boxes[i].isActive && boxes[i].position.x < targetPosBox[i]) {
				//boxes[i].position.x = (int)boxes[i].position.x + 1;
				boxes[i].velocity.horizontal = 500;
			}
			else if (boxes[i].isActive && boxes[i].position.x > targetPosBox[i]) {
				//boxes[i].position.x = (int)boxes[i].position.x;
				boxes[i].velocity.horizontal = -500;
			}
			else if (boxes[i].isActive && boxes[i].position.x == targetPosBox[i]) {
				boxes[i].velocity.horizontal = 0;
				boxes[i].isActive = 0;
			}
		}
		else {
			boxes[i].grounded = 1;
			boxes[i].velocity.horizontal = 0;
		}

		if (array_checkCollisionWallLeft(boxes[i], platforms, nPlatforms) != -1)
		{
			//boxes[i].position.x = platforms[array_checkCollisionWallLeft(boxes[i], platforms, nPlatforms)].position.x - 64;
			targetPosBox[i] = boxes[i].position.x;
			boxes[i].velocity.horizontal = 0;
			boxes[i].grounded = 1;
			//printf("%d wall touch1\n", i);
		}
		else if (array_checkCollisionWallRight(boxes[i], platforms, nPlatforms) != -1)
		{	
			//boxes[i].position.x = platforms[array_checkCollisionWallRight(boxes[i], platforms, nPlatforms)].position.x + platforms[array_checkCollisionWallRight(boxes[i], platforms, nPlatforms)].width;
			targetPosBox[i] = boxes[i].position.x;
			boxes[i].velocity.horizontal = 0;
			boxes[i].grounded = 1;
			//printf("%d wall touch2\n", i);
		}
		else if (array_checkCollisionWallLeft(boxes[i], boxes, nBoxes) != -1 )
		{
			//boxes[i].position.x = boxes[array_checkCollisionWallLeft(boxes[i], boxes, nBoxes)].position.x - 64;
			boxes[i].grounded = 1;
			targetPosBox[i] = boxes[i].position.x;
			boxes[i].velocity.horizontal = 0;
			printf("%d box touch1\n", i);
		}
		else if (array_checkCollisionWallRight(boxes[i], boxes, nBoxes) != -1)
		{
			//boxes[i].position.x = boxes[array_checkCollisionWallRight(boxes[i], boxes, nBoxes)].position.x + boxes[array_checkCollisionWallRight(boxes[i], boxes, nBoxes)].width;
			boxes[i].grounded = 1;
			targetPosBox[i] = boxes[i].position.x;
			boxes[i].velocity.horizontal = 0;
			printf("%d box touch2\n", i);
		}
		else {
			boxes[i].grounded = 0;
		}

		if (boxes[i].frame == 0) {
			boxes[i].position.x += boxes[i].velocity.horizontal * deltaTime;
		}
		else {
			boxes[i].grounded = 1;
		}

		if (boxes[i].position.x + 1 > targetPosBox[i] && boxes[i].position.x - 1 < targetPosBox[i] && boxes[i].isActive)
			boxes[i].position.x = targetPosBox[i];

		if (array_checkCollisionGroundBoxes(boxes[i], platforms, nPlatforms) != -1) {

			//mozliwy problem
			boxes[i].position.y = platforms[array_checkCollisionGroundBoxes(boxes[i], platforms, nPlatforms)].position.y - boxes[i].height;
			boxes[i].velocity.vertical = 0;
		}
		else if (array_checkCollisionGroundBoxes(boxes[i], boxes, nBoxes) != -1) {

			boxes[i].velocity.vertical = 0;
			boxes[i].position.y = boxes[array_checkCollisionGroundBoxes(boxes[i], boxes, nBoxes)].position.y - boxes[i].height;
			
		}
		else
		{
			//printf("not touching\n");
			boxes[i].velocity.horizontal = 0;
			//boxes[i].grounded = 1;
			boxes[i].position.y += boxes[i].velocity.vertical * deltaTime;
			boxes[i].velocity.vertical += STANDARD_GRAVITY * deltaTime;
		}
	}

	if (fireball.isActive)
		fireball.position.x += fireball.velocity.horizontal * deltaTime;

	
	if (array_checkCollisionWallRight(fireball, boxes, nBoxes) != -1 && !fireball.grounded)
	{
		boxes[array_checkCollisionWallRight(fireball, boxes, nBoxes)].frame = 0;
		boxes[array_checkCollisionWallRight(fireball, boxes, nBoxes)].grounded = 0;
		fireballCollisionIndex = array_checkCollisionWallRight(fireball, boxes, nBoxes);
		fireball.position.x = boxes[fireballCollisionIndex].position.x + fireball.width;
		fireball.position.y = boxes[fireballCollisionIndex].position.y;
		fireballActiveTime = FIREBALL_DECAY_TIME;
		fireball.velocity.horizontal = 0;
		fireball.grounded = 1;
		fireball.frame = 1;
	}
	else if (array_checkCollisionWallLeft(fireball, boxes, nBoxes) != -1 && !fireball.grounded)
	{
		boxes[array_checkCollisionWallLeft(fireball, boxes, nBoxes)].frame = 0;
		boxes[array_checkCollisionWallLeft(fireball, boxes, nBoxes)].grounded = 0;
		fireballCollisionIndex = array_checkCollisionWallLeft(fireball, boxes, nBoxes);
		fireball.position.x = boxes[fireballCollisionIndex].position.x - fireball.width;
		fireball.position.y = boxes[fireballCollisionIndex].position.y;
		fireballActiveTime = FIREBALL_DECAY_TIME;
		fireball.velocity.horizontal = 0;
		fireball.grounded = 1;
		fireball.frame = 1;
	}
	else if (checkOutOfBounds(fireball))
		fireball.isActive = 0;

	//printf("%f\n", fireballActiveTime);

	if (fireballActiveTime > 0 && fireball.grounded) {
		fireballActiveTime -= deltaTime * 10;
		fireball.position.x = (fireball.flip)? boxes[fireballCollisionIndex].position.x + fireball.width: boxes[fireballCollisionIndex].position.x - fireball.width;
	}
	else if (fireballActiveTime <= 0 && fireball.grounded){
		fireball.isActive = 0;
		fireball.position.y = -128;
	}

	if (lightningActiveTime <= 0.0) {
		lightning.isActive = 0;
	}
	else {

		lightningActiveTime -= deltaTime;
		lightning.position.y += lightning.velocity.vertical * deltaTime;

		if (array_checkCollisionGround(lightning, platforms, nPlatforms) != -1 && lightning.frame == 0) {
			
			lightning.frame = 1;
			lightning.velocity.vertical = 0;
			lightning.position.y = platforms[array_checkCollisionGround(lightning, platforms, nPlatforms)].position.y - lightning.height;

			lightningChildLeft.position.y = lightning.position.y + lightningChildLeft.height;
			lightningChildLeft.position.x = lightning.position.x - lightning.width;
			lightningChildLeft.isActive = 1;

			lightningChildRight.position.y = lightningChildLeft.position.y;
			lightningChildRight.position.x = lightning.position.x + lightning.width;
			lightningChildRight.isActive = 1;

		}
		else if (array_checkCollisionGround(lightning, boxes, nBoxes) != -1 && lightning.frame == 0) {
			
			lightning.frame = 1;
			lightning.velocity.vertical = 0;
			lightning.position.y = boxes[array_checkCollisionGround(lightning, boxes, nBoxes)].position.y - lightning.height;

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

	if (array_checkCollisionTeleport(teleport, platforms, nPlatforms) == 1 ||
		array_checkCollisionTeleport(teleport, boxes, nBoxes) == 1 ||
		checkIsInsideObject(teleport, platforms, nPlatforms) == 1 ||
		checkIsInsideObject(teleport, boxes, nBoxes) == 1 ||
		player.grounded == 0)
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

	if ((checkCollision(player, goal) ||
		checkIsInsideObjectSingular(player, goal)) &&
		goal.frame == 1)
	{
		levelID++;
		freeMemory();
		readLevelData(levelID);
	}

	if ((checkCollision(player, doorKey) ||
		checkIsInsideObjectSingular(player, doorKey)) &&
		goal.frame == 0)
	{
		goal.frame = 1;
	}
}

void pauseUpdate() {

	//add logic in future, so If pressed on a button does an action
	//every button has an action attached to it

	tSelect.position.x = tCurrentSelect->position.x - tSelect.width - 20;
	tSelect.position.y = tCurrentSelect->position.y;

}

void deathUpdate() {

	tSelect.position.x = tCurrentSelect->position.x - tSelect.width - 20;
	tSelect.position.y = tCurrentSelect->position.y;

}

void titleUpdate() {

	tSelect.position.x = tCurrentSelect->position.x - tSelect.width - 20;
	tSelect.position.y = tCurrentSelect->position.y;
}

void loadScreenUpdate() {

	//Im not actually sure if there will be a load screen at all 
	//The levels load pretty fast and I dont think it would look good

}