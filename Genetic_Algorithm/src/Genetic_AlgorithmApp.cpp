#include "Genetic_AlgorithmApp.h"

void Genetic_AlgorithmApp::prepareSettings(Settings* settings)
{
    settings->setWindowSize(Vec2i(800, 600));
    settings->setFrameRate(60.0f);

    m_cameraMode = false;
}

void Genetic_AlgorithmApp::setup()
{
    m_font = Font("Calibri", 20);
    m_textureFont = gl::TextureFont::create(m_font);

    setupCamera();
}

void Genetic_AlgorithmApp::update()
{
    if (m_cameraMode)
    {
        if (m_camNumber >= 0 && m_video.isDeviceSetup(m_camNumber))
        {
            if (m_video.isFrameNew(m_camNumber))
            {
                if (!m_video.getPixels(m_camNumber, m_cameraImage.getData(), true, true))
                {
                    console() << "Copy camera image failed" << std::endl;
                }
            }
        }
    }
}

void Genetic_AlgorithmApp::IHM()
{
    std::string message;
    Vec2f position;

    if (m_cameraMode)
    {
        if (m_camNumber < 0)
        {
            message = "No camera setup";
            position = Vec2f(30.0f, 30.f);
        }
        else
        {
            message = "Cam renderer";
            position = Vec2f(30.0f, 30.f);
        }
    }
    else
    {
        if (m_textures.size() > 0)
        {
            message = "Diapo rendrer";
            position = Vec2f(30.0f, 30.0f);
        }
        else
        {
            message = "No image load";
            position = Vec2f(30.0f, 30.0f);
        }
    }

    m_textureFont->drawString(message, position);
}

void Genetic_AlgorithmApp::draw()
{
    if (m_cameraMode)
    {
        if (m_camNumber >= 0)
        {
            gl::draw(gl::Texture(m_cameraImage));
        }
    }
    else
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
    }

    IHM();
}

void Genetic_AlgorithmApp::keyDown(KeyEvent event)
{
    if (event.getCode() == KeyEvent::KEY_ESCAPE)
        quit();
    
    else if (event.getCode() == KeyEvent::KEY_m)
    {
        m_cameraMode = !m_cameraMode;
    }
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

void Genetic_AlgorithmApp::setupCamera()
{
    int camCount = videoInput::listDevices();

    if (camCount > 0)
    {
        m_camNumber = 0;

        if (m_video.isDeviceSetup(m_camNumber))
            m_video.stopDevice(m_camNumber);

        if (!m_video.setupDevice(m_camNumber))
        {
            m_camNumber = -1;
            console() << "Setup camera failed" << std::endl;
        }
        else
        {
            int width = m_video.getWidth(m_camNumber);
            int height = m_video.getHeight(m_camNumber);
            
            m_cameraImage = Surface(width, height, false, SurfaceChannelOrder::RGB);

            setWindowSize(Vec2i(width, height));

            m_cameraMode = true;
        }
    }
    else
    {
        console() << "No cam detected" << std::endl;
    }
}

/**
 * Retourne toutes les touches clavier appuyé à l'instant T
 *
 * const std::vector<TouchEvent::Touch>& getActiveTouches() const;
 */

CINDER_APP_NATIVE(Genetic_AlgorithmApp, RendererGl)
