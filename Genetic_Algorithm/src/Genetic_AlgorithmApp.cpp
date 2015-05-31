#include "Genetic_AlgorithmApp.h"

/**
 * TODO
 *
 * - FullScreen
 * - Algo Gen
 * - 2 Modes :
 *      - Fenétré : Chargement des images, set des différents params
 *      - FullSceen : Déroulement de l'algo, (Changement de l'image procécé, utilisation de la cam en temps réel, pris d'un snapshoot à l'aide de la cam, load d'une image par une URL, ...)
 * - Suite
 */

void Genetic_AlgorithmApp::prepareSettings(Settings* settings)
{
    settings->setWindowSize(cinder::Vec2i(800, 600));
    settings->setFrameRate(60.0f);

    m_cameraMode = false;
}

void Genetic_AlgorithmApp::setup()
{
    m_font = Font("Calibri", 20);
    m_textureFont = gl::TextureFont::create(m_font);

    m_hasCaptureCamera = false;
    setupCamera();
}

void Genetic_AlgorithmApp::update()
{
    if (m_cameraMode)
    {
        if (m_camera && m_camera->checkNewFrame())
        {
            m_cameraImage = m_camera->getSurface();
            m_cameraTexture = gl::Texture(m_cameraImage);
        }
    }

}

void Genetic_AlgorithmApp::IHM()
{
    std::string message;
    Vec2f position;

    if (m_cameraMode)
    {
        if (!m_camera)
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
        if (m_camera && m_cameraTexture)
        {
            gl::draw(m_cameraTexture, getWindowBounds());

            if (m_hasCaptureCamera)
                gl::draw(m_videoCapture.m_texture, Rectf(0.0f, 0.0f, getWindowWidth() / 10.0f, getWindowHeight() / 10.0f));
        }
    }
    else
    {
        gl::clear(Color(0.0f, 0.0f, 0.0f), true);

        ci::Rectf screen = getWindowBounds();
        ci::Rectf curRect = Rectf::zero();

        unsigned int imageNumber = m_textures.size();
        unsigned int xCount = 0;
        unsigned int yCount = xCount;

        double dressSize = sqrt(imageNumber);

        if (dressSize - static_cast<int>(dressSize) > 0.0)
        {
            dressSize += 1.0;
        }
        
        xCount = static_cast<unsigned int>(dressSize);
        yCount = xCount;

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
    else if (event.getCode() == KeyEvent::KEY_c)
    {
        if (m_cameraMode)
        {
            m_videoCapture.m_image.copyFrom(m_cameraImage, m_cameraImage.getBounds());
            m_videoCapture.m_texture = gl::Texture(m_videoCapture.m_image);

            m_hasCaptureCamera = true;
        }
    }
    else if (event.getCode() == KeyEvent::KEY_f)
    {
        setFullScreen(!isFullScreen());
    }
    else if (event.getCode() == KeyEvent::KEY_l)
    {
        fs::path p = getOpenFilePath("", ImageIo::getLoadExtensions());

        if (!p.empty())
        {
            m_textures.clear();

            try
            {
                gl::Texture texture(loadImage(p));
                m_textures.push_back(texture);
            }
            catch (ci::ImageIoException&)
            {
                console() << "Format non supporté pour l'image : " << p.filename() << std::endl;
            }
            catch (std::exception&)
            {
                console() << "Error occur" << std::endl;
            }
        }
        else
        {
        }
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
    auto deviceList = cinder::Capture::getDevices();

    if (deviceList.size() > 0)
    {
        auto device = deviceList.front();

        m_camera = cinder::Capture::create(1920, 1080);
        
        m_videoCapture.m_image = Surface(m_camera->getSurface());

        m_camera->start();

        m_cameraMode = true;
    }
}

/**
 * Retourne toutes les touches clavier appuyé à l'instant T
 *
 * const std::vector<TouchEvent::Touch>& getActiveTouches() const;
 */

CINDER_APP_NATIVE(Genetic_AlgorithmApp, RendererGl)
