/*
-----------------------------------------------------------------------------
Filename:    engine.h
-----------------------------------------------------------------------------

Main engine class

-----------------------------------------------------------------------------
*/

#ifndef TUTORIALAPPLICATION_H
#define TUTORIALAPPLICATION_H

#include <OgreRoot.h>
#include <OgreCamera.h>
#include <OgreSceneManager.h>
#include <OgreRenderWindow.h>
#include <OgreWindowEventUtilities.h>
#include <OgreTimer.h>

#include <OISEvents.h>
#include <OISInputManager.h>
#include <OISKeyboard.h>
#include <OISMouse.h>

#include <CEGUI/CEGUI.h>
#include <CEGUI/RendererModules/Ogre/Renderer.h>

#include "awgraphics/camera.h"
#include "awgraphics/scenedirector.h"
#include "awinput/inputhandler.h"
#include "awphysics/physicsmanager.h"
#include "awsystem/gameobjectmanager.h"
#include "awutility/logger.h"

//---------------------------------------------------------------------------
namespace AWSim {

    class Engine : public Ogre::WindowEventListener, public Ogre::FrameListener,
                   public OIS::KeyListener, public OIS::MouseListener
    {
    public:
        Engine();
        virtual ~Engine();


        static bool parseOptions(int argc, char *argv[], Engine &engine);
        bool go();
    protected:
        // Ogre::WindowEventListener
        virtual void windowResized(Ogre::RenderWindow* rw);
        virtual void windowClosed(Ogre::RenderWindow* rw);

        // Ogre::FrameListener
        virtual bool frameRenderingQueued(const Ogre::FrameEvent& evt);

        // OIS::KeyListener
        virtual bool keyPressed( const OIS::KeyEvent &arg );
        virtual bool keyReleased( const OIS::KeyEvent &arg );

        // OIS::MouseListener
        virtual bool mouseMoved( const OIS::MouseEvent &arg );
        virtual bool mousePressed( const OIS::MouseEvent &arg, OIS::MouseButtonID id );
        virtual bool mouseReleased( const OIS::MouseEvent &arg, OIS::MouseButtonID id );

        bool quit(const CEGUI::EventArgs &e);

    private:
        Ogre::Root* mRoot;
        Ogre::String mResourcesCfg;
        Ogre::String mPluginsCfg;
        Ogre::RenderWindow* mWindow;
        Ogre::SceneManager* mSceneMgr;
        Ogre::SceneNode* mPlayerNode;
        Ogre::Timer mTimer;

        // Graphics
        AWGraphics::Camera* mCamera;
        AWGraphics::SceneDirector* mSceneDirector;

        // Input
        AWInput::InputHandler mInputHandler; // why would anyone use a pointer here? For late initialization?

        // Physics
        AWPhysics::PhysicsManager *mPhysicsManager;

        // System (overall coordination between subsystems)
        AWSystem::GameObjectManager *mGameObjectManager;

        // OIS members
        OIS::InputManager* mInputManager;
        OIS::Mouse* mMouse;
        OIS::Keyboard* mKeyboard;

        // CEGUI members
        CEGUI::OgreRenderer* mGuiRenderer;
        bool mShutdown;

        // Debug
        AWUtil::Logger mLogger;
    };
}

//---------------------------------------------------------------------------

#endif // #ifndef TUTORIALAPPLICATION_H

//---------------------------------------------------------------------------
