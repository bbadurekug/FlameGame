#pragma once
#ifndef MANAGER_H
#define MANAGER_H

enum GameState {

	TITLESCREEN = 0,
	GAMEPLAY = 1,
	PAUSESCREEN = 2,
	LOADSCREEN = 3,
	DEATHSTATE = 4,
	LEVELEDITOR = 5

};

enum GameState gameState;

#endif 
