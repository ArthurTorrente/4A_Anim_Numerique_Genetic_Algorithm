#pragma once

#if 0
#ifndef _SimpleFps_Text
#define _SimpleFps_Text

#include <Windows.h>
#include <iostream>
#pragma comment(lib, "opengl32.lib")
#include <GL/glew.h>
#include <GLFW\glfw3.h>

#include <SOIL.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <ft2build.h>
#include FT_FREETYPE_H

#include "Vector3.h"

#include <stdio.h>
#include <map>
#include <string>
#include <sstream>

using namespace std;

struct Character {
	GLuint TextureID;   // ID handle of the glyph texture
	glm::ivec2 Size;    // Size of glyph
	glm::ivec2 Bearing;  // Offset from baseline to left/top of glyph
	GLuint Advance;    // Horizontal offset to advance to next glyph
};



class Text
{
public:
	std::map<GLchar, Character> Characters;
	void RenderText(std::string text, GLfloat x, GLfloat y, GLfloat scale, GLuint VBO);

	Text();
	~Text();
};

#endif

#endif