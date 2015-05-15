#ifndef _GENETICALGORITHMAPP_H_
#define _GENETICALGORITHMAPP_H_

#include "cinder/app/AppNative.h"
#include "cinder/gl/gl.h"
#include "cinder/ImageIo.h"
#include "cinder/gl/Texture.h"
#include "cinder/gl/TextureFont.h"
#include "cinder/Capture.h"

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
    bool m_cameraMode;

    std::vector<gl::Texture> m_textures;

    Font m_font;
    gl::TextureFontRef m_textureFont;

    /**
     * Camera
     */
    cinder::CaptureRef m_camera;
    Surface m_cameraImage;
    gl::Texture m_cameraTexture;

    bool m_hasCaptureCamera;
    Image m_videoCapture;

    void setupCamera();
};

#endif //_GENETICALGORITHMAPP_H_