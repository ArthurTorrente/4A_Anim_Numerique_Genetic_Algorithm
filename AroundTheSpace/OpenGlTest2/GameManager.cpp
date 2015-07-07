#include "stdafx.h"
#include "GameManager.h"

#include <iostream>     // cout, endl
#include <fstream>      // fstream

#include <algorithm>    // copy
#include <iterator>     // ostream_operator

#include "cinder/CinderResources.h"


using namespace std;
using namespace boost;

GameManager::GameManager(bool running) :
_running(running),
_window(glfwGetCurrentContext()),
_render(&RenderSystem::getRenderSystem()),
_resourcesManager(&ResourcesManager::getResourcesManager()),
_movementSystem(&MouvementSystem::getMouvementSystem()),
_cameraSystem(&CameraSystem::getCameraSystem()),
_scene(new Scene()),
_playerInputSystem(&PlayerInputSystem::getPlayerInputSystem())
{
	//capture = Capture(640, 480);
	//capture.start();

	auto deviceList = cinder::Capture::getDevices();

	if (deviceList.size() > 0)
	{
		auto device = deviceList.front();

		if (device->checkAvailable())
		{
			capture = cinder::Capture::create(1280, 720, device);

			//m_videoCapture = Surface(m_capture->getSurface());

			capture->start();

			//m_captureMode = true;
		}
	}


	//m_currentImage = processImage(loadImage("manu5.png"));
	m_currentImage =  capture->getSurface();
	m_currentImageLoadedIndex = 0;

	int width = 512;
	int height = 512;



	m_currentAlgoGenImage = ci::ip::resizeCopy(m_currentImage, m_currentImage.getBounds(), ci::Vec2i(width / 5, height / 5));

	m_algoGen.setPopSize(100);
	m_algoGen.setup(m_currentAlgoGenImage.getWidth(), m_currentAlgoGenImage.getHeight());

	m_isStarted = true;
	m_isPaused = false;

	if (m_thread.joinable())
		m_thread.join();

	m_threadRunning = true;

	m_thread = std::thread(std::bind(&GameManager::threadingCompute, this));

	//setupIHM();
}


void GameManager::invertArea(Surface *surface, Area area)
{
	Surface::Iter iter = surface->getIter(area);
	while (iter.line()) {
		while (iter.pixel()) {
			iter.r() = 255 - iter.r();
			iter.g() = 255 - iter.g();
			iter.b() = 255 - iter.b();
		}
	}
}

void GameManager::thresholdArea(Surface *surface, Area area)
{
	Surface::Iter iter = surface->getIter(area);
	while (iter.line()) {
		while (iter.pixel()) {
			// this is not the best way to determine grayscale - see CHANTRAIT<>::grayscale()
			uint8_t gray = (iter.r() + iter.g() + iter.b()) / 3;
			if (gray > 128)
				iter.r() = iter.g() = iter.b() = 255;
			else
				iter.r() = iter.g() = iter.b() = 0;
		}
	}
}

void GameManager::edgeDetectArea(Surface *surface, Area area)
{
	// make a copy of the original before we start writing on it
	Surface inputSurface(surface->clone(area));

	// we'll need to iterate the inputSurface as well as the output surface
	Surface::ConstIter inputIter(inputSurface.getIter());
	Surface::Iter outputIter(surface->getIter(area));

	while (inputIter.line()) {
		outputIter.line();
		while (inputIter.pixel()) {
			outputIter.pixel();
			int32_t sumRed = inputIter.rClamped(0, -1) + inputIter.rClamped(-1, 0) + inputIter.r() * -4 + inputIter.rClamped(1, 0) + inputIter.rClamped(0, 1);
			outputIter.r() = constrain<int32_t>(abs(sumRed), 0, 255);
			int32_t sumGreen = inputIter.gClamped(0, -1) + inputIter.gClamped(-1, 0) + inputIter.g() * -4 + inputIter.gClamped(1, 0) + inputIter.gClamped(0, 1);
			outputIter.g() = constrain<int32_t>(abs(sumGreen), 0, 255);
			int32_t sumBlue = inputIter.bClamped(0, -1) + inputIter.bClamped(-1, 0) + inputIter.b() * -4 + inputIter.bClamped(1, 0) + inputIter.bClamped(0, 1);
			outputIter.b() = constrain<int32_t>(abs(sumBlue), 0, 255);
		}
	}
}

Surface GameManager::processImage(const Surface input)
{
	// make the result be a copy of input, taking care of our upper-left quadrant as well
	Surface resultSurface(input.clone());

	// let's do the upper-right, an inverted image
	invertArea(&resultSurface, Area(input.getWidth() / 2, 0, input.getWidth(), input.getHeight() / 2));

	// now the lower-right, a thresholded image
	thresholdArea(&resultSurface, Area(input.getWidth() / 2, input.getHeight() / 2, input.getWidth(), input.getHeight()));

	// and finally the lower left, an edge-detected image
	edgeDetectArea(&resultSurface, Area(0, input.getHeight() / 2, input.getWidth() / 2, input.getHeight()));

	return resultSurface;
}

void GameManager::nextStep()
{
	ci::Surface tmpImage = m_algoGen(m_currentAlgoGenImage);

	m_mutex.lock();
	{
		m_algoGenImage = tmpImage;
	}
	m_mutex.unlock();
}

void GameManager::threadingCompute()
{
	cinder::Timer timer;

	while (m_threadRunning)
	{
		if (m_isStarted && !m_isPaused)
		{
			timer.start();

			ci::Surface tmpImage = m_algoGen(m_currentAlgoGenImage);

			m_mutex.lock();
			{
				m_algoGenImage = tmpImage;
			}
			m_mutex.unlock();

			//timer.stop();
			m_computeFPS = 1.0 / timer.getSeconds();
		}
	}
}

GameManager::~GameManager()
{
	ResourcesManager::destroyResourcesManager();
	RenderSystem::destroyRenderSystem();
	RenderSystem::destroyRenderSystem();
	PlayerInputSystem::destroyPlayerInputSystem();
}


#define Updates_Per_Second 60.0f

void GameManager::runGameLoop()
{
	double lastime = glfwGetTime();
	double deltaTime = 0.0f;
	double deltaTurnTime = 0.0f;


	while (_running) {

		double currentTime = glfwGetTime();
		deltaTime += (currentTime - lastime)*Updates_Per_Second;
		deltaTurnTime += (currentTime - lastime);
		lastime = currentTime;
		while (deltaTime >= 1.0f) {

			_running = !glfwWindowShouldClose(_window);

			_playerInputSystem->update();
			_movementSystem->update(_scene->getChildren(), deltaTime);
			--deltaTime;
		}

		/*while (deltaTurnTime >= 3.0f)
		{
		NextTurn();
		deltaTurnTime -= 3.0f;
		}*/

		//if (m_algoGenImage != NULL)
		//{
		//m_algoGenImage = capture->getSurface();
		_render->_m_algoGenImage = m_algoGenImage;

		_render->render(_scene->getChildren(), _scene->getLights());
		//}


	}
}

/*
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode){
if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS){
glfwSetWindowShouldClose(window, GL_TRUE);
}
if (key == GLFW_KEY_C && action == GLFW_PRESS){
// Do turn camera
// GameManager::moveCamera();
}
} */

void GameManager::moveCamera(){
	// TODO
}

GameManager& GameManager::getGameManager()
{
	static GameManager *gamemanager = NULL;

	if (gamemanager == NULL) {

		glfwInit();
		glfwWindowHint(GLFW_DEPTH_BITS, 24);
		glfwWindowHint(GLFW_RED_BITS, 8);
		glfwWindowHint(GLFW_BLUE_BITS, 8);
		glfwWindowHint(GLFW_GREEN_BITS, 8);
		glfwWindowHint(GLFW_ALPHA_BITS, 8);
		glfwWindowHint(GLFW_SAMPLES, 16);
		glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
		//GLFWwindow* window = glfwCreateWindow(1920, 1080, "Fps", glfwGetPrimaryMonitor(), NULL);
		GLFWwindow* window = glfwCreateWindow(1920, 1080, "Fps", NULL, NULL);
		glfwMakeContextCurrent(window);

		//glfwSetKeyCallback(window, key_callback);
		glewExperimental = GL_TRUE;
		GLenum error = glewInit();

		gamemanager = new GameManager(true);
	}

	return *gamemanager;
}

void GameManager::destroyGameMangaer()
{
	static GameManager *gamemanager = &getGameManager();

	if (gamemanager != NULL) {
		delete gamemanager;
	}
}



void GameManager::NextTurn()
{
	/*GLfloat x;
	GLfloat y;
	GLfloat z;

	for (int i = 0; i < _scene->getChildren()->size(); ++i)
	{
	for (int j = 0; j < battles->size(); ++j)
	{
	if (_scene->getChildren()->at(i)->chess == battles->at(j)->_chess)
	{
	if (battles->at(j)->s3.size() > turn + 1)
	{
	if (_scene->getChildren()->at(i)->id == battles->at(j)->s3.at(turn))
	{
	if (battles->at(j)->s4.at(turn) == -1)
	{
	fire(_scene->getChildren()->at(i)->chess, &x, &y, &z, &battles->at(j)->s5.at(turn), _scene->getChildren()->at(i));
	}
	else {
	return_chessLocation(_scene->getChildren()->at(i)->chess, &x, &y, &z, &battles->at(j)->s4.at(turn), &battles->at(j)->s5.at(turn));
	_scene->getChildren()->at(i)->set_goal(makeVector3(x, y, z));
	}
	}

	if (_scene->getChildren()->at(i)->id == battles->at(j)->s3.at(turn + 1))
	{
	if (battles->at(j)->s4.at(turn+1) == -1){
	fire(_scene->getChildren()->at(i)->chess, &x, &y, &z, &battles->at(j)->s5.at(turn+1), _scene->getChildren()->at(i));
	}
	else {
	return_chessLocation(_scene->getChildren()->at(i)->chess, &x, &y, &z, &battles->at(j)->s4.at(turn + 1), &battles->at(j)->s5.at(turn + 1));
	_scene->getChildren()->at(i)->set_goal(makeVector3(x, y, z));
	}
	}

	}

	}
	}
	}

	turn+=2;*/
}

void GameManager::fire(int chessPos, GLfloat* x, GLfloat* y, GLfloat* z, GLfloat* it, Entity* entity)
{
	/*Vector3 v;
	Vector3 v2;
	Vector3 v3;
	GLfloat len;

	for (int i = 0; i < _scene->getChildren()->size(); ++i)
	{
	if (_scene->getChildren()->at(i)->id == (int)*it && chessPos == _scene->getChildren()->at(i)->chess)
	{
	v = normalizeVector3(subtractVector3(_scene->getChildren()->at(i)->get_position(), entity->get_position()));
	}
	}

	v2.x = -v.x * 2.0f + entity->get_position().x;
	v2.y = entity->get_position().y;
	v2.z = -v.z * 2.0f + entity->get_position().z;

	v3.x = v.x * 1.5f + entity->get_position().x;
	v3.y = entity->get_position().y;
	v3.z = v.z * 1.5f + entity->get_position().z;

	_scene->createParticle(v3, 0.10f);

	entity->set_goal(entity->get_position());
	entity->set_position(v2);*/
}



void GameManager::return_chessLocation(int chessPos, GLfloat* x, GLfloat* y, GLfloat* z, GLfloat* it, GLfloat* it2)
{
	/*switch (chessPos)
	{
	case 0:
	*x = *it + 0.0f;
	*y = 0.6f;
	*z = *it2 + 0.0f;
	break;
	case 1:
	*x = *it + 20.0f;
	*y = 0.6f;
	*z = *it2 + 0.0f;
	break;
	case 2:
	*x = *it + 0.0f;
	*y = 0.6f;
	*z = *it2 + (-20.0f);
	break;
	case 3:
	*x = *it + 20.0f;
	*y = 0.6f;
	*z = *it2 + (-20.0f);
	break;

	/////////////

	case 4:
	*x =  -9.4f;
	*y = *it + 0.0f;
	*z = *it2 + 0.0f;
	break;
	case 5:
	*x = -9.4f;
	*y = *it + 0.0f;
	*z = *it2 + (-20.0f);
	break;
	case 6:
	*x = -9.4f;
	*y = *it + 15.0f;
	*z = *it2 + 0.0f;
	break;
	case 7:
	*x = -9.4f;
	*y = *it + 15.0f;
	*z = *it2 + (-20.0f);
	break;

	//////////////

	case 8:
	*x = *it + 0.0f;
	*y = *it2 + 0.0f;
	*z = -29.4f;
	break;
	case 9:
	*x = *it + 20.0;
	*y = *it2 + 0.0f;
	*z = -29.4f;
	break;
	case 10:
	*x = *it + 0.0f;
	*y = *it2 + 15.0f;
	*z = (-29.4f);
	break;
	case 11:
	*x = *it + 20.0f;
	*y = *it2 + 15.0f;
	*z = (-29.4f);
	break;
	}*/
}

