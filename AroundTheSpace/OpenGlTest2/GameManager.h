#pragma once
#include <thread>
#include <vector>
#include <mutex>

#include <iostream>
#include <Windows.h>
#include<GL\glew.h>
#include <GLFW\glfw3.h>

#include "RenderSystem.h"	
#include "ResourcesManager.h"
#include "MouvementSystem.h"
#include "CameraSystem.h"
#include "PlayerInputSystem.h"
#include "Scene.h"
#include "Battle.h"
#include "cinder/Timer.h"
//#define FREEGLUT_LIB_PRAGMAS 0
//#pragma comment(lib, "freeglut.lib")
//#pragma comment(lib, "opengl32.lib")
//#include <gl/GL.h>
//#include "GL/freeglut.h"
//#include "GL/glext.h"
//#include <stdio.h>
//#include <tchar.h>



#include "cinder/ip/Resize.h"

#include "ColorAlgoGen.h"

#include "tools.h"
#include "Image.h"

#include "cinder/ImageIo.h"
//#include "cinder/app/App.h"
#include "Resources.h"
#include "cinder/CinderResources.h"

using namespace ci;
using namespace std;

class GameManager
{
private :
	bool _running;
	RenderSystem* _render;
	ResourcesManager* _resourcesManager;
	MouvementSystem* _movementSystem;
	CameraSystem* _cameraSystem;
	GLFWwindow* _window;
	PlayerInputSystem *_playerInputSystem;
	Scene* _scene;

	vector<Battle*> *battles;

	int turn;

public:
	GameManager(bool running);
	~GameManager();


	static GameManager& getGameManager();
	void runGameLoop();
	//void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
	void moveCamera();
	void destroyGameMangaer();
	void NextTurn();
	void fire(int chessPos, GLfloat* x, GLfloat* y, GLfloat* z, GLfloat* it, Entity* entity);
	void return_chessLocation(int chessPos, GLfloat* x, GLfloat* y, GLfloat* z, GLfloat* it, GLfloat* it2);




	/**
	* Liste d'image chargé
	*/
	unsigned int m_currentImageLoadedIndex;
	std::vector<cinder::Surface> m_imageLoaded;

	/**
	* Next step de l'algo Gen
	*/
	void nextStep();

	/**
	* Algo gen
	*/
	ColorAlgoGen m_algoGen;

	cinder::ColorA getAveragePixelColor(int startXIndex, int startYIndex, int groupRowNumber);

	/*
	Threading
	*/
	int m_numberOfPopulation;
	std::mutex m_mutex;
	double m_computeFPS;
	bool m_threadRunning;
	std::thread m_thread;
	void threadingCompute();

	/**
	* Algo Gen
	* Image utilisé par l'algo Gen
	*/
	cinder::Surface m_currentImage;
	/* Image reduite pour l'algoGen */
	cinder::Surface m_currentAlgoGenImage;
	/*
	Image retourné par l'algoGen
	*/
	cinder::Surface m_algoGenImage;
	/* Rescale de l'image en paramètre et set current image */
	void setCurrentImage(const cinder::Surface&);

	bool m_isStarted;
	bool m_isPaused;

	bool m_renderCurrentImage;

	void GameManager::invertArea(Surface *surface, Area area);

	void GameManager::thresholdArea(Surface *surface, Area area);

	void GameManager::edgeDetectArea(Surface *surface, Area area);

	Surface GameManager::processImage(const Surface input);
	
};

