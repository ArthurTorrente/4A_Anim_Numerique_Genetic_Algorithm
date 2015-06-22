//
//  Matrix3.h
//  SimpleFPS
//
//  Created by Dimitriy Dounaev on 22/12/13.
//  Copyright (c) 2013 Dimitriy Dounaev. All rights reserved.
//

#include <Windows.h>
#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "GLU32.lib")
#include<GL/glew.h>
#include<GLFW\glfw3.h>
#include<GL\GLU.h>

#ifndef __SimpleFPS__Matrix3__
#define __SimpleFPS__Matrix3__

#include <iostream>


typedef struct
{
    GLfloat m00;
    GLfloat m01;
    GLfloat m02;
    GLfloat m10;
    GLfloat m11;
    GLfloat m12;
    GLfloat m20;
    GLfloat m21;
    GLfloat m22;
    
} Matrix3;

Matrix3 scalerMultiplyMatrix3(Matrix3 matrix, GLfloat scalerValue);
Matrix3 addMatrix3(Matrix3 matrixA, Matrix3 matrixB);


const Matrix3 identityMatrix3 = { 1.0f, 0.0f, 0.0f,
0.0f, 1.0f, 0.0f,
0.0f, 0.0f, 1.0f };

Matrix3 makeRotationMatrix3(GLfloat angle, GLfloat xAxis, GLfloat yAxis, GLfloat zAxis);


#endif /* defined(__SimpleFPS__Matrix3__) */
