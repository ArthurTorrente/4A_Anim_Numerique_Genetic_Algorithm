#ifndef _GENETICALGORITHMAPP_H_
#define _GENETICALGORITHMAPP_H_

#include "cinder/app/AppNative.h"
#include "cinder/gl/gl.h"
#include "cinder/ImageIo.h"
#include "cinder/gl/Texture.h"
#include "cinder/gl/TextureFont.h"
#include "cinder/Capture.h"

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
    void update();
    void IHM();
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
     * Liste d'image chargé
     */
    std::vector<gl::Texture> m_textures;

    /**
     * Texture utilisé pour le rendu de string
     */
    Font m_font;
    gl::TextureFontRef m_textureFont;

    /**
     * Camera
     */
    cinder::CaptureRef m_camera;
    Surface m_cameraImage;
    gl::Texture m_cameraTexture;

    /**
     * 
     */
    bool m_hasCaptureCamera;
    DrawableImage m_videoCapture;

    /**
     * Setup de la camera
     */
    void setupCamera();

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
	void initSticky()
};

#endif //_GENETICALGORITHMAPP_H_