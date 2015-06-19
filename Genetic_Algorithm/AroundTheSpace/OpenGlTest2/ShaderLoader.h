#pragma once
#include <Windows.h>
#include <GL/glew.h>
#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "glew32.lib")
//#include <GLFW\glfw3.h>

//#include <GL/glx.h>


class ShaderLoader
{
private :
	GLuint _programHandle;
		
public:

	GLuint getProgramHandle();

	GLuint compileShader(GLenum shader, const char* source);

	ShaderLoader(const char* sourceVS, const char* sourceFS);
	~ShaderLoader();
};

