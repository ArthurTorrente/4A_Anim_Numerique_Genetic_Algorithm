#ifndef _GENETICALGORITHMAPP_H_
#define _GENETICALGORITHMAPP_H_

#include <vector>
#include <thread>

#include "cinder/app/AppNative.h"
#include "cinder/gl/gl.h"
#include "cinder/ImageIo.h"
#include "cinder/gl/Texture.h"
#include "cinder/gl/TextureFont.h"
#include "cinder/Capture.h"
#include "cinder/params/Params.h"
#include "cinder/ip/Resize.h"

#include "Sticky.h"
#include "Stixel.h"
#include "ColorAlgoGen.h"

#include "tools.h"
#include "Image.h"

#include "StickyCamera.h"

using namespace ci;
using namespace ci::app;
using namespace std;

class Genetic_AlgorithmApp : public AppNative
{
public:
    ~Genetic_AlgorithmApp();
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
    bool m_captureMode;

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
     * Camera capture
     */
    cinder::CaptureRef m_capture;
    Surface m_captureImage;

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

    bool m_renderCurrentImage;

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
     * Next step de l'algo Gen
     */
    void nextStep();

    /**
     * IHMs
     */
    cinder::params::InterfaceGlRef m_ihmParam;
    cinder::params::InterfaceGlRef m_ihmStats;
    void toggleDisplayStat();
    void changeMode();
    void toggleDisplayCurrent();
    void loadImage();

	/**
	* Nombre de pixel par groupe
	*/
	int m_pixelPerSticky;

	/**
	* Nombre de pixel entre deux sticky
	*/
	int m_numberGapPixel;

    /**
     * Setup de la caméra
     */
    void setupCapture();

    float m_beforeResizeWidth;
    float m_beforeResizeHeight;

    cinder::ColorA getAveragePixelColor(int startXIndex, int startYIndex, int groupRowNumber);

    /**
     * Algo gen
     */
    ColorAlgoGen m_algoGen;

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

    /* Camera */
    StickyCamera m_camera;
    cinder::params::InterfaceGlRef m_camParam;

    void updateCamParam();
};

#endif //_GENETICALGORITHMAPP_H_