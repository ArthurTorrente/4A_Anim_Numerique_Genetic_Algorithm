//
//  Vector3.h
//  SimpleFPS
//
//  Created by Dimitriy Dounaev on 30/09/13.
//  Copyright (c) 2013 Dimitriy Dounaev. All rights reserved.
//
#pragma once

#include <iostream>
#include <Windows.h>
#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "GLU32.lib")
#include<GL/glew.h>
#include<GLFW\glfw3.h>
#include<GL\GLU.h>

#ifndef __SimpleFPS__Vector3__
#define __SimpleFPS__Vector3__


#include "Matrix3.h"

typedef struct {
    
    GLfloat x;
    GLfloat y;
    GLfloat z;
    
} Vector3;

Vector3 makeVector3(GLfloat x, GLfloat y, GLfloat z);
Vector3 addVector3(Vector3 vectorA, Vector3 vectorB);
Vector3 subtractVector3(Vector3 vectorA, Vector3 vectorB);
Vector3 normalizeVector3(Vector3 vector);
Vector3 scalerMultiplyVector3(Vector3 vectorToMultiply, GLfloat scalerValue);
Vector3 crossProductVector3(Vector3 vectorA, Vector3 vectorB);
GLfloat dotProductVector3(Vector3 vectorA, Vector3 vectorB);
Vector3 transformVector3(Vector3 vector, Matrix3 transformationMatrix);
GLfloat length(Vector3 vector);
#endif /* defined(__SimpleFPS__Vector3__) */
