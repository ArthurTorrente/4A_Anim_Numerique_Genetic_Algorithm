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
}


RenderSystem::~RenderSystem()
{
}

void RenderSystem::render(std::vector<Entity*> *children, std::vector<light *>* lights, cinder::Surface algoGenImage)
{
	static int time;

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	for (std::vector<Entity*>::iterator iterator = children->begin(); iterator != children->end(); iterator++)
	{
		Entity* entity = *iterator;
		
		if (entity->get_vertexBuffer() != NULL) {

			

			glUseProgram(entity->get_vertexBuffer()->get_Shader()->getProgramHandle());



				glm::mat4 m_projectionMatrix = glm::perspective(45.0f, (1600.0f / 900.0f), 0.1f, 1000.0f);
				GLuint transformLoc = glGetUniformLocation(entity->get_vertexBuffer()->get_Shader()->getProgramHandle(), "projectionMatrix");
				glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(m_projectionMatrix));

				GLfloat radius = 2.0f;
				GLfloat camX = sin(glfwGetTime()) * radius;
				GLfloat camZ = cos(glfwGetTime()) * radius;

				glm::mat4 m_modelMatrix;

				m_modelMatrix = glm::translate(m_modelMatrix, glm::vec3(entity->get_position().x, entity->get_position().y, entity->get_position().z));

				m_modelMatrix = glm::rotate(m_modelMatrix, entity->get_rotation().x, glm::vec3(1.0f, 0.0f, 0.0f));
				m_modelMatrix = glm::rotate(m_modelMatrix, entity->get_rotation().y, glm::vec3(0.0f, 1.0f, 0.0f));
				m_modelMatrix = glm::rotate(m_modelMatrix, entity->get_rotation().z, glm::vec3(0.0f, 0.0f, 1.0f));

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
				GLuint transformLo = glGetUniformLocation(entity->get_vertexBuffer()->get_Shader()->getProgramHandle(), "time");
				glUniform1f(transformLo, f);

				GLuint transformLoc2 = glGetUniformLocation(entity->get_vertexBuffer()->get_Shader()->getProgramHandle(), "modelMatrix");
				glUniformMatrix4fv(transformLoc2, 1, GL_FALSE, glm::value_ptr(m_modelMatrix));

				glm::mat4 view;
				if (_cubemaptexture != -1 && entity->get_vertexBuffer()->_cube) {
					view = glm::mat4(glm::mat3(glm::lookAt(glm::vec3(_currentCamera->get_position().x, _currentCamera->get_position().y, _currentCamera->get_position().z),
						glm::vec3(_currentCamera->get_eyeVector().x, _currentCamera->get_eyeVector().y, _currentCamera->get_eyeVector().z),
						glm::vec3(_currentCamera->get_upVector().x, _currentCamera->get_upVector().y, _currentCamera->get_upVector().z))));

					GLuint transformLoc3 = glGetUniformLocation(entity->get_vertexBuffer()->get_Shader()->getProgramHandle(), "viewMatrix");
					glUniformMatrix4fv(transformLoc3, 1, GL_FALSE, glm::value_ptr(view));
					glDepthFunc(GL_LEQUAL);
					//glDepthMask(GL_FALSE);
				}
				else {
					//glDepthMask(GL_TRUE);
					glDepthFunc(GL_LESS);
					view = glm::lookAt(glm::vec3(_currentCamera->get_position().x, _currentCamera->get_position().y, _currentCamera->get_position().z),
						glm::vec3(_currentCamera->get_eyeVector().x, _currentCamera->get_eyeVector().y, _currentCamera->get_eyeVector().z),
						glm::vec3(_currentCamera->get_upVector().x, _currentCamera->get_upVector().y, _currentCamera->get_upVector().z));

					GLuint transformLoc3 = glGetUniformLocation(entity->get_vertexBuffer()->get_Shader()->getProgramHandle(), "viewMatrix");
					glUniformMatrix4fv(transformLoc3, 1, GL_FALSE, glm::value_ptr(view));
					glUniform3f(glGetUniformLocation(entity->get_vertexBuffer()->get_Shader()->getProgramHandle(), "cameraPos"), _currentCamera->get_position().x, _currentCamera->get_position().y, _currentCamera->get_position().z);
				}
				
				//glBindTexture(GL_TEXTURE_CUBE_MAP, _cubemaptexture);

				/*GLuint transformLoc3 = glGetUniformLocation(entity->get_vertexBuffer()->get_Shader()->getProgramHandle(), "viewMatrix");
				glUniformMatrix4fv(transformLoc3, 1, GL_FALSE, glm::value_ptr(view));*/



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
				/*unsigned char* image = SOIL_load_image(s, &width, &height, 0, SOIL_LOAD_RGB);*/
				uint8_t* buffer = algoGenImage.getData();
				//cinder::ImageSourceRef ref = loadImage("manu2.png");
				//cinder::Surface32f = ref;
				//uint8_t* buffer = ;
				glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 512/10, 512/10, 0, GL_RGBA, GL_UNSIGNED_BYTE, buffer);
				glGenerateMipmap(GL_TEXTURE_2D);

				glBindTexture(GL_TEXTURE_2D, 0);

				//glDepthFunc();
				glActiveTexture(GL_TEXTURE0);
				//glActiveTexture(GL_TEXTURE1);
				//glActiveTexture(GL_TEXTURE2);
				//glActiveTexture(GL_TEXTURE3);
				glBindTexture(GL_TEXTURE_2D, texture);
				//glBindTexture(GL_TEXTURE_2D, _materials->at(0)->get_textureID_2());

				/*GLint matDiffuseLoc = glGetUniformLocation(_shader->getProgramHandle(), "material.diffuse");
				GLint matSpecularLoc = glGetUniformLocation(_shader->getProgramHandle(), "material.specular");
				GLint matShineLoc = glGetUniformLocation(_shader->getProgramHandle(), "material.shininess");

				glUniform1i(matDiffuseLoc, _materials->at(0)->get_textureID());
				glUniform1i(matSpecularLoc, _materials->at(0)->get_textureID_2());
				glUniform1f(matShineLoc, _materials->at(0)->get_shineness());*/

				GLint matAmbientLoc = glGetUniformLocation(entity->get_vertexBuffer()->get_Shader()->getProgramHandle(), "material.ambient");
				GLint matDiffuseLoc = glGetUniformLocation(entity->get_vertexBuffer()->get_Shader()->getProgramHandle(), "material.diffuse");
				GLint matSpecularLoc = glGetUniformLocation(entity->get_vertexBuffer()->get_Shader()->getProgramHandle(), "material.specular");
				GLint matShineLoc = glGetUniformLocation(entity->get_vertexBuffer()->get_Shader()->getProgramHandle(), "material.shininess");

				glUniform3f(matAmbientLoc, 1.0f, 1.0f, 1.00f);
				glUniform3f(matDiffuseLoc, 1.0f, 1.0f, 1.0f);
				glUniform3f(matSpecularLoc, 0.5f, 0.5f, 0.5f);
				glUniform1f(matShineLoc, 32.0f);
				

				//entity->get_vertexBuffer()->renderMaterials();
				renderLights(entity, lights);

				GLint viewPosLoc = glGetUniformLocation(entity->get_vertexBuffer()->get_Shader()->getProgramHandle(), "viewPos");
				glUniform3f(viewPosLoc, _currentCamera->get_position().x, _currentCamera->get_position().y, _currentCamera->get_position().z);


			

			entity->get_vertexBuffer()->configureVertexAttributes();
			entity->get_vertexBuffer()->renderVertexBuffer();
		}
	}
	


	glfwSwapBuffers(_window);
	glfwPollEvents();
}

RenderSystem& RenderSystem::getRenderSystem()
{
	static RenderSystem * renderSystem = NULL;

	if (renderSystem == NULL)
	{
		renderSystem = new RenderSystem();

		// Cubemap (Skybox)
		
		

		glClearColor(0.0f,0.0f,0.0f,0.5f);

		glMatrixMode(GL_PROJECTION);
		gluPerspective(45.0f, 1600.0f / 900.0f, 0.1, 10000);
		glViewport(0.0f, 0.0f, 1600.0f, 900.0f);
		glMatrixMode(GL_MODELVIEW);
		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LESS);
		glEnable(GL_BLEND);
		
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	}

	return *renderSystem;
}

void RenderSystem::renderLights(Entity* entity,std::vector<light *>* lights)
{
	glUniform1f(glGetUniformLocation(entity->get_vertexBuffer()->get_Shader()->getProgramHandle(), "numberoflights"), lights->size());

	for (int i = 0; i < lights->size(); i++)
	{
		stringstream ss;
		string index;
		ss << i;
		index = ss.str();
		glUniform3f(glGetUniformLocation(entity->get_vertexBuffer()->get_Shader()->getProgramHandle(), ("pointLights["+index+"].position").c_str()), lights->at(i)->get_position().x, lights->at(i)->get_position().y, lights->at(i)->get_position().z);
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