#include "stdafx.h"
/*#if 0
#include "stdafx.h"
#include "Text.h"


Text::Text()
{
FT_Library ft;
if (FT_Init_FreeType(&ft))
std::cout << "ERROR::FREETYPE: Could not init FreeType Library" << std::endl;

FT_Face face;
if (FT_New_Face(ft, "fonts/arial.ttf", 0, &face))
std::cout << "ERROR::FREETYPE: Failed to load font" << std::endl;

FT_Set_Pixel_Sizes(face, 0, 48);

if (FT_Load_Char(face, 'X', FT_LOAD_RENDER))
std::cout << "ERROR::FREETYTPE: Failed to load Glyph" << std::endl;

// Disable byte-alignment restriction
glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

// Load first 128 characters of ASCII set
for (GLubyte c = 0; c < 128; c++)
{
// Load character glyph
if (FT_Load_Char(face, c, FT_LOAD_RENDER))
{
std::cout << "ERROR::FREETYTPE: Failed to load Glyph" << std::endl;
continue;
}
// Generate texture
GLuint texture;
glGenTextures(1, &texture);
glBindTexture(GL_TEXTURE_2D, texture);
glTexImage2D(
GL_TEXTURE_2D,
0,
GL_RED,
face->glyph->bitmap.width,
face->glyph->bitmap.rows,
0,
GL_RED,
GL_UNSIGNED_BYTE,
face->glyph->bitmap.buffer
);
// Set texture options
glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
// Now store character for later use
Character character = {
texture,
glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
face->glyph->advance.x
};
Characters.insert(std::pair<GLchar, Character>(c, character));
}
glBindTexture(GL_TEXTURE_2D, 0);
// Destroy FreeType once we're finished
FT_Done_Face(face);
FT_Done_FreeType(ft);
}


Text::~Text()
{
}

void Text::RenderText(std::string text, GLfloat x, GLfloat y, GLfloat scale, GLuint VBO)
{
// Activate corresponding render state
glActiveTexture(GL_TEXTURE0);

// Iterate through all characters
std::string::const_iterator c;
for (c = text.begin(); c != text.end(); c++)
{
Character ch = Characters[*c];

GLfloat xpos = x + ch.Bearing.x * scale;
GLfloat ypos = y - (ch.Size.y - ch.Bearing.y) * scale;

GLfloat w = ch.Size.x * scale;
GLfloat h = ch.Size.y * scale;
// Update VBO for each character
GLfloat vertices[6][4] = {
{ xpos, ypos + h, 0.0, 0.0 },
{ xpos, ypos, 0.0, 1.0 },
{ xpos + w, ypos, 1.0, 1.0 },

{ xpos, ypos + h, 0.0, 0.0 },
{ xpos + w, ypos, 1.0, 1.0 },
{ xpos + w, ypos + h, 1.0, 0.0 }
};
// Render glyph texture over quad
glBindTexture(GL_TEXTURE_2D, ch.TextureID);
// Update content of VBO memory
glBindBuffer(GL_ARRAY_BUFFER, VBO);
glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices); // Be sure to use glBufferSubData and not glBufferData

glBindBuffer(GL_ARRAY_BUFFER, 0);
// Render quad
glDrawArrays(GL_TRIANGLES, 0, 6);
// Now advance cursors for next glyph (note that advance is number of 1/64 pixels)
x += (ch.Advance >> 6) * scale; // Bitshift by 6 to get value in pixels (2^6 = 64 (divide amount of 1/64th pixels by 64 to get amount of pixels))
}
glBindVertexArray(0);
glBindTexture(GL_TEXTURE_2D, 0);
}
#endif*/