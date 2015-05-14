#include "camera.h"

AWGraphics::Camera *AWGraphics::createCamera(Ogre::SceneManager *sceneMgr,
                                             Ogre::RenderWindow *window)
{
    return new Camera(sceneMgr, window);
}

AWGraphics::Camera::Camera(Ogre::SceneManager *sceneMgr, Ogre::RenderWindow *window)
    : mSceneMgr(sceneMgr),
      mName(Ogre::String("main_camera"))
{
    // Initialize and add a camera
    mCamera = mSceneMgr->createCamera("MainCam");

    mCamera->setPosition(0, 0, 80);
    mCamera->lookAt(0, 0, -300);
    mCamera->setNearClipDistance(5);

    // Initialize and add a viewport
    Ogre::Viewport* vp = window->addViewport(mCamera);

    vp->setBackgroundColour(Ogre::ColourValue(0,0,0));

    mCamera->setAspectRatio(
        Ogre::Real(vp->getActualWidth()) /
        Ogre::Real(vp->getActualHeight()));

     // Create the camera's node structure
     mCameraNode = mSceneMgr->getRootSceneNode ()->createChildSceneNode ();
     mCameraNode->translate(0, 0, 80);

     // ... and attach the Ogre camera to the camera node
     //mCameraNode->attachObject (mCamera);

     // Default tightness
     mTightness = 0.01f;
}

AWGraphics::Camera::~Camera()
{
    mCameraNode->detachAllObjects ();
    // delete mCamera; //??
    mSceneMgr->destroySceneNode (mName);
    mSceneMgr->destroySceneNode (mName + "_target");
}

void AWGraphics::Camera::attachChaseCam(Ogre::SceneNode *targetNode)
{
    mTargetNode = targetNode;
    mCameraNode->setAutoTracking (true, mTargetNode); // The camera will always look at the camera target
    mCameraNode->setFixedYawAxis (true); // Needed because of auto tracking
}

