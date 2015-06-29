/*
-----------------------------------------------------------------------------
Filename:    engine.cpp
-----------------------------------------------------------------------------

*/

#include "engine.h"

#include <OgreException.h>
#include <OgreConfigFile.h>
#include <OgreViewport.h>


#include <iostream>

#include <boost/program_options.hpp>

//-------------------------------------------------------------------------------------
AWSim::Engine::Engine()
    : mRoot(nullptr),
      mResourcesCfg(Ogre::StringUtil::BLANK),
      mPluginsCfg(Ogre::StringUtil::BLANK),
      mWindow(nullptr),
      mSceneMgr(nullptr),
      mCamera(nullptr),
      mPlayerNode(nullptr),
      mInputManager(nullptr),
      mMouse(nullptr),
      mKeyboard(nullptr),
      mGuiRenderer(nullptr),
      mShutdown(false),
      mSceneDirector(nullptr),
      mPhysicsManager(nullptr),
      mGameObjectManager(nullptr),
      mLogger("awengine.log")
{
}
//-------------------------------------------------------------------------------------
AWSim::Engine::~Engine()
{
    //Remove ourself as a Window listener
    Ogre::WindowEventUtilities::removeWindowEventListener(mWindow, this);
    windowClosed(mWindow);

    //mPhysicsManager->destroyPhysicsWorld();
    delete mPhysicsManager;

    delete mSceneDirector;

    delete mRoot; // assume this cleans up entities and cameras?
}
//-------------------------------------------------------------------------------------
bool AWSim::Engine::parseOptions(int argc, char *argv[], AWSim::Engine &engine)
{
    // Create a local alias for brevity
    namespace po = boost::program_options;

    // Declare the supported options.
    po::options_description desc("Allowed options");
    desc.add_options()
        ("help", "produce help message")
        ("testserver", "run as a test server")
        ("testclient", "run as a test client")
    ;

    return true;
}
//-------------------------------------------------------------------------------------
bool AWSim::Engine::go()
{
#ifdef _DEBUG
    mResourcesCfg = "resources_d.cfg";
    mPluginsCfg = "plugins_d.cfg";
#else
    mResourcesCfg = "resources.cfg";
    mPluginsCfg = "plugins.cfg";
#endif

    // Add root object
    mRoot = new Ogre::Root(mPluginsCfg);

    Ogre::ConfigFile cf;
    cf.load(mResourcesCfg); // config file that points to resource locations

    Ogre::String secName, name, locType;

    // "Unpack" the config file
    Ogre::ConfigFile::SectionIterator secIt = cf.getSectionIterator(); // the config file is a "vector" of sections
    while (secIt.hasMoreElements())
    {
        secName = secIt.peekNextKey();
        Ogre::ConfigFile::SettingsMultiMap* settings = secIt.getNext(); // a section like [General]
        Ogre::ConfigFile::SettingsMultiMap::iterator it;
        for (it = settings->begin(); it != settings->end(); ++it)
        {
            locType = it->first; // as in "FileSystem" =
            name = it->second; // as in "path/to/resource/folder"
            Ogre::ResourceGroupManager::getSingleton().addResourceLocation(name, locType, secName);
        }
    }

    if(!(mRoot->restoreConfig() || mRoot->showConfigDialog()))
        return false;

    // Initialize root
    mWindow = mRoot->initialise(true, "TutorialApplication Render Window");

    Ogre::TextureManager::getSingleton().setDefaultNumMipmaps(5);
    Ogre::ResourceGroupManager::getSingleton().initialiseAllResourceGroups();

    // Initialize and add a scene manager
    mSceneMgr = mRoot->createSceneManager(Ogre::ST_GENERIC);
    mSceneMgr->setAmbientLight(Ogre::ColourValue(.5, .5, .5));
    mSceneMgr->setShadowTechnique(Ogre::SHADOWTYPE_TEXTURE_ADDITIVE);
    mSceneMgr->setShadowTextureSize(2048); // is there a way to set the physical rendering size of the shadow camera? make it smaller?

    // Initialize scene director
    mSceneDirector = new AWGraphics::SceneDirector(mRoot, mSceneMgr);

    // Camera
    mCamera = AWGraphics::createCamera(mSceneMgr, mWindow);

    // Initialize CEGUI
    mGuiRenderer = &CEGUI::OgreRenderer::bootstrapSystem();

    CEGUI::ImageManager::setImagesetDefaultResourceGroup("Imagesets");
    CEGUI::Font::setDefaultResourceGroup("Fonts");
    CEGUI::Scheme::setDefaultResourceGroup("Schemes");
    CEGUI::WidgetLookManager::setDefaultResourceGroup("LookNFeel");
    CEGUI::WindowManager::setDefaultResourceGroup("Layouts");

    CEGUI::SchemeManager::getSingleton().createFromFile("TaharezLook.scheme");
    //CEGUI::System::getSingleton().getDefaultGUIContext().getMouseCursor().setDefaultImage("TaharezLook/MouseArrow");
    //CEGUI::MouseCursor::show();

    CEGUI::WindowManager &wmgr = CEGUI::WindowManager::getSingleton();
    CEGUI::Window *sheet = wmgr.createWindow("DefaultWindow", "CEGUIDemo/Sheet");
    CEGUI::System::getSingleton().getDefaultGUIContext().setRootWindow(sheet);

    CEGUI::Window *quit = wmgr.createWindow("TaharezLook/Button", "CEGUIDemo/QuitButton");
    quit->setText("Quit");
    quit->setSize(CEGUI::USize(CEGUI::UDim(0.15, 0), CEGUI::UDim(0.05, 0)));
    sheet->addChild(quit);

    quit->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&AWSim::Engine::quit, this));

    // Initialize Open Input System
    Ogre::LogManager::getSingletonPtr()->logMessage("*** Initializing OIS ***");
    OIS::ParamList pl;
    size_t windowHnd = 0;
    std::ostringstream windowHndStr;

    mWindow->getCustomAttribute("WINDOW", &windowHnd);
    windowHndStr << windowHnd;
    pl.insert(std::make_pair(std::string("WINDOW"), windowHndStr.str()));

    // for showing the mouse?
#if defined OIS_WIN32_PLATFORM
    pl.insert(std::make_pair(std::string("w32_mouse"), std::string("DISCL_FOREGROUND" )));
    pl.insert(std::make_pair(std::string("w32_mouse"), std::string("DISCL_NONEXCLUSIVE")));
    pl.insert(std::make_pair(std::string("w32_keyboard"), std::string("DISCL_FOREGROUND")));
    pl.insert(std::make_pair(std::string("w32_keyboard"), std::string("DISCL_NONEXCLUSIVE")));
#elif defined OIS_LINUX_PLATFORM
    pl.insert(std::make_pair(std::string("x11_mouse_grab"), std::string("false")));
    pl.insert(std::make_pair(std::string("x11_mouse_hide"), std::string("false")));
    pl.insert(std::make_pair(std::string("x11_keyboard_grab"), std::string("false")));
    pl.insert(std::make_pair(std::string("XAutoRepeatOn"), std::string("true")));
#endif

    mInputManager = OIS::InputManager::createInputSystem( pl );

    mKeyboard = static_cast<OIS::Keyboard*>(mInputManager->createInputObject( OIS::OISKeyboard, true ));
    mMouse = static_cast<OIS::Mouse*>(mInputManager->createInputObject( OIS::OISMouse, true ));

    mMouse->setEventCallback(this);
    mKeyboard->setEventCallback(this);

    // Initialize physics
    mPhysicsManager = new AWPhysics::PhysicsManager;
    mPhysicsManager->initPhysicsWorld();

    // Initialize game objects manager;
    mGameObjectManager = new AWSystem::GameObjectManager(mSceneDirector, mPhysicsManager);
    mGameObjectManager->createPlane(0.0, 0.0, 0.0);
    mGameObjectManager->createBox(2, 2, 0.1);

    // Create scene
    mSceneDirector->createPlayer();
    mPlayerNode = mSceneDirector->getPlayerNode();

    // connect the inputhandler to the node:
    mInputHandler.attachToPlayerNode(mPlayerNode);
    mInputHandler.attachToCameraNode(mCamera->getCameraNode());

    // connect camera and player (?)
    mCamera->attachChaseCam(mPlayerNode);

    //Set initial mouse clipping size
    windowResized(mWindow);

    //Register as a Window listener
    Ogre::WindowEventUtilities::addWindowEventListener(mWindow, this);

    //Register this object as a Frame Listener
    mRoot->addFrameListener(this);

    // reset the timer
    mTimer.reset();

    //Start rendering
    mRoot->startRendering();

    return true;
}
//-------------------------------------------------------------------------------------
//Adjust mouse clipping area
void AWSim::Engine::windowResized(Ogre::RenderWindow* rw)
{
    unsigned int width, height, depth;
    int left, top;
    rw->getMetrics(width, height, depth, left, top);

    const OIS::MouseState &ms = mMouse->getMouseState();
    ms.width = width;
    ms.height = height;
}
//-------------------------------------------------------------------------------------
//Unattach OIS before window shutdown (very important under Linux)
void AWSim::Engine::windowClosed(Ogre::RenderWindow* rw)
{
    //Only close for window that created OIS (the main window)
    if(rw == mWindow)
    {
        if(mInputManager)
        {
            mInputManager->destroyInputObject( mMouse );
            mInputManager->destroyInputObject( mKeyboard );

            OIS::InputManager::destroyInputSystem(mInputManager);
            mInputManager = 0;
        }
    }
}
//-------------------------------------------------------------------------------------
//Capture input data
bool AWSim::Engine::frameRenderingQueued(const Ogre::FrameEvent& evt)
{
    if(mWindow->isClosed())
        return false;

    //Need to capture/update each device
    mKeyboard->capture();
    mMouse->capture();

    if(mKeyboard->isKeyDown(OIS::KC_ESCAPE))
        return false;

    if(mShutdown)
        return false;

    unsigned long timestep_microsec = mTimer.getMicroseconds();
    mTimer.reset();

    float timestep_seconds = (float)(timestep_microsec)*1e-6; // does this cause a Load-Hit-Store?

    //mLogger.reportStringStream(std::stringstream()<<"dt = " << timestep_seconds << "s");

    //std::cout << "timestep_seconds = " << timestep_seconds << "\n";

    //handle buffered input (i.e. "if key is down" type input)
    mInputHandler.handleBufferedInput(timestep_seconds);

    //step physics
    mPhysicsManager->updatePhysics(timestep_seconds);
    mGameObjectManager->updatePhysics();

    //update graphics
    mGameObjectManager->updateGraphics();
    // need to step physics, and update transforms, implement update graphics transf...
    // as well as add a static plane floor
    // something is wrong...

    //Need to inject timestamps to CEGUI System.
    CEGUI::System::getSingleton().injectTimePulse(evt.timeSinceLastFrame);

    //std::cout << "calling frameRendingQueued\n";

    return true;
}
//-------------------------------------------------------------------------------------
CEGUI::MouseButton convertButton(OIS::MouseButtonID buttonID) // convert OIS mouse buttons to CEGUI
{
    switch (buttonID)
    {
    case OIS::MB_Left:
        return CEGUI::LeftButton;

    case OIS::MB_Right:
        return CEGUI::RightButton;

    case OIS::MB_Middle:
        return CEGUI::MiddleButton;

    default:
        return CEGUI::LeftButton;
    }
}
//-------------------------------------------------------------------------------------
bool AWSim::Engine::keyPressed( const OIS::KeyEvent &arg )
{
    CEGUI::GUIContext& context = CEGUI::System::getSingleton().getDefaultGUIContext();
    context.injectKeyDown((CEGUI::Key::Scan)arg.key);
    context.injectChar((CEGUI::Key::Scan)arg.text);

    mInputHandler.keyPressed(arg);
    return true;
}
//-------------------------------------------------------------------------------------
bool AWSim::Engine::keyReleased( const OIS::KeyEvent &arg )
{
    CEGUI::System::getSingleton().getDefaultGUIContext().injectKeyUp((CEGUI::Key::Scan)arg.key);

    mInputHandler.keyReleased(arg);
    return true;
}
//-------------------------------------------------------------------------------------
bool AWSim::Engine::mouseMoved( const OIS::MouseEvent &arg )
{
    CEGUI::GUIContext &gui_context = CEGUI::System::getSingleton().getDefaultGUIContext();
    //gui_context.injectMouseMove(arg.state.X.rel, arg.state.Y.rel);
    gui_context.injectMousePosition(arg.state.X.abs, arg.state.Y.abs);

    // Scroll wheel.
    if (arg.state.Z.rel)
        gui_context.injectMouseWheelChange(arg.state.Z.rel / 120.0f);

    mInputHandler.mouseMoved(arg);
    return true;
}
//-------------------------------------------------------------------------------------
bool AWSim::Engine::mousePressed( const OIS::MouseEvent &arg, OIS::MouseButtonID id )
{
    CEGUI::System::getSingleton().getDefaultGUIContext().injectMouseButtonDown(convertButton(id));

    mInputHandler.mousePressed(arg, id);
    return true;
}
//-------------------------------------------------------------------------------------
bool AWSim::Engine::mouseReleased( const OIS::MouseEvent &arg, OIS::MouseButtonID id )
{
    CEGUI::System::getSingleton().getDefaultGUIContext().injectMouseButtonUp(convertButton(id));

    mInputHandler.mouseReleased(arg, id);
    return true;
}
//-------------------------------------------------------------------------------------
bool AWSim::Engine::quit(const CEGUI::EventArgs &e)
{
    mShutdown = true;

    return true;
}

//---------------------------------------------------------------------------

#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
#define WIN32_LEAN_AND_MEAN
#include "windows.h"
#endif

#ifdef __cplusplus
extern "C" {
#endif

#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
    INT WINAPI WinMain(HINSTANCE hInst, HINSTANCE, LPSTR strCmdLine, INT)
#else
    int main(int argc, char *argv[])
#endif
    {
        // Create application object
        AWSim::Engine app;

        try {
            if (AWSim::Engine::parseOptions(argc, argv, app))
            {
                app.go();
            }
        } catch(Ogre::Exception& e)  {
#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
            MessageBox(NULL, e.getFullDescription().c_str(), "An exception has occurred!", MB_OK | MB_ICONERROR | MB_TASKMODAL);
#else
            std::cerr << "An exception has occurred: " <<
                e.getFullDescription().c_str() << std::endl;
#endif
        }

        return 0;
    }

#ifdef __cplusplus
}
#endif

//---------------------------------------------------------------------------
