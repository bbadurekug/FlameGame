#pragma once
#ifndef COLLISION_H
#define	COLLISION_H

#include"gameObjects.h"

int checkCollisionGround(GameObject entity, GameObject ground);

int checkCollisionCeiling(GameObject entity, GameObject ground);

int checkCollisionWallLeft(GameObject entity, GameObject ground);

int checkCollisionWallRight(GameObject entity, GameObject ground);

int checkCollision(GameObject entity, GameObject ground);

int checkOutOfBounds(GameObject entity);

int array_checkCollisionGround(GameObject entity, GameObject* ground, int nObjects);

int array_checkCollisionCeiling(GameObject entity, GameObject* ground, int nObjects);

int array_checkCollisionWallLeft(GameObject entity, GameObject* ground, int nObjects);

int array_checkCollisionWallRight(GameObject entity, GameObject* ground, int nObjects);

int array_checkCollision(GameObject entity, GameObject* ground, int nObjects);

#endif 
