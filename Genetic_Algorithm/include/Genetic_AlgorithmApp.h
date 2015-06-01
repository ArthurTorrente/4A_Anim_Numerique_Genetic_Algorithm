#ifndef _GENETICALGORITHMAPP_H_
#define _GENETICALGORITHMAPP_H_

#include "cinder/app/AppNative.h"
#include "cinder/gl/gl.h"
#include "cinder/ImageIo.h"
#include "cinder/gl/Texture.h"
#include "cinder/gl/TextureFont.h"
#include "cinder/Capture.h"
#include "cinder/params/Params.h"

#include "cinder/Rand.h"
#include "Sticky.h"

#include <vector>

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
    void draw();

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
     * Capture d'une image de la camera
     */
    bool m_hasCaptureCamera;
    cinder::Surface m_videoCapture;

    /**
	* Tableau des sticky
	*/
	std::vector<Sticky> m_StickyArmy;

	/**
	* Taille d'une ligne de sticky
	*/
	int m_RowStickyLength;

	/**
	* Initialise les sticky
	*/
    void initSticky();

    /**
     * IHMs
     */
    cinder::params::InterfaceGlRef m_ihmParam;
    void changeMode();

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
};

#endif //_GENETICALGORITHMAPP_H_