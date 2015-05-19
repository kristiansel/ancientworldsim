#ifndef CAMERA_H
#define CAMERA_H

#include <OgreCamera.h>
#include <OgreSceneManager.h>
#include <OgreRenderWindow.h>

namespace AWGraphics
{
    class Camera
    {
    public:
        Camera(Ogre::SceneManager *sceneMgr, Ogre::RenderWindow *window);
        ~Camera();

        void attachChaseCam(Ogre::SceneNode *targetNode);
    private:
        Ogre::Camera* mCamera; // gets deleted by SceneManger
        Ogre::String mName;

        Ogre::SceneNode *mTargetNode; // The camera target
        Ogre::SceneNode *mCameraNode; // The camera itself

        Ogre::SceneManager *mSceneMgr;
        Ogre::Real mTightness;

    };

    Camera *createCamera(Ogre::SceneManager *sceneMgr, Ogre::RenderWindow *window);
}

#endif // CAMERA_H
