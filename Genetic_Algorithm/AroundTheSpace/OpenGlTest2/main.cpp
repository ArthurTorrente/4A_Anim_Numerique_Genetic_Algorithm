// OpenGlTest2.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include <cstdio>

#include "GameManager.h"


#include <cstdio>

#include <stdlib.h>

#include <vector>
#include <list>

int main(int argc, char** argv)
{
	GameManager* game = &GameManager::getGameManager();
	game->runGameLoop();

	return 0;
}

