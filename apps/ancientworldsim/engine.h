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

#include <OISEvents.h>
#include <OISInputManager.h>
#include <OISKeyboard.h>
#include <OISMouse.h>

#include <CEGUI/CEGUI.h>
#include <CEGUI/RendererModules/Ogre/Renderer.h> //from 0.8 it's just Ogre/Renderer.h

//---------------------------------------------------------------------------

class Engine : public Ogre::WindowEventListener, public Ogre::FrameListener,
               public OIS::KeyListener, public OIS::MouseListener
{
public:
    Engine();
    virtual ~Engine();

    bool go();
protected:
    // Ogre::WindowEventListener
    virtual void windowResized(Ogre::RenderWindow* rw);
    virtual void windowClosed(Ogre::RenderWindow* rw);

    // Ogre::FrameListener
    virtual bool frameRenderingQueued(const Ogre::FrameEvent& evt);

    // recompile please

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
    Ogre::Camera* mCamera;


    // OIS members
    OIS::InputManager* mInputManager;
    OIS::Mouse* mMouse;
    OIS::Keyboard* mKeyboard;

    // CEGUI members
    CEGUI::OgreRenderer* mRenderer;
};

//---------------------------------------------------------------------------

#endif // #ifndef TUTORIALAPPLICATION_H

//---------------------------------------------------------------------------
