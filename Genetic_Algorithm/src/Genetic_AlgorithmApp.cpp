#include "Genetic_AlgorithmApp.h"

#include "Resources.h"

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

Genetic_AlgorithmApp::~Genetic_AlgorithmApp()
{
    m_threadRunning = false;

    if (m_thread.joinable())
        m_thread.join();

    if (m_threadHeight.joinable())
        m_threadHeight.join();
}

void Genetic_AlgorithmApp::prepareSettings(Settings* settings)
{
    m_beforeResizeWidth = 800;
    m_beforeResizeHeight = 600;
    settings->setWindowSize(cinder::Vec2i(800, 600));
    settings->setFrameRate(200.0f);

    settings->enableConsoleWindow();

    m_captureMode = false;
}

void Genetic_AlgorithmApp::setup()
{
    /* App */
    m_renderCurrentImage = false;
    /* === */
    /* Algo Gen */
	m_pixelPerSticky = 15;
	m_numberGapPixel = 0;
    m_isStarted = false;
    m_isPaused = false;
    /* === */

    /* Image Load*/
    m_currentImageLoadedIndex = 0;
    /* === */
    
    /* Camera capture */
    setupCapture();
    m_hasCaptureCamera = false;
    m_realTime = false;
    /* === */

    /* IHM */
    m_ihmParam = cinder::params::InterfaceGl::create("Sticky", cinder::Vec2i(245, 340));
    m_ihmParam->setPosition(cinder::Vec2i(20, 20));
    
    m_camParam = cinder::params::InterfaceGl::create("Camera", cinder::Vec2i(250, 150));
    m_camParam->setPosition(cinder::Vec2i(getWindowWidth() - 300, 20));

    m_ihmShader = cinder::params::InterfaceGl::create("Shader", cinder::Vec2i(150, 50));
    m_ihmShader->setPosition(cinder::Vec2i(getWindowWidth() - 200, 190));

    m_ihmStats = cinder::params::InterfaceGl::create("Stats", cinder::Vec2i(200, 100));
    m_ihmStats->setPosition(cinder::Vec2i(getWindowWidth() - 200, 260));
    m_ihmStats->hide();

    setupIHM();
    /* === */

    /* THREAD */
    m_threadRunning = false;
    m_computeFPS = 0.0;
    m_computeHeightFPS = 0.0;

    m_numberOfPopulation = 100;

    /* Camera */
    m_camera.setup(60.0f, getWindowAspectRatio(), 5.0f, 300000.0f, 500.f);

    /* shader */
    m_neighbors = 1;
    try
    {
        m_shader = gl::GlslProg::create(loadResource(SHADER_VERT), loadResource(SHADER_FRAG));
    }
    catch (gl::GlslProgCompileExc& e)
    {
        console() << e.what() << std::endl;
        quit();
    }
    catch (...)
    {
        console() << "Shader error" << std::endl;
        quit();
    }

    m_isBuilder = false;
    //console() << m_shader->getShaderLog(m_shader->getHandle()) << std::endl;
}

void Genetic_AlgorithmApp::setupIHM()
{
    m_ihmParam->clear();

    m_ihmParam->addButton("Toggle Mode", std::bind(&Genetic_AlgorithmApp::changeMode, this));
    m_ihmParam->addButton("Show/Hide Stats", std::bind(&Genetic_AlgorithmApp::toggleDisplayStat, this));
    m_ihmParam->addButton("Show/Hide Current Image", std::bind(&Genetic_AlgorithmApp::toggleDisplayCurrent, this));

    m_ihmParam->addSeparator("Input Options");

    if (m_captureMode)
    {
        if (!m_capture)
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
    m_ihmParam->addParam("Number of new Population", &m_numberOfPopulation, "min=0 step=1", m_isStarted && !m_isPaused ? true : false);
    m_ihmParam->addParam("Pixel par groupe", &m_pixelPerSticky, "min=1 max=1920 step=1", m_isStarted && !m_isPaused ? true : false);
    m_ihmParam->addParam("Espacement", &m_numberGapPixel, "min=0 max=10 step=1", m_isStarted && !m_isPaused ? true : false);

    m_ihmParam->addSeparator("Algo Gen Options");
    m_ihmParam->addParam("Save Best", &(m_algoGen.getInterval(ColorAlgoGen::COPY)), m_isStarted && !m_isPaused ? true : false).max(100.0f).min(0.0f).step(1.0f);
    m_ihmParam->addParam("Mutation Ratio", &(m_algoGen.getInterval(ColorAlgoGen::MUTATE)), m_isStarted && !m_isPaused ? true : false).max(100.0f).min(0.0f).step(1.0f);
    m_ihmParam->addParam("CrossOver Ratio", &(m_algoGen.getInterval(ColorAlgoGen::CROSSOVER)), m_isStarted && !m_isPaused ? true : false).max(100.0f).min(0.0f).step(1.0f);
    m_ihmParam->addParam("Random Ratio", &(m_algoGen.getInterval(ColorAlgoGen::RANDOM)), m_isStarted && !m_isPaused ? true : false).max(100.0f).min(0.0f).step(1.0f);

    m_ihmParam->addSeparator("Algo controle");
    m_ihmParam->addButton("Start", std::bind(&Genetic_AlgorithmApp::start, this));
    m_ihmParam->addButton("Pause", std::bind(&Genetic_AlgorithmApp::pause, this));
    m_ihmParam->addButton("Stop", std::bind(&Genetic_AlgorithmApp::stop, this));

    /* Camera ihm */
    m_camParam->clear();
    m_camParam->addText("Camera Param");
    m_camParam->addParam("Distance", &m_camera.getCameraDistance(), "min=20 max=300000 step=10");
    m_camParam->addParam("Rotation", &m_camera.getRotation());

    /* Shader ihm */
    m_ihmShader->clear();
    m_ihmShader->addParam("Shader Neighbors", &m_neighbors, "min=1 max=100 step=1");
}

void Genetic_AlgorithmApp::update()
{
    if (m_captureMode)
    {
        if (m_capture && m_capture->checkNewFrame())
        {
            m_captureImage = m_capture->getSurface();

            if (m_realTime)
                m_currentImage = m_captureImage;
        }
    }

    m_camera.update();
}

/*
    luminance = (r * 0.3) + (g * 0.59) + (b * 0.11)
*/

#if 0
void Genetic_AlgorithmApp::draw()
{
    gl::clear(cinder::Color::black());

    auto screen = getWindowBounds();

    if (m_algoGenResult)
    {
        m_mutex.lock();
        {
            gl::draw(gl::Texture(m_algoGenResult), ci::Rectf(-(0.5f * m_currentImage.getWidth()), 0.5f * m_currentImage.getHeight(), 0.5f * m_currentImage.getWidth(), -(0.5f * m_currentImage.getHeight())));
        }
        m_mutex.unlock();
    }    

    if (m_renderCurrentImage && m_currentImage)
    {
        int32_t x1 = static_cast<int32_t>(- (0.5f * m_currentImage.getWidth()));
        int32_t y1 = static_cast<int32_t>(0.5f * m_currentImage.getHeight());

        int32_t x2 = static_cast<int32_t>((0.5f * m_currentImage.getWidth()) * 0.1f);
        int32_t y2 = static_cast<int32_t>(-((0.5f * m_currentImage.getHeight()) * 0.1f));

        x2 = static_cast<int32_t>(x1 + (0.1f * (x2 - x1)));
        y2 = static_cast<int32_t>(y1 + (0.1f * (y2 - y1)));
        
        gl::draw(gl::Texture(m_currentImage), ci::Rectf(x1, y1, x2, y2 ));
    }

    updateIHM();
}
#endif

void Genetic_AlgorithmApp::draw()
{
    gl::clear(cinder::Color::black());

    m_camera.update();

    gl::enableDepthRead();
    gl::enableDepthWrite();

    if (m_isStarted && m_algoGenResult && m_heightMap)
    {
        auto texture = gl::Texture(m_algoGenResult);
        auto heightMap = gl::Texture(m_heightMap);

        m_shader->bind();

        m_shader->uniform("u_texture", 0);
        m_shader->uniform("u_heightMap", 1);
        m_shader->uniform("u_time", static_cast<float>(0.00025 * m_timerShader.getSeconds()));
        m_shader->uniform("u_neighbors", m_neighbors);

        texture.bind(0);
        heightMap.bind(1);
        
        gl::color(Color::white());

        gl::draw(m_planeResultAlgoGen);

        texture.unbind();
        heightMap.unbind();
        m_shader->unbind();
    }

    if (m_renderCurrentImage && m_currentImage)
    {
        auto screen = getWindowBounds();
        auto demiW = 0.5f * screen.getWidth();
        auto demiH = 0.5f * screen.getHeight();

        float widthTenPercent = 0.1f * demiW;
        float heightTenPercent = 0.1f * demiH;
        
        ci::Rectf rectDraw(demiW - widthTenPercent, demiH - heightTenPercent, demiW, demiH);

        gl::draw(gl::Texture(m_currentImage), rectDraw);
    }

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
            m_ihmStats->addText("Compute Height FPS : " + std::to_string(m_computeHeightFPS));
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

    m_ihmParam->draw();
}

void Genetic_AlgorithmApp::resize()
{
    ci::Rectf screen = getWindowBounds();
 
    m_beforeResizeWidth = screen.getWidth();
    m_beforeResizeHeight = screen.getHeight();

    m_camParam->setPosition(cinder::Vec2i(getWindowWidth() - 300, 20));
    m_ihmShader->setPosition(cinder::Vec2i(getWindowWidth() - 200, 190));
    m_ihmStats->setPosition(cinder::Vec2i(getWindowWidth() - 200, 260));
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
    else
    {
        if (m_captureMode)
        {
            if (event.getCode() == KeyEvent::KEY_c)
            {
                if (m_captureMode)
                {
                    m_videoCapture.copyFrom(m_captureImage, m_captureImage.getBounds());

                    m_hasCaptureCamera = true;

                    m_currentImage = m_videoCapture;
                }
            }
        }
        else
        {
            if (event.getCode() == KeyEvent::KEY_n)
            {
                if (!m_captureMode && m_imageLoaded.size() > 0)
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
    if (m_isStarted && !m_isPaused)
    {
        if (event.isLeftDown())
        {
            m_isBuilder = true;
        }
    }
}

void Genetic_AlgorithmApp::mouseUp(MouseEvent event)
{
    if (event.isLeft())
    {
        m_isBuilder = false;
    }
}

void Genetic_AlgorithmApp::mouseWheel(MouseEvent event)
{

}

void Genetic_AlgorithmApp::mouseMove(MouseEvent event)
{
}

void Genetic_AlgorithmApp::mouseDrag(MouseEvent event)
{
#if 0
    if (m_isBuilder)
    {
        float u = static_cast<float>(event.getX()) / static_cast<float>(getWindowWidth());
        float v = static_cast<float>(event.getY()) / static_cast<float>(getWindowHeight());

        auto ray = m_camera.getCamera().generateRay(u, v, static_cast<float>(m_algoGenResult.getWidth()) / static_cast<float>(m_algoGenResult.getHeight()));

        float result;

        if (ray.calcPlaneIntersection(ci::Vec3f::zero(), ci::Vec3f::zAxis(), &result))
        {
            auto intersect = ray.calcPosition(result);

            int demiW = m_algoGenResult.getWidth() / 2;
            int demiH = m_algoGenResult.getHeight() / 2;

            if (intersect.x <= demiW && intersect.x >= -demiW &&
                intersect.y <= demiH && intersect.y >= -demiH)
            {
                auto heightPos = ci::Vec2i(static_cast<int>(intersect.x + demiW), static_cast<int>(-intersect.y + demiH));
                auto pixel = m_heightMap.getPixel(heightPos);

                /*pixel.r = tools::clamp(0.0f, 255.0f, pixel.r + 2.0f);
                pixel.g = tools::clamp(0.0f, 255.0f, pixel.g + 1.0f);
                pixel.b = tools::clamp(0.0f, 255.0f, pixel.b + 3.0f);*/

                pixel.r = tools::clamp(0.0f, std::numeric_limits<float>::max(), pixel.r + 2.0f);
                pixel.g = tools::clamp(0.0f, std::numeric_limits<float>::max(), pixel.g + 1.0f);
                pixel.b = tools::clamp(0.0f, std::numeric_limits<float>::max(), pixel.b + 3.0f);

                m_heightMap.setPixel(heightPos, pixel);
            }
        }
    }
#endif
}

void Genetic_AlgorithmApp::fileDrop(FileDropEvent event)
{
    if (m_captureMode)
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

void Genetic_AlgorithmApp::setupCapture()
{
    auto deviceList = cinder::Capture::getDevices();

    if (deviceList.size() > 0)
    {
        auto device = deviceList.front();
        
        if (device->checkAvailable())
        {
            m_capture = cinder::Capture::create(1920, 1080, device);

            m_videoCapture = Surface(m_capture->getSurface());

            m_capture->start();

            m_captureMode = true;
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
    if (m_isStarted && !m_isPaused)
        return;

    m_captureMode = !m_captureMode;

    if (m_captureMode)
    {
        if (m_realTime)
            m_currentImage = m_captureImage;
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
            console() << "Format non supporté pour l'image : " << p.filename() << std::endl;
        }
        catch (std::exception&)
        {
            console() << "Error occur" << std::endl;
        }
    }

    setupIHM();
}

static void genPlaneResult(unsigned int width, unsigned int height, ci::TriMesh& mesh)
{
    mesh.clear();

    float demiW = static_cast<float>(width / 2);
    float demiH = static_cast<float>(height / 2);

    for (float j = -demiH; j < demiH; ++j)
    {
        for (float i = -demiW; i < demiW; ++i)
        {
            mesh.appendVertex(ci::Vec3f(i, j, 0.0f));
            
            mesh.appendTexCoord(ci::Vec2f(
                (i + demiW) / width,
                (j + demiH) / height
                ));
        }
    }

    //For create face algorithme
    size_t w = width - 1;
    size_t h = height - 1;

    for (size_t j = 0; j < h; ++j)
    {
        for (size_t i = 0; i < w; ++i)
        {
            mesh.appendTriangle(width * j + i, width * (j + 1) + i, width * j + i + 1);
            mesh.appendTriangle(width * j + i + 1, width * (j + 1) + i, width * (j + 1) + i + 1);
        }
    }
}

void Genetic_AlgorithmApp::start()
{
    if (m_isStarted || m_pixelPerSticky <= m_numberGapPixel || !m_currentImage)
        return;

    ci::Rectf screen = getWindowBounds();

    int width = m_currentImage.getWidth();
    int height = m_currentImage.getHeight();

    m_currentAlgoGenImage = ci::ip::resizeCopy(m_currentImage, m_currentImage.getBounds(), ci::Vec2i(width / m_pixelPerSticky, height / m_pixelPerSticky));

    m_algoGen.setPopSize(m_numberOfPopulation);
    m_algoGen.setup(m_currentAlgoGenImage.getWidth(), m_currentAlgoGenImage.getHeight());

    m_algoGenHeight.setPopSize(m_numberOfPopulation);
    m_algoGenHeight.getInterval(ColorAlgoGen::COPY) = m_algoGen.getInterval(ColorAlgoGen::COPY);
    m_algoGenHeight.getInterval(ColorAlgoGen::MUTATE) = m_algoGen.getInterval(ColorAlgoGen::MUTATE);
    m_algoGenHeight.getInterval(ColorAlgoGen::CROSSOVER) = m_algoGen.getInterval(ColorAlgoGen::CROSSOVER);
    m_algoGenHeight.getInterval(ColorAlgoGen::RANDOM) = m_algoGen.getInterval(ColorAlgoGen::RANDOM);

    m_algoGenHeight.setup(m_currentAlgoGenImage.getWidth(), m_currentAlgoGenImage.getHeight());

    genPlaneResult(m_currentAlgoGenImage.getWidth(), m_currentAlgoGenImage.getHeight(), m_planeResultAlgoGen);

    m_timerShader.start();

    m_isStarted = true;
    m_isPaused = false;
    m_isBuilder = false;

    if (m_thread.joinable())
        m_thread.join();

    if (m_threadHeight.joinable())
        m_threadHeight.join();

    m_threadRunning = true;

    m_thread = std::thread(std::bind(&Genetic_AlgorithmApp::threadingCompute, this));
    m_threadHeight = std::thread(std::bind(&Genetic_AlgorithmApp::threadingComputeHeight, this));

    int32_t distance = std::max(m_currentAlgoGenImage.getWidth(), m_currentAlgoGenImage.getHeight());

    m_camera.getCameraDistance() = static_cast<float>(distance /*+ distance * 1.1f*/);
    m_camera.update();

    setupIHM();
}


void Genetic_AlgorithmApp::pause()
{
    if (!m_isStarted)
        return;

    m_algoGenHeight.getInterval(ColorAlgoGen::COPY) = m_algoGen.getInterval(ColorAlgoGen::COPY);
    m_algoGenHeight.getInterval(ColorAlgoGen::MUTATE) = m_algoGen.getInterval(ColorAlgoGen::MUTATE);
    m_algoGenHeight.getInterval(ColorAlgoGen::CROSSOVER) = m_algoGen.getInterval(ColorAlgoGen::CROSSOVER);
    m_algoGenHeight.getInterval(ColorAlgoGen::RANDOM) = m_algoGen.getInterval(ColorAlgoGen::RANDOM);

    m_isPaused = !m_isPaused;

    setupIHM();
}

void Genetic_AlgorithmApp::stop()
{
    if (!m_isStarted)
        return;

    m_threadRunning = false;

    m_algoGenResult = ci::Surface();

    m_isStarted = false;
    m_isPaused = false;

    setupIHM();
}

void Genetic_AlgorithmApp::nextStep()
{
    ci::Surface tmpImage = m_algoGen(m_currentAlgoGenImage);
    
    m_mutex.lock();
    {
        m_algoGenResult = tmpImage;
    }
    m_mutex.unlock();
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
            //timer.stop();
            m_computeFPS = 1.0 / timer.getSeconds();
        }
    }
}

void Genetic_AlgorithmApp::threadingComputeHeight()
{
    cinder::Timer timer;

    while (m_threadRunning)
    {
        if (m_isStarted && !m_isPaused)
        {
            timer.start();
            
            ci::Surface tmpImage = m_algoGenHeight(m_currentAlgoGenImage);

            m_mutex.lock();
            {
                m_heightMap = tmpImage;
            }
            m_mutex.unlock();

            m_computeHeightFPS = 1.0 / timer.getSeconds();
        }
    }
}

CINDER_APP_NATIVE(Genetic_AlgorithmApp, RendererGl)
