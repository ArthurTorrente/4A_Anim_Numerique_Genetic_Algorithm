//
//  Vector4.h
//  SimpleFPS
//
//  Created by Dimitriy Dounaev on 8/10/13.
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

#ifndef __SimpleFPS__Vector4__
#define __SimpleFPS__Vector4__

#include <iostream>


typedef struct
{
    GLfloat x;
    GLfloat y;
    GLfloat z;
    GLfloat w;
    
} Vector4;

Vector4 makeVector4(GLfloat x, GLfloat y, GLfloat z, GLfloat w);

#endif /* defined(__SimpleFPS__Vector4__) */
