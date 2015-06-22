#pragma once

#ifndef _SimpleFps_Materials
#define _SimpleFps_Materials

#include <Windows.h>
#include <iostream>
#pragma comment(lib, "opengl32.lib")
#include <GL/glew.h>
#include <GLFW\glfw3.h>

#include <SOIL.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Vector3.h"

#include <stdio.h>
#include <string>
#include <sstream>

using namespace std;

class materials
{
private:
	GLuint _textureID;
	GLuint _textureID_2;
	

	GLfloat _shineness;
public:
	materials(const char* s, GLfloat shineness);
	materials(const char* s, const char* s2, GLfloat shineness);
	materials(GLuint id, GLfloat shineness);
	materials(GLuint id, GLuint id2, GLfloat shineness);
	~materials();

	GLuint get_textureID();
	GLuint get_textureID_2();

	GLfloat get_shineness();
};

#endif _SimpleFps_Materials