//
//  Vector2.cpp
//  SimpleFPS
//
//  Created by Dimitriy Dounaev on 22/12/13.
//  Copyright (c) 2013 Dimitriy Dounaev. All rights reserved.
//

#include "stdafx.h"
#include "Vector2.h"

Vector2 makeVector2(GLfloat x, GLfloat y)
{
	Vector2 newVector;
	newVector.x = x;
	newVector.y = y;

	return newVector;
}