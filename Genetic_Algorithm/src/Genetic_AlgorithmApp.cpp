#include "Genetic_AlgorithmApp.h"

/**
 * TODO
 *
 * - FullScreen
 * - Algo Gen
 * - 2 Modes :
 *      - Fen�tr� : Chargement des images, set des diff�rents params
 *      - FullSceen : D�roulement de l'algo, (Changement de l'image proc�c�, utilisation de la cam en temps r�el, pris d'un snapshoot � l'aide de la cam, load d'une image par une URL, ...)
 * - Suite
 */

Genetic_AlgorithmApp::~Genetic_AlgorithmApp()
{
    m_threadRunning = false;

    if (m_thread.joinable())
        m_thread.join();
}

void Genetic_AlgorithmApp::prepareSettings(Settings* settings)
{
    this->beforeResizeWidth = 800;
    this->beforeResizeHeight = 600;
    settings->setWindowSize(cinder::Vec2i(800, 600));
    settings->setFrameRate(200.0f);

    m_cameraMode = false;
}

void Genetic_AlgorithmApp::setup()
{
    /* App */
    m_renderCurrentImage = true;
    /* === */
    /* Algo Gen */
	this->m_pixelGroupNumber = 15;
	this->m_numberGapPixel = 0;
    m_isStarted = false;
    m_isPaused = false;
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
    m_ihmParam = cinder::params::InterfaceGl::create("Sticky", cinder::Vec2i(245, 340));
    m_ihmStats = cinder::params::InterfaceGl::create("Stats", cinder::Vec2i(150, 100));
    m_ihmStats->setPosition(cinder::Vec2i(getWindowWidth() - 150, 20));
    m_ihmStats->hide();

    setupIHM();

    m_renderString = cinder::gl::TextureFont::create(cinder::Font("Calibri", 25));
    /* === */

    /* THREAD */
    m_threadRunning = false;
    m_computeFPS = 0.0;

    m_numbreOfNewPop = 5;
}

void Genetic_AlgorithmApp::setupIHM()
{
    m_ihmParam->clear();

    m_ihmParam->addButton("Toggle Mode", std::bind(&Genetic_AlgorithmApp::changeMode, this));
    m_ihmParam->addButton("Show/Hide Stats", std::bind(&Genetic_AlgorithmApp::toggleDisplayStat, this));
    m_ihmParam->addButton("Show/Hide Current Image", std::bind(&Genetic_AlgorithmApp::toggleDisplayCurrent, this));

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
        m_ihmParam->addButton("Load image", std::bind(&Genetic_AlgorithmApp::loadImage, this));

        std::string message;

        if (m_imageLoaded.size() > 0)
        {
            message += std::to_string(m_imageLoaded.size());
            message += " images load";
            m_ihmParam->addText(message.data());

            message = "Current image : ";
            message += std::to_string(m_currentImageLoadedIndex);

            m_ihmParam->addText(message.data());
        }
        else
        {
            message = "No image load";
            m_ihmParam->addText(message.data());
        }
    }

    m_ihmParam->addSeparator("Apps options");
    m_ihmParam->addParam("Number of new Population", &m_numbreOfNewPop, "min=0 step=1", m_isStarted && !m_isPaused ? true : false);
    m_ihmParam->addParam("Pixel par groupe", &m_pixelGroupNumber, "min=1 max=1920 step=1", m_isStarted && !m_isPaused ? true : false);
    m_ihmParam->addParam("Espacement", &m_numberGapPixel, "min=0 max=10 step=1", m_isStarted && !m_isPaused ? true : false);

    m_ihmParam->addSeparator("Algo Gen Options");
    m_ihmParam->addParam("Save Best", &(m_algoGen.getInterval(ColorAlgoGen::COPY)), m_isStarted && !m_isPaused ? true : false).max(100.0f).min(0.0f).step(1.0f);
    m_ihmParam->addParam("Mutation Ratio", &(m_algoGen.getInterval(ColorAlgoGen::MUTATE)), m_isStarted && !m_isPaused ? true : false).max(100.0f).min(0.0f).step(1.0f);
    m_ihmParam->addParam("Combinaison Ratio", &(m_algoGen.getInterval(ColorAlgoGen::COMBINAISON)), m_isStarted && !m_isPaused ? true : false).max(100.0f).min(0.0f).step(1.0f);
    m_ihmParam->addParam("Random Ratio", &(m_algoGen.getInterval(ColorAlgoGen::RANDOM)), m_isStarted && !m_isPaused ? true : false).max(100.0f).min(0.0f).step(1.0f);

    m_ihmParam->addSeparator("Algo controle");
    m_ihmParam->addButton("Start", std::bind(&Genetic_AlgorithmApp::start, this));
    m_ihmParam->addButton("Pause", std::bind(&Genetic_AlgorithmApp::pause, this));
    m_ihmParam->addButton("Stop", std::bind(&Genetic_AlgorithmApp::stop, this));
    
    if (m_isPaused)
        m_ihmParam->addButton("NextStep", std::bind(&Genetic_AlgorithmApp::nextStep, this));
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

    /*if (m_isStarted && !m_isPaused)
    {
        nextStep();
    }*/
}

void Genetic_AlgorithmApp::draw()
{
    gl::clear(cinder::Color::black());

    m_mutex.lock();

    for (std::vector<Sticky>::size_type i = 0; i < this->m_StickyArmy.size(); i++)
	{
		this->m_StickyArmy[i].sticky.draw();
	}
    
    m_mutex.unlock();

    if (m_renderCurrentImage && m_currentImage)
        gl::draw(gl::Texture(m_currentImage), cinder::Rectf(0.0f, 0.0f, 0.1f * getWindowWidth(), 0.1f * getWindowHeight()));

    updateIHM();
}

void Genetic_AlgorithmApp::updateIHM()
{
    if (m_ihmStats->isVisible())
    {
        m_ihmStats->clear();
        
        m_ihmStats->addText(std::string("FPS :") + std::to_string(getAverageFps()));

        if (m_isStarted && !m_isPaused)
        {
            m_ihmStats->addText("Start");
            m_ihmStats->addText("Algo FPS : " + std::to_string(m_computeFPS));
            m_ihmStats->addText("Pop size : " + std::to_string(m_StickyArmy.size()));
            m_ihmStats->addText(std::string("Current image width" + m_currentImage.getWidth()));
            m_ihmStats->addText(std::string("Current image height" + m_currentImage.getHeight()));

        }
        else if (m_isPaused)
        {
            m_ihmStats->addText("Pause");
        }
        else
        {
            m_ihmStats->addText("Stop");
        }
    }

    //m_ihmStats->draw();
    m_ihmParam->draw();
}

void Genetic_AlgorithmApp::resize()
{
    ci::Rectf screen = getWindowBounds();
    float wRatio = screen.getWidth() / this->beforeResizeWidth;
    float hRatio = screen.getHeight() / this->beforeResizeHeight;

    m_mutex.lock();
    for (std::vector<Sticky>::size_type i = 0; i < this->m_StickyArmy.size(); i++)
    {
        this->m_StickyArmy[i].sticky.updateSize(wRatio, hRatio);
    }
    m_mutex.unlock();

    this->beforeResizeWidth = screen.getWidth();
    this->beforeResizeHeight = screen.getHeight();

    m_ihmStats->setPosition(cinder::Vec2i(static_cast<int>(screen.getWidth()) - 200, 20));
    
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
        m_renderCurrentImage = !m_renderCurrentImage;
    }
    else if (event.getCode() == KeyEvent::KEY_s)
    {
        this->start();
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
                loadImage();
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
    if (m_cameraMode)
        return;

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
                m_imageLoaded.push_back(cinder::loadImage(path));
            }
            catch (ci::ImageIoException&)
            {
                console() << "Format non support� pour l'image : " << path.filename() << std::endl;
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

void Genetic_AlgorithmApp::toggleDisplayStat()
{
    if (m_ihmStats->isVisible())
        m_ihmStats->hide();
    else
        m_ihmStats->show();
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

void Genetic_AlgorithmApp::toggleDisplayCurrent()
{
    m_renderCurrentImage = !m_renderCurrentImage;
}

void Genetic_AlgorithmApp::loadImage()
{
    fs::path p = getOpenFilePath("", ImageIo::getLoadExtensions());

    if (!p.empty())
    {
        m_imageLoaded.clear();

        try
        {
            m_imageLoaded.push_back(cinder::loadImage(p));
            m_currentImage = m_imageLoaded.front();
            m_currentImageLoadedIndex = 0;
        }
        catch (ci::ImageIoException&)
        {
            console() << "Format non support� pour l'image : " << p.filename() << std::endl;
        }
        catch (std::exception&)
        {
            console() << "Error occur" << std::endl;
        }
    }

    setupIHM();
}

void Genetic_AlgorithmApp::start()
{
    if (m_isStarted || m_pixelGroupNumber <= m_numberGapPixel || !m_currentImage)
        return;

    ci::Rectf screen = getWindowBounds();
    /*
    int width = static_cast<int>(screen.getWidth());
    int height = static_cast<int>(screen.getHeight());
    */
    float widthRatio = screen.getWidth() / this->m_currentImage.getWidth();
    float heigthRatio = screen.getHeight() / this->m_currentImage.getHeight();

    int width = static_cast<int>(this->m_currentImage.getWidth());
    int height = static_cast<int>(this->m_currentImage.getHeight());

    float pixelGroupNumber = static_cast<float>(m_pixelGroupNumber);
    float numberGapPixel = static_cast<float>(m_numberGapPixel);

    this->m_StickyArmy.clear();
    this->m_StickyArmy.resize((height / this->m_pixelGroupNumber) * (width / this->m_pixelGroupNumber));
    
    Stixel currentStix;

    m_mutex.lock();
	for (float i = 0; i < height / this->m_pixelGroupNumber; i++)
	{
        for (float j = 0; j < width / this->m_pixelGroupNumber; j++)
        {
            currentStix.sticky = Sticky(pixelGroupNumber * widthRatio - numberGapPixel* widthRatio, pixelGroupNumber* heigthRatio - numberGapPixel* heigthRatio,
                (j + 1.f) * (pixelGroupNumber * widthRatio), (i + 1.f) * (pixelGroupNumber * heigthRatio),
                //this->getAveragePixelColor(j  * this->m_pixelGroupNumber, i * this->m_pixelGroupNumber, this->m_pixelGroupNumber));
                cinder::ColorA(RANDOMIZER.nextFloat(0.0f, 1.0f), RANDOMIZER.nextFloat(0.0f, 1.0f), RANDOMIZER.nextFloat(0.0f, 1.0f), 1.0f));

            currentStix.pixel = Pixel((j + 1.f) * pixelGroupNumber, (i + 1.f) * pixelGroupNumber, this->getAveragePixelColor(j * this->m_pixelGroupNumber, i * this->m_pixelGroupNumber, this->m_pixelGroupNumber));
            this->m_StickyArmy[j + i * (width / this->m_pixelGroupNumber)] = currentStix;
		}
	}
    m_mutex.unlock();

    m_isStarted = true;
    m_isPaused = false;

    if (m_thread.joinable())
        m_thread.join();

    m_threadRunning = true;

    m_thread = std::thread(std::bind(&Genetic_AlgorithmApp::threadingCompute, this));

    setupIHM();
}

void Genetic_AlgorithmApp::pause()
{
    if (!m_isStarted)
        return;

    m_isPaused = !m_isPaused;

    setupIHM();
}

void Genetic_AlgorithmApp::stop()
{
    if (!m_isStarted)
        return;

    m_mutex.lock();
    this->m_StickyArmy.clear();
    m_mutex.unlock();

    m_threadRunning = false;

    m_isStarted = false;
    m_isPaused = false;

    setupIHM();
}

void Genetic_AlgorithmApp::nextStep()
{
    std::vector<IAlgoGen::StixelsWrapper> newPop;
    newPop.reserve(m_numbreOfNewPop);

    m_mutex.lock();
    auto sitckyArmyCopie(m_StickyArmy);
    m_mutex.unlock();

    for (unsigned int i = 0; i < m_numbreOfNewPop; ++i)
        newPop.push_back(m_algoGen(sitckyArmyCopie));

    if (newPop.size() > 0)
    {
        std::sort(newPop.begin(), newPop.end());

        m_mutex.lock();
        if (m_isStarted && !m_isPaused)
            m_StickyArmy = newPop.front().stixel;
        m_mutex.unlock();
    }
}

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

void Genetic_AlgorithmApp::threadingCompute()
{
    cinder::Timer timer;

    while (m_threadRunning)
    {
        if (m_isStarted && !m_isPaused)
        {
            timer.start();
            nextStep();
            timer.stop();
            m_computeFPS = 1.0 / timer.getSeconds();
        }
    }
}

CINDER_APP_NATIVE(Genetic_AlgorithmApp, RendererGl)
