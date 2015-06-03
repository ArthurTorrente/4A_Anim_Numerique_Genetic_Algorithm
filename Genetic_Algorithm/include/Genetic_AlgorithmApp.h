#ifndef _GENETICALGORITHMAPP_H_
#define _GENETICALGORITHMAPP_H_

#include <vector>

#include "cinder/app/AppNative.h"
#include "cinder/gl/gl.h"
#include "cinder/ImageIo.h"
#include "cinder/gl/Texture.h"
#include "cinder/gl/TextureFont.h"
#include "cinder/Capture.h"
#include "cinder/params/Params.h"
#include "cinder/Rand.h"

#include "Sticky.h"
#include "Stixel.h"
#include "ColorAlgoGen.h"

#include "tools.h"
#include "Image.h"

using namespace ci;
using namespace ci::app;
using namespace std;

class Genetic_AlgorithmApp : public AppNative
{
public:
    /**
    * Main Window event
    */
    void prepareSettings(Settings* settings);
    void setup();
    void setupIHM();
    void update();
    void updateIHM();
    void draw();
    void resize();

    /**
    * Keyboard event
    */
    void keyDown(KeyEvent event);
    void keyUp(KeyEvent event);

    /**
    * Mouse event
    */
    void mouseDown(MouseEvent event);
    void mouseUp(MouseEvent event);
    void mouseWheel(MouseEvent event);
    void mouseMove(MouseEvent event);
    void mouseDrag(MouseEvent event);

    /**
    * File droped into window event
    */
    void fileDrop(FileDropEvent event);

protected:
    /**
     * Mode Webcam ou utilisation d'image (FileDrop ou FileExplorer)
     */
    bool m_cameraMode;

    /**
     * Mode temp reel
     */
    bool m_realTime;

    /**
     * Liste d'image chargé
     */
    unsigned int m_currentImageLoadedIndex;
    std::vector<cinder::Surface> m_imageLoaded;

    /**
     * Camera
     */
    cinder::CaptureRef m_camera;
    Surface m_cameraImage;

    /**
     * Render de string à l'écran
     */
    cinder::gl::TextureFontRef m_renderString;

    /**
     * Capture d'une image de la camera
     */
    bool m_hasCaptureCamera;
    cinder::Surface m_videoCapture;

    /**
	* Tableau des sticky
	*/
	std::vector<Stixel> m_StickyArmy;

	/**
	* Taille d'une ligne de sticky
	*/
	int m_RowStickyLength;

    bool m_isStarted;
    bool m_isPaused;

	/**
	* Initialise les sticky
	*/
    void start();

    /**
    * Initialise les sticky
    */
    void pause();

    /**
    * Détruit les sticky
    */
    void stop();

    /**
     * IHMs
     */
    cinder::params::InterfaceGlRef m_ihmParam;
    void changeMode();
    void loadImage();

	/**
	* Nombre de pixel par groupe
	*/
	int m_pixelGroupNumber;

	/**
	* Nombre de pixel entre deux sticky
	*/
	int m_numberGapPixel;

    /**
     * Algo Gen
     * Image utilisé par l'algo Gen
     */
    cinder::Surface m_currentImage;

    /**
     * Setup de la caméra
     */
    void setupCamera();

    float beforeResizeWidth;
    float beforeResizeHeight;

    cinder::ColorA getAveragePixelColor(int startXIndex, int startYIndex, int groupRowNumber);

    /**
     * Algo gen
     */
    ColorAlgoGen m_algoGen;
};

#endif //_GENETICALGORITHMAPP_H_