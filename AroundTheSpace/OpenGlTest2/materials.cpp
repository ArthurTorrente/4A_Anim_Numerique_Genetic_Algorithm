#include "stdafx.h"
#include "materials.h"

materials::materials(const char* s, GLfloat shineness) : _shineness(shineness)
{
	glGenTextures(1, &_textureID);
	glBindTexture(GL_TEXTURE_2D, _textureID); // All upcoming GL_TEXTURE_2D operations now have effect on this texture object
	// Set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// Set texture wrapping to GL_REPEAT (usually basic wrapping method)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// Set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_REPEAT);

	// Load image, create texture and generate mipmaps
	int width, height;
	unsigned char* image = SOIL_load_image(s, &width, &height, 0, SOIL_LOAD_RGB);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
	glGenerateMipmap(GL_TEXTURE_2D);
	SOIL_free_image_data(image);
	glBindTexture(GL_TEXTURE_2D, 0);
}



materials::materials(const char* s, const char* s2, GLfloat shineness) : _shineness(shineness)
{
	glGenTextures(1, &_textureID);
	glBindTexture(GL_TEXTURE_2D, _textureID); // All upcoming GL_TEXTURE_2D operations now have effect on this texture object
	// Set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// Set texture wrapping to GL_REPEAT (usually basic wrapping method)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// Set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// Load image, create texture and generate mipmaps
	int width, height;
	unsigned char* image = SOIL_load_image(s, &width, &height, 0, SOIL_LOAD_RGB);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
	glGenerateMipmap(GL_TEXTURE_2D);
	SOIL_free_image_data(image);
	glBindTexture(GL_TEXTURE_2D, 0);

	glGenTextures(1, &_textureID_2);
	glBindTexture(GL_TEXTURE_2D, _textureID_2); // All upcoming GL_TEXTURE_2D operations now have effect on this texture object
	// Set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// Set texture wrapping to GL_REPEAT (usually basic wrapping method)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// Set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// Load image, create texture and generate mipmaps
	int width2, height2;
	unsigned char* image2 = SOIL_load_image(s2, &width2, &height2, 0, SOIL_LOAD_RGB);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width2, height2, 0, GL_RGB, GL_UNSIGNED_BYTE, image2);
	glGenerateMipmap(GL_TEXTURE_2D);
	SOIL_free_image_data(image2);
	glBindTexture(GL_TEXTURE_2D, 0);
}

materials::materials(GLuint id, GLfloat shineness) : _textureID(id), _shineness(shineness)
{

}

materials::materials(GLuint id, GLuint id2, GLfloat shineness) : _textureID(id), _textureID_2(id2), _shineness(shineness)
{

}

materials::~materials()
{
}

GLuint materials::get_textureID()
{
	return _textureID;
}
GLuint materials::get_textureID_2()
{
	return _textureID_2;
}

GLfloat materials::get_shineness()
{
	return _shineness;
}
