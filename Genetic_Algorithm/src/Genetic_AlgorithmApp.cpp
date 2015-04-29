#include "cinder/app/AppNative.h"
#include "cinder/gl/gl.h"
#include "cinder/ImageIo.h"
#include "cinder/gl/Texture.h"
#include "cinder/gl/TextureFont.h"

#include <vector>

#include "tools.h"

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
    void textIHM();
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
    std::vector<gl::Texture> m_textures;

    Font m_font;
    gl::TextureFontRef m_textureFont;
};

void Genetic_AlgorithmApp::prepareSettings(Settings* settings)
{
    settings->setWindowSize(Vec2i(800, 600));
    settings->setFrameRate(60.0f);
}

void Genetic_AlgorithmApp::setup()
{
    m_font = Font("Calibri", 20);
    m_textureFont = gl::TextureFont::create(m_font);
}

void Genetic_AlgorithmApp::update()
{
}

void Genetic_AlgorithmApp::textIHM()
{
    m_textureFont->drawString("String IHM", Vec2f(30.0f, 30.0f));
}

void Genetic_AlgorithmApp::draw()
{
    gl::clear(Color(0.0f, 0.0f, 0.0f), true);

    ci::Rectf screen = getWindowBounds();
    ci::Rectf curRect = Rectf::zero();
    
    unsigned int imageNumber = m_textures.size();
    unsigned int xCount = static_cast<unsigned int>(sqrt(imageNumber));
    unsigned int yCount = xCount;

    float deltaX = screen.getWidth() / xCount;
    float deltaY = screen.getHeight() / yCount;
    
    curRect.set(0.0f, 0.0f, deltaX, deltaY);

    for (auto it = m_textures.begin(); it != m_textures.end(); ++it)
    {
        gl::draw(*it, curRect);

        curRect.x1 += deltaX;
        curRect.x2 += deltaX;

        if (curRect.x1 == screen.x2)
        {
            curRect.y1 += deltaY;
            curRect.y2 += deltaY;

            curRect.x1 = 0.0f;
            curRect.x2 = deltaX;
        }
    }

    textIHM();
}

void Genetic_AlgorithmApp::keyDown(KeyEvent event)
{
    if (event.getCode() == KeyEvent::KEY_ESCAPE)
        quit();
}

void Genetic_AlgorithmApp::keyUp(KeyEvent event)
{

}

void Genetic_AlgorithmApp::mouseDown(MouseEvent event)
{
}

void Genetic_AlgorithmApp::mouseUp(MouseEvent event)
{

}

void Genetic_AlgorithmApp::mouseWheel(MouseEvent event)
{

}

void Genetic_AlgorithmApp::mouseMove(MouseEvent event)
{

}

void Genetic_AlgorithmApp::mouseDrag(MouseEvent event)
{

}

void Genetic_AlgorithmApp::fileDrop(FileDropEvent event)
{
    m_textures.clear();

    unsigned int fileCount = event.getNumFiles();

    fs::path path;

    for (unsigned int i = 0; i < fileCount; ++i)
    {
        try
        {
            path = event.getFile(i);
            gl::Texture texture(loadImage(path));
            m_textures.push_back(texture);
        }
        catch (ci::ImageIoException&)
        {
            console() << "Format non supporté pour l'image : " << path.filename() << std::endl;
        }
        catch (std::exception&)
        {
            console() << "Error occur" << std::endl;
        }
        
    }
}

/**
 * Retourne toutes les touches clavier appuyé à l'instant T
 *
 * const std::vector<TouchEvent::Touch>& getActiveTouches() const;
 */

CINDER_APP_NATIVE(Genetic_AlgorithmApp, RendererGl)
