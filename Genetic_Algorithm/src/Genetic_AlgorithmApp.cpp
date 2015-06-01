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
    /* Algo Gen */
	this->m_pixelGroupNumber = 10;
	this->m_numberGapPixel = 3;
    /* === */

    /* Image Load*/
    m_currentImageLoadedIndex = 0;
    /* === */
    /* Camera */
    setupCamera();
    m_hasCaptureCamera = false;
    m_realTime = false;
    /* === */

    /* IHM */
    m_ihmParam = cinder::params::InterfaceGl::create("Sticky", cinder::Vec2i(200, 200));
    setupIHM();
    /* === */
}

void Genetic_AlgorithmApp::setupIHM()
{
    m_ihmParam->clear();

    m_ihmParam->addButton("Toggle Mode", std::bind(&Genetic_AlgorithmApp::changeMode, this));

    m_ihmParam->addSeparator("Input Options");

    if (m_cameraMode)
    {
        if (!m_camera)
        {
            m_ihmParam->addText("No camera found");
        }
        else
        {
            m_ihmParam->addText("Camera mode");
            m_ihmParam->addParam("Real Time", &m_realTime);
        }
    }
    else
    {
        m_ihmParam->addText("Diaporama mode");
        std::string message = std::to_string(m_imageLoaded.size());
        message += " images load";

        m_ihmParam->addText(message.data());

        message = "Current image : ";
        message += std::to_string(m_currentImageLoadedIndex);

        m_ihmParam->addText(message.data());
    }

    m_ihmParam->addSeparator("Algo Gen Options");
    m_ihmParam->addParam("Pixel par groupe", &m_pixelGroupNumber, "min=1 max=1920 step=1");
    m_ihmParam->addParam("Espacement", &m_numberGapPixel, "min=1 max=10 step=1");
    m_ihmParam->addButton("Generate sticky", std::bind(&Genetic_AlgorithmApp::initSticky, this));
}

void Genetic_AlgorithmApp::update()
{
    if (m_cameraMode)
    {
        if (m_camera && m_camera->checkNewFrame())
        {
            m_cameraImage = m_camera->getSurface();

            if (m_realTime)
                m_currentImage = m_cameraImage;
        }
    }
}

void Genetic_AlgorithmApp::draw()
{
    gl::clear(cinder::Color::black());

    if (m_currentImage)
        gl::draw(gl::Texture(m_currentImage), getWindowBounds());

    for (std::vector<Sticky>::size_type i = 0; i < this->m_StickyArmy.size(); i++)
	{
		this->m_StickyArmy[i].sticky.draw();
	}

    m_ihmParam->draw();
}

void Genetic_AlgorithmApp::keyDown(KeyEvent event)
{
    if (event.getCode() == KeyEvent::KEY_ESCAPE)
    {
        quit();
    }
    else if (event.getCode() == KeyEvent::KEY_m)
    {
        changeMode();
    }
    else if (event.getCode() == KeyEvent::KEY_f)
    {
        setFullScreen(!isFullScreen());
    }
    else if (event.getCode() == KeyEvent::KEY_w)
    {
        m_currentImage = cinder::Surface();
    }
    else if (event.getCode() == KeyEvent::KEY_s)
    {
        this->initSticky();
    }
    else if (event.getCode() == KeyEvent::KEY_d)
    {
        static int hash = 0;
        cinder::Rand myRand(hash++);

        for (auto& stixel : this->m_StickyArmy)
        {
            stixel.sticky.ChangeColor(cinder::ColorA(myRand.nextFloat(0.0f, 1.0f), myRand.nextFloat(0.0f, 1.0f), myRand.nextFloat(0.0f, 1.0f), 0.1f));
        }
    }
    else
    {
        if (m_cameraMode)
        {
            if (event.getCode() == KeyEvent::KEY_c)
            {
                if (m_cameraMode)
                {
                    m_videoCapture.copyFrom(m_cameraImage, m_cameraImage.getBounds());

                    m_hasCaptureCamera = true;

                    m_currentImage = m_videoCapture;
                }
            }
        }
        else
        {
            if (event.getCode() == KeyEvent::KEY_n)
            {
                if (!m_cameraMode && m_imageLoaded.size() > 0)
                {
                    ++m_currentImageLoadedIndex;

                    if (m_currentImageLoadedIndex >= m_imageLoaded.size())
                        m_currentImageLoadedIndex = 0;

                    m_currentImage = m_imageLoaded[m_currentImageLoadedIndex];
                }
            }
            else if (event.getCode() == KeyEvent::KEY_l)
            {
                fs::path p = getOpenFilePath("", ImageIo::getLoadExtensions());

                if (!p.empty())
                {
                    m_imageLoaded.clear();

                    try
                    {
                        m_imageLoaded.push_back(loadImage(p));
                        m_currentImage = m_imageLoaded.front();
                        m_currentImageLoadedIndex = 0;
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
            }
        }
    }

    setupIHM();
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
    m_imageLoaded.clear();

    unsigned int fileCount = event.getNumFiles();

    fs::path path;

    if (fileCount > 0)
    {
        for (unsigned int i = 0; i < fileCount; ++i)
        {
            try
            {
                path = event.getFile(i);
                m_imageLoaded.push_back(loadImage(path));
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

        m_currentImage = m_imageLoaded.front();
        m_currentImageLoadedIndex = 0;
    }

    setupIHM();
}

void Genetic_AlgorithmApp::setupCamera()
{
    auto deviceList = cinder::Capture::getDevices();

    if (deviceList.size() > 0)
    {
        auto device = deviceList.front();
        
        if (device->checkAvailable())
        {
            m_camera = cinder::Capture::create(1920, 1080, device);

            m_videoCapture = Surface(m_camera->getSurface());

            m_camera->start();

            m_cameraMode = true;
        }
    }
}


void Genetic_AlgorithmApp::changeMode()
{
    m_cameraMode = !m_cameraMode;

    if (m_cameraMode)
    {
        if (m_realTime)
            m_currentImage = m_cameraImage;
        else
        {
            if (m_hasCaptureCamera)
                m_currentImage = m_videoCapture;
            else
                m_currentImage = cinder::Surface();
        }
    }
    else
    {
        if (m_imageLoaded.size() > 0)
        {
            m_currentImage = m_imageLoaded[m_currentImageLoadedIndex];
        }
        else
            m_currentImage = cinder::Surface();
    }

    setupIHM();
}

void Genetic_AlgorithmApp::initSticky()
{
    if (!m_currentImage)
        return;

    ci::Rectf screen = getWindowBounds();
	//
    int width = static_cast<int>(this->m_currentImage.getWidth());
    int height = static_cast<int>(this->m_currentImage.getHeight());
	//
    cinder::Rand myRand(0);

    float pixelGroupNumber = static_cast<float>(m_pixelGroupNumber);
    float numberGapPixel = static_cast<float>(m_numberGapPixel);

    this->m_StickyArmy.clear();
    
	for (float i = 0; i < height / this->m_pixelGroupNumber; i++)
	{
		for (float j = 0; j < width / this->m_pixelGroupNumber; j++)
		{
            Stixel currentStix;
            currentStix.sticky = Sticky(pixelGroupNumber - numberGapPixel, pixelGroupNumber - numberGapPixel,
                (j) * pixelGroupNumber, (i) * pixelGroupNumber,
                this->getAveragePixelColor(j * this->m_pixelGroupNumber, i * this->m_pixelGroupNumber, this->m_pixelGroupNumber));
                //cinder::ColorA(myRand.nextFloat(0.0f, 1.0f), myRand.nextFloat(0.0f, 1.0f), myRand.nextFloat(0.0f, 1.0f), 0.1f));
            
            currentStix.pixel = Pixel((j + 1.f) * pixelGroupNumber, (i + 1.0f) * pixelGroupNumber, this->getAveragePixelColor(j * this->m_pixelGroupNumber, i * this->m_pixelGroupNumber, this->m_pixelGroupNumber));
            this->m_StickyArmy.push_back(currentStix);
		}
	}
}
#if 0
cinder::ColorA Genetic_AlgorithmApp::getAveragePixelColor(int startXIndex, int startYIndex, int groupRowNumber)
{
    float redColor = 0.f;
    float greenColor = 0.f;
    float blueColor = 0.f;
    for (int i = startYIndex; i < startYIndex + groupRowNumber; i++)
    {
        for (int j = startXIndex; j < startXIndex + groupRowNumber; j++)
        {
            redColor += this->m_currentImage.getPixel(cinder::Vec2i(j, i)).r;
            greenColor += this->m_currentImage.getPixel(cinder::Vec2i(j, i)).g;
            blueColor += this->m_currentImage.getPixel(cinder::Vec2i(j, i)).b;
        }
    }
    float numberElement = cinder::math<float>::pow(groupRowNumber, 2);
    redColor /= numberElement;
    greenColor /= numberElement;
    blueColor /= numberElement;
    //
    redColor = cinder::math<float>::clamp(redColor);
    greenColor = cinder::math<float>::clamp(greenColor);
    blueColor = cinder::math<float>::clamp(blueColor);
    //
    std::ostringstream ss;
    ss << redColor;
    OutputDebugStringA(ss.str().c_str());

    return cinder::ColorA(redColor,greenColor,blueColor,1.0f);
}
#endif

cinder::ColorA Genetic_AlgorithmApp::getAveragePixelColor(int startXIndex, int startYIndex, int groupRowNumber)
{
    cinder::ColorA averageColor;
    for (int i = startYIndex; i < startYIndex + groupRowNumber; i++)
    {
        for (int j = startXIndex; j < startXIndex + groupRowNumber; j++)
        {
            averageColor += this->m_currentImage.getPixel(cinder::Vec2i(j, i));
        }
    }
    float numberElement = cinder::math<float>::pow(static_cast<float>(groupRowNumber), 2);
    averageColor /= numberElement;
    return averageColor;
}


/**
 * Retourne toutes les touches clavier appuyé à l'instant T
 *
 * const std::vector<TouchEvent::Touch>& getActiveTouches() const;
 */

CINDER_APP_NATIVE(Genetic_AlgorithmApp, RendererGl)
