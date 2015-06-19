#pragma once


#include <iostream>
#include "Vector3.h"
#include "stdafx.h"
#include "Entity.h"

#ifndef _SimpleFpsMouvementSystem
#define _SimpleFpsMouvementSystem

class MouvementSystem
{
private:
	MouvementSystem();
	~MouvementSystem();
public:
	void update(std::vector<Entity*>* children, int deltaTime);
	float Approach(GLfloat goal, GLfloat Current, float dt);

	static MouvementSystem& getMouvementSystem();
	static void destroyMouvementSystem();
};

#endif