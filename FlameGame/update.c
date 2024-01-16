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

	//deltaTime cannot be deleted, makes game run smoothly
	deltaTime = (SDL_GetTicks() - lastFrameTime) / 1000.0;
	lastFrameTime = SDL_GetTicks();

}

void update() {

	//printf("%f %f %f %f\n", player.position.x, player.position.y, player.velocity.horizontal, player.velocity.vertical);

	//printf("%d\n", array_checkCollisionGround(player, platforms, nPlatforms));

	if (array_checkCollisionGround(player, platforms, nPlatforms) != -1 && player.velocity.vertical > 0) {

		player.grounded = 1;
		player.position.y = platforms[array_checkCollisionGround(player, platforms, nPlatforms)].position.y - player.height;
		if(player.frame > 1) player.frame = 0;
	}
	else if (array_checkCollisionGround(player, boxes, nBoxes) != -1 && player.velocity.vertical > 0) {

		player.grounded = 1;
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

		if (checkCollisionCeiling(player, boxes[i]) &&
			boxes[i].velocity.vertical > 0)
		{
			//this means that the player has died
			tCurrentSelect = &tTryAgain;
			gameState = DEATHSTATE;
		}

		if (array_checkCollisionGround(boxes[i], platforms, nPlatforms) != -1) {
			boxes[i].position.y = platforms[array_checkCollisionGround(boxes[i], platforms, nPlatforms)].position.y - boxes[i].height;
			boxes[i].velocity.vertical = 0;		
		}
		else if (array_checkCollisionGround(boxes[i], boxes, nBoxes) != -1) {
			boxes[i].position.y = boxes[array_checkCollisionGround(boxes[i], boxes, nBoxes)].position.y - boxes[i].height;
			boxes[i].velocity.vertical = 0;
		}
		else
		{
			boxes[i].position.y += boxes[i].velocity.vertical * deltaTime;
			boxes[i].velocity.vertical += STANDARD_GRAVITY * deltaTime;
		}

		//printf("box:%d pos:%f vel:%f \n", i, boxes[i].position.x, boxes[i].velocity.horizontal);

		if(!boxes[i].grounded)
			boxes[i].position.x += boxes[i].velocity.horizontal * deltaTime;

		//printf("%f\n", boxes[i].velocity.vertical);

		if (array_checkCollisionWallLeft(boxes[i], platforms, nPlatforms) != -1)
		{	
			boxes[i].grounded = 1;
			if(boxes[i].velocity.vertical != 0)
				boxes[i].position.x = (int)boxes[i].position.x - 1;
		}
		else if (array_checkCollisionWallRight(boxes[i], platforms, nPlatforms) != -1)
		{
			boxes[i].grounded = 1;
			if (boxes[i].velocity.vertical != 0)
				boxes[i].position.x = (int)boxes[i].position.x + 1;
		}
		else if (array_checkCollisionWallLeft(boxes[i], boxes, nBoxes) != -1)
		{	
			boxes[i].velocity.horizontal = 0;
			boxes[i].grounded = 1;
		}
		else if (array_checkCollisionWallRight(boxes[i], boxes, nBoxes) != -1)
		{
			boxes[i].velocity.horizontal = 0;
			boxes[i].grounded = 1;
		}
		else if (((checkCollisionWallLeft(player, boxes[i]) && player.flip == 0) ||
				  (checkCollisionWallRight(player, boxes[i]) && player.flip == 1)) &&
				   player.velocity.horizontal == 0)
		{
			boxes[i].velocity.horizontal = 0;
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

		//this whole else if section might need a rework in the future, works fine for now

	}

	if (fireball.isActive)
		fireball.position.x += fireball.velocity.horizontal * deltaTime;

	
	if (array_checkCollisionWallRight(fireball, boxes, nBoxes) != -1 && !fireball.grounded)
	{
		boxes[array_checkCollisionWallRight(fireball, boxes, nBoxes)].frame = 0;
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

		if (array_checkCollisionGround(lightning, platforms, nPlatforms) != -1) {
			
			lightning.frame = 1;
			lightning.velocity.vertical = 0;
			lightning.position.y = platforms[array_checkCollisionGround(lightning, platforms, nPlatforms)].position.y - lightning.height - 0.001;

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