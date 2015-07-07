#include "stdafx.h"
#include "RenderSystem.h"
#include <stdio.h>
#include <iostream>
#include <sstream>

using namespace std;

Entity* RenderSystem::getCurrentCamera()
{
	return _currentCamera;
}

void RenderSystem::setCurrentCamera(Entity* currentCamera)
{
	_currentCamera = currentCamera;
}

RenderSystem::RenderSystem() : _window(glfwGetCurrentContext()), _cameraSystem(&CameraSystem::getCameraSystem())
{
	_currentCamera = _cameraSystem->getCurrentCamera();
	vector<const GLchar*> faces;
	faces.push_back("right.jpg");
	faces.push_back("left.jpg");
	faces.push_back("top.jpg");
	faces.push_back("bottom.jpg");
	faces.push_back("back.jpg");
	faces.push_back("front.jpg");
	_cubemaptexture = loadCubemap(faces);

	// Configure depth map FBO
	const GLuint SHADOW_WIDTH = 1024, SHADOW_HEIGHT = 1024;

	glGenFramebuffers(1, &depthMapFBO);
	// - Create depth texture

	glGenTextures(1, &depthMap);
	glBindTexture(GL_TEXTURE_2D, depthMap);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMap, 0);
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

}

RenderSystem::~RenderSystem()
{

}

void RenderSystem::render(std::vector<Entity*> *children, std::vector<light *>* lights)
{
	if (shadowDepth)
	{
		renderDepth(children, lights);
	}
	static int time;

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glViewport(0.0f, 0.0f, 1920.0f, 1080.0f);

	for (std::vector<Entity*>::iterator iterator = children->begin(); iterator != children->end(); iterator++)
	{
		Entity* entity = *iterator;

		if (entity->get_vertexBuffer() != NULL) {


			if (shadowDepth && !entity->get_vertexBuffer()->_cube) {
				glUseProgram(entity->get_vertexBuffer()->_shaderDepthFramed->getProgramHandle());
				setMatrices(entity, entity->get_vertexBuffer()->_shaderDepthFramed);
			}
			else {
				glUseProgram(entity->get_vertexBuffer()->get_Shader()->getProgramHandle());
				setMatrices(entity, entity->get_vertexBuffer()->get_Shader());
			}




			/**/
			if (shadowDepth)
			{
				glActiveTexture(GL_TEXTURE1);
				glBindTexture(GL_TEXTURE_2D, depthMap);
			}







			entity->get_vertexBuffer()->configureVertexAttributes();
			entity->get_vertexBuffer()->renderVertexBuffer();
			glBindTexture(GL_TEXTURE_2D, 0);
		}
	}

	//RenderQuad();

	glfwSwapBuffers(_window);
	glfwPollEvents();
}

void RenderSystem::setMatrices(Entity* entity, ShaderInterface* shader)
{
	glm::mat4 m_projectionMatrix = glm::perspective(45.0f, (1920.0f / 1080.0f), 0.1f, 1000.0f);
	GLuint transformLoc = glGetUniformLocation(shader->getProgramHandle(), "projectionMatrix");
	glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(m_projectionMatrix));

	GLfloat radius = 2.0f;
	GLfloat camX = sin(glfwGetTime()) * radius;
	GLfloat camZ = cos(glfwGetTime()) * radius;

	glm::mat4 m_modelMatrix;

	m_modelMatrix = glm::translate(m_modelMatrix, glm::vec3(entity->get_position().x, entity->get_position().y, entity->get_position().z));

	/*m_modelMatrix = glm::rotate(m_modelMatrix, entity->get_rotation().x, glm::vec3(1.0f, 0.0f, 0.0f));
	m_modelMatrix = glm::rotate(m_modelMatrix, entity->get_rotation().y, glm::vec3(0.0f, 1.0f, 0.0f));
	m_modelMatrix = glm::rotate(m_modelMatrix, entity->get_rotation().z, glm::vec3(0.0f, 0.0f, 1.0f));*/

	glm::vec3 vec(entity->get_scale().x, entity->get_scale().y, entity->get_scale().z);
	m_modelMatrix = glm::scale(m_modelMatrix, vec);

	float f = glfwGetTime() * 3.0f;

	if (up_down == true) {
		f = next_Step + (next_Step - f);
	}


	if (f >= next_Step)
	{
		next_Step += 180;
		up_down = !up_down;
	}
	GLuint transformLo = glGetUniformLocation(shader->getProgramHandle(), "time");
	glUniform1f(transformLo, f);

	GLuint transformLoc2 = glGetUniformLocation(shader->getProgramHandle(), "modelMatrix");
	glUniformMatrix4fv(transformLoc2, 1, GL_FALSE, glm::value_ptr(m_modelMatrix));

	glm::mat4 view;
	if (entity->get_vertexBuffer()->_cube) {
		view = glm::mat4(glm::mat3(glm::lookAt(glm::vec3(_currentCamera->get_position().x, _currentCamera->get_position().y, _currentCamera->get_position().z),
			glm::vec3(_currentCamera->get_eyeVector().x, _currentCamera->get_eyeVector().y, _currentCamera->get_eyeVector().z),
			glm::vec3(_currentCamera->get_upVector().x, _currentCamera->get_upVector().y, _currentCamera->get_upVector().z))));

		GLuint transformLoc3 = glGetUniformLocation(shader->getProgramHandle(), "viewMatrix");
		glUniformMatrix4fv(transformLoc3, 1, GL_FALSE, glm::value_ptr(view));
		glBindTexture(GL_TEXTURE_CUBE_MAP, _cubemaptexture);
		glDepthFunc(GL_LEQUAL);
		//glDepthMask(GL_FALSE);
	}
	else {
		//glDepthMask(GL_TRUE);
		glDepthFunc(GL_LESS);
		view = glm::lookAt(glm::vec3(_currentCamera->get_position().x, _currentCamera->get_position().y, _currentCamera->get_position().z),
			glm::vec3(_currentCamera->get_eyeVector().x, _currentCamera->get_eyeVector().y, _currentCamera->get_eyeVector().z),
			glm::vec3(_currentCamera->get_upVector().x, _currentCamera->get_upVector().y, _currentCamera->get_upVector().z));

		GLuint transformLoc3 = glGetUniformLocation(shader->getProgramHandle(), "viewMatrix");
		glUniformMatrix4fv(transformLoc3, 1, GL_FALSE, glm::value_ptr(view));
		glUniform3f(glGetUniformLocation(shader->getProgramHandle(), "cameraPos"), _currentCamera->get_position().x, _currentCamera->get_position().y, _currentCamera->get_position().z);



		if (cinderImage) {
			GLuint texture;

			glGenTextures(1, &texture);
			glBindTexture(GL_TEXTURE_2D, texture); // All upcoming GL_TEXTURE_2D operations now have effect on this texture object
			// Set the texture wrapping parameters
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// Set texture wrapping to GL_REPEAT (usually basic wrapping method)
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
			// Set texture filtering parameters
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			// Load image, create texture and generate mipmaps
			//int 512, 512;
			//unsigned char* image = SOIL_load_image(s, &width, &height, 0, SOIL_LOAD_RGB);
			uint8_t* buffer = _m_algoGenImage.getData();

			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, _m_algoGenImage.getWidth(), _m_algoGenImage.getHeight(), 0, GL_RGB, GL_UNSIGNED_BYTE, buffer);
			glGenerateMipmap(GL_TEXTURE_2D);

			glBindTexture(GL_TEXTURE_2D, 0);

			//glDepthFunc();
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, texture);
		}
		else {
			if (!shadowDepth) {
				entity->get_vertexBuffer()->renderMaterials();
			}
			else {
				glActiveTexture(GL_TEXTURE0);

				glBindTexture(GL_TEXTURE_2D, entity->get_vertexBuffer()->_materials->at(0)->get_textureID());
			}

		}

		if (shadowDepth) {
			glm::mat4 lightProjection, lightView;
			glm::mat4 lightSpaceMatrix;
			GLfloat near_plane = 1.0f, far_plane = 7.5f;
			lightProjection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, near_plane, far_plane);
			lightView = glm::lookAt(glm::vec3(0.2f, 20.0f, 20.3f), glm::vec3(0.0f), glm::vec3(1.0));
			lightSpaceMatrix = lightProjection * lightView;
			// - render scene from light's point of view
			glUniformMatrix4fv(glGetUniformLocation(shader->getProgramHandle(), "lightSpaceMatrix"), 1, GL_FALSE, glm::value_ptr(lightSpaceMatrix));
		}
		else {
			glUniform3f(glGetUniformLocation(shader->getProgramHandle(), "dirLight.direction"), 0.2f, 20.0f, 20.3f);
			glUniform3f(glGetUniformLocation(shader->getProgramHandle(), "dirLight.ambient"), 0.8f, 0.8f, 0.8f);
			glUniform3f(glGetUniformLocation(shader->getProgramHandle(), "dirLight.diffuse"), 1.0f, 1.0f, 1.0f);
			glUniform3f(glGetUniformLocation(shader->getProgramHandle(), "dirLight.specular"), 1.0f, 1.0f, 1.0f);
		}

		/*GLint matAmbientLoc = glGetUniformLocation(shader->getProgramHandle(), "material.ambient");
		GLint matDiffuseLoc = glGetUniformLocation(shader->getProgramHandle(), "material.diffuse");
		GLint matSpecularLoc = glGetUniformLocation(shader->getProgramHandle(), "material.specular");
		GLint matShineLoc = glGetUniformLocation(shader->getProgramHandle(), "material.shininess");

		glUniform3f(matAmbientLoc, 1.0f, 1.0f, 1.00f);
		glUniform3f(matDiffuseLoc, 1.0f, 1.0f, 1.0f);
		glUniform3f(matSpecularLoc, 0.5f, 0.5f, 0.5f);
		glUniform1f(matShineLoc, 32.0f);*/

		//renderLights(entity, lights);

		GLint viewPosLoc = glGetUniformLocation(shader->getProgramHandle(), "viewPos");
		glUniform3f(viewPosLoc, _currentCamera->get_position().x, _currentCamera->get_position().y, _currentCamera->get_position().z);

		glm::vec3 lightPos(0.2f, 20.0f, 20.3f);
		glUniform3fv(glGetUniformLocation(shader->getProgramHandle(), "lightPos"), 1, &lightPos[0]);
	}
}

GLuint quadVAO = 0;
GLuint quadVBO;
void RenderSystem::RenderQuad()
{
	if (quadVAO == 0)
	{
		GLfloat quadVertices[] = {
			// Positions        // Texture Coords
			-1.0f, 1.0f, 0.0f, 0.0f, 1.0f,
			-1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
			1.0f, 1.0f, 0.0f, 1.0f, 1.0f,
			1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
		};
		// Setup plane VAO
		glGenVertexArrays(1, &quadVAO);
		glGenBuffers(1, &quadVBO);
		glBindVertexArray(quadVAO);
		glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)0);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	}
	glBindVertexArray(quadVAO);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	glBindVertexArray(0);
}

void RenderSystem::renderDepth(std::vector<Entity*> *children, std::vector<light *>* lights)
{

	glViewport(0, 0, 1024, 1024);
	glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
	glClear(GL_DEPTH_BUFFER_BIT);

	for (std::vector<Entity*>::iterator iterator = children->begin(); iterator != children->end(); iterator++)
	{
		Entity* entity = *iterator;

		if (entity->get_vertexBuffer() != NULL && entity->get_vertexBuffer()->_cube == false) {

			glUseProgram(entity->get_vertexBuffer()->_shaderDepth->getProgramHandle());

			glm::mat4 lightProjection, lightView;
			glm::mat4 lightSpaceMatrix;
			GLfloat near_plane = 1.0f, far_plane = 7.5f;
			lightProjection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, near_plane, far_plane);
			lightView = glm::lookAt(glm::vec3(0.2f, 20.0f, 20.3f), glm::vec3(0.0f), glm::vec3(1.0));
			lightSpaceMatrix = lightProjection * lightView;
			// - render scene from light's point of view
			glUniformMatrix4fv(glGetUniformLocation(entity->get_vertexBuffer()->_shaderDepth->getProgramHandle(), "lightSpaceMatrix"), 1, GL_FALSE, glm::value_ptr(lightSpaceMatrix));

			glm::mat4 m_modelMatrix;

			m_modelMatrix = glm::translate(m_modelMatrix, glm::vec3(entity->get_position().x, entity->get_position().y, entity->get_position().z));

			/*m_modelMatrix = glm::rotate(m_modelMatrix, entity->get_rotation().x, glm::vec3(1.0f, 0.0f, 0.0f));
			m_modelMatrix = glm::rotate(m_modelMatrix, entity->get_rotation().y, glm::vec3(0.0f, 1.0f, 0.0f));
			m_modelMatrix = glm::rotate(m_modelMatrix, entity->get_rotation().z, glm::vec3(0.0f, 0.0f, 1.0f));*/

			glm::vec3 vec(entity->get_scale().x, entity->get_scale().y, entity->get_scale().z);
			m_modelMatrix = glm::scale(m_modelMatrix, vec);

			GLuint transformLoc2 = glGetUniformLocation(entity->get_vertexBuffer()->_shaderDepth->getProgramHandle(), "modelMatrix");
			glUniformMatrix4fv(transformLoc2, 1, GL_FALSE, glm::value_ptr(m_modelMatrix));


			entity->get_vertexBuffer()->configureVertexAttributes();
			entity->get_vertexBuffer()->renderVertexBuffer();


		}
	}
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

RenderSystem& RenderSystem::getRenderSystem()
{
	static RenderSystem * renderSystem = NULL;

	if (renderSystem == NULL)
	{
		renderSystem = new RenderSystem();

		glClearColor(1.0f, 1.0f, 1.0f, 0.1f);

		glMatrixMode(GL_PROJECTION);
		gluPerspective(45.0f, 1920.0f / 1080.0f, 0.1, 10000);
		glViewport(0.0f, 0.0f, 1920.0f, 1080.0f);
		glMatrixMode(GL_MODELVIEW);
		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LESS);
		glEnable(GL_BLEND);

		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	}

	return *renderSystem;
}

void RenderSystem::renderLights(Entity* entity, std::vector<light *>* lights)
{
	glUniform1f(glGetUniformLocation(entity->get_vertexBuffer()->get_Shader()->getProgramHandle(), "numberoflights"), lights->size());

	for (int i = 0; i < lights->size(); i++)
	{
		stringstream ss;
		string index;
		ss << i;
		index = ss.str();
		glUniform3f(glGetUniformLocation(entity->get_vertexBuffer()->get_Shader()->getProgramHandle(), ("pointLights[" + index + "].position").c_str()), lights->at(i)->get_position().x, lights->at(i)->get_position().y, lights->at(i)->get_position().z);
		glUniform3f(glGetUniformLocation(entity->get_vertexBuffer()->get_Shader()->getProgramHandle(), ("pointLights[" + index + "].ambient").c_str()), lights->at(i)->get_ambient().x, lights->at(i)->get_ambient().y, lights->at(i)->get_ambient().z);
		glUniform3f(glGetUniformLocation(entity->get_vertexBuffer()->get_Shader()->getProgramHandle(), ("pointLights[" + index + "].diffuse").c_str()), lights->at(i)->get_diffuse().x, lights->at(i)->get_diffuse().y, lights->at(i)->get_diffuse().z);
		glUniform3f(glGetUniformLocation(entity->get_vertexBuffer()->get_Shader()->getProgramHandle(), ("pointLights[" + index + "].specular").c_str()), lights->at(i)->get_specular().x, lights->at(i)->get_specular().y, lights->at(i)->get_specular().z);
		glUniform1f(glGetUniformLocation(entity->get_vertexBuffer()->get_Shader()->getProgramHandle(), ("pointLights[" + index + "].constant").c_str()), lights->at(i)->get_constant());
		glUniform1f(glGetUniformLocation(entity->get_vertexBuffer()->get_Shader()->getProgramHandle(), ("pointLights[" + index + "].linear").c_str()), lights->at(i)->get_linear());
		glUniform1f(glGetUniformLocation(entity->get_vertexBuffer()->get_Shader()->getProgramHandle(), ("pointLights[" + index + "].quadratic").c_str()), lights->at(i)->get_quadratic());
	}
}

void RenderSystem::destroyRenderSystem()
{
	RenderSystem* renderSystem = &getRenderSystem();
	delete renderSystem;
}

GLuint RenderSystem::loadCubemap(vector<const GLchar*> faces)
{
	GLuint textureID;
	glGenTextures(1, &textureID);
	glActiveTexture(GL_TEXTURE0);

	int width, height;
	unsigned char* image;

	glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);
	for (GLuint i = 0; i < faces.size(); i++)
	{
		image = SOIL_load_image(faces[i], &width, &height, 0, SOIL_LOAD_RGB);
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
		SOIL_free_image_data(image);
	}
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);

	return textureID;
}

void RenderSystem::screenshot(char filename[160], int x, int y)
{// get the image data
	uint8_t* buffer = _m_algoGenImage.getData();

	/*long imageSize = x * y * 3;
	//unsigned char *data = new unsigned char[imageSize];
	glReadPixels(0, 0, x, y, GL_BGR, GL_UNSIGNED_BYTE, buffer);// split x and y sizes into bytes
	int xa = x % 256;
	int xb = (x - xa) / 256; int ya = y % 256;
	int yb = (y - ya) / 256;//assemble the header
	unsigned char header[18] = { 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, (char)xa, (char)xb, (char)ya, (char)yb, 24, 0 };
	// write header and data to file
	fstream File(filename, ios::out | ios::binary);
	File.write(reinterpret_cast<char *>(header), sizeof(char) * 18);
	File.write(reinterpret_cast<char *>(buffer), sizeof(char)*3840);
	File.close();

	delete[] buffer;
	buffer = NULL;*/

	/* save that image as another type */
	/*int save_result = SOIL_save_image
	(
	"new_terrain.dds",
	SOIL_SAVE_TYPE_DDS,
	x, y, 0,
	buffer
	);*/

	/* save a screenshot of your awesome OpenGL game engine, running at 1024x768 */
	int save_result = SOIL_save_image
		(
		"aza.tga",
		SOIL_SAVE_TYPE_TGA,
		x, y, 3, buffer
		);

	/* loaded a file via PhysicsFS, need to decompress the image from RAM, */
	/* where it's in a buffer: unsigned char *image_in_RAM */
	/*GLuint tex_2d_from_RAM = SOIL_load_OGL_texture_from_memory
	(
	image_in_RAM,
	image_in_RAM_bytes,
	SOIL_LOAD_AUTO,
	SOIL_CREATE_NEW_ID,
	SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_COMPRESS_TO_DXT
	);*/
}