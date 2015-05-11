/*
-----------------------------------------------------------------------------
Filename:    engine.cpp
-----------------------------------------------------------------------------

*/

#include "engine.h"

#include <OgreException.h>
#include <OgreConfigFile.h>
#include <OgreViewport.h>
#include <OgreEntity.h>

#include <iostream>


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
      mRenderer(nullptr),
      mShutdown(false)

{
}
//-------------------------------------------------------------------------------------
AWSim::Engine::~Engine()
{
    //Remove ourself as a Window listener
    Ogre::WindowEventUtilities::removeWindowEventListener(mWindow, this);
    windowClosed(mWindow);
    delete mRoot; // assume this cleans up entities and cameras?
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

//    // Initialize and add a camera
//    mCamera = mSceneMgr->createCamera("MainCam");

//    mCamera->setPosition(0, 0, 80);
//    mCamera->lookAt(0, 0, -300);
//    mCamera->setNearClipDistance(5);

//    // Initialize and add a viewport
//    Ogre::Viewport* vp = mWindow->addViewport(mCamera);

//    vp->setBackgroundColour(Ogre::ColourValue(0,0,0));

//    mCamera->setAspectRatio(
//        Ogre::Real(vp->getActualWidth()) /
//        Ogre::Real(vp->getActualHeight()));

    mCamera = AWGraphics::createCamera(mSceneMgr, mWindow);

    // Initialize CEGUI
    mRenderer = &CEGUI::OgreRenderer::bootstrapSystem();

    CEGUI::ImageManager::setImagesetDefaultResourceGroup("Imagesets");
    CEGUI::Font::setDefaultResourceGroup("Fonts");
    CEGUI::Scheme::setDefaultResourceGroup("Schemes");
    CEGUI::WidgetLookManager::setDefaultResourceGroup("LookNFeel");
    CEGUI::WindowManager::setDefaultResourceGroup("Layouts");

    CEGUI::SchemeManager::getSingleton().createFromFile("TaharezLook.scheme");
    CEGUI::System::getSingleton().getDefaultGUIContext().getMouseCursor().setDefaultImage("TaharezLook/MouseArrow");


    CEGUI::WindowManager &wmgr = CEGUI::WindowManager::getSingleton();
    CEGUI::Window *sheet = wmgr.createWindow("DefaultWindow", "CEGUIDemo/Sheet");
    CEGUI::System::getSingleton().getDefaultGUIContext().setRootWindow(sheet);

    CEGUI::Window *quit = wmgr.createWindow("TaharezLook/Button", "CEGUIDemo/QuitButton");
    quit->setText("Quit");
    quit->setSize(CEGUI::USize(CEGUI::UDim(0.15, 0), CEGUI::UDim(0.05, 0)));
    sheet->addChild(quit);

    quit->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&AWSim::Engine::quit, this));

    // Initialize and add an entity
    Ogre::Entity* ogreEntity = mSceneMgr->createEntity("ogrehead.mesh");

    Ogre::SceneNode* ogreNode = mSceneMgr->getRootSceneNode()->createChildSceneNode();
    ogreNode->attachObject(ogreEntity);

    mPlayerNode = ogreNode;

    // Initialize and add a light
    mSceneMgr->setAmbientLight(Ogre::ColourValue(.5, .5, .5));

    Ogre::Light* light = mSceneMgr->createLight("MainLight");
    light->setPosition(20, 80, 50);

    // Initialize Open Input System
    Ogre::LogManager::getSingletonPtr()->logMessage("*** Initializing OIS ***");
    OIS::ParamList pl;
    size_t windowHnd = 0;
    std::ostringstream windowHndStr;

    mWindow->getCustomAttribute("WINDOW", &windowHnd);
    windowHndStr << windowHnd;
    pl.insert(std::make_pair(std::string("WINDOW"), windowHndStr.str()));

    mInputManager = OIS::InputManager::createInputSystem( pl );

    mKeyboard = static_cast<OIS::Keyboard*>(mInputManager->createInputObject( OIS::OISKeyboard, true ));
    mMouse = static_cast<OIS::Mouse*>(mInputManager->createInputObject( OIS::OISMouse, true ));

    mMouse->setEventCallback(this);
    mKeyboard->setEventCallback(this);

    //Set initial mouse clipping size
    windowResized(mWindow);

    //Register as a Window listener
    Ogre::WindowEventUtilities::addWindowEventListener(mWindow, this);

    //Register this object as a Frame Listener
    mRoot->addFrameListener(this);

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
    return true;
}
//-------------------------------------------------------------------------------------
bool AWSim::Engine::keyReleased( const OIS::KeyEvent &arg )
{
    CEGUI::System::getSingleton().getDefaultGUIContext().injectKeyUp((CEGUI::Key::Scan)arg.key);
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
    return true;
}
//-------------------------------------------------------------------------------------
bool AWSim::Engine::mousePressed( const OIS::MouseEvent &arg, OIS::MouseButtonID id )
{
    CEGUI::System::getSingleton().getDefaultGUIContext().injectMouseButtonDown(convertButton(id));
    return true;
}
//-------------------------------------------------------------------------------------
bool AWSim::Engine::mouseReleased( const OIS::MouseEvent &arg, OIS::MouseButtonID id )
{
    CEGUI::System::getSingleton().getDefaultGUIContext().injectMouseButtonUp(convertButton(id));
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
            app.go();
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
