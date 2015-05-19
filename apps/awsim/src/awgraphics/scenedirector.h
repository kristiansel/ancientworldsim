#ifndef AWSCENEDIRECTOR_H
#define AWSCENEDIRECTOR_H

#include <OgreRoot.h>
#include <OgreSceneManager.h>

namespace AWGraphics
{
    class SceneDirector
    {
    public:
        SceneDirector(Ogre::Root* root_node, Ogre::SceneManager* scene_mgr);

        void createPlayer();
        Ogre::SceneNode* getPlayerNode();

        Ogre::SceneNode* createBox(float x, float y, float z);
        Ogre::SceneNode* createPlane(float x, float y, float z);

    private:
        Ogre::Root* mRoot; // Engine class is owner
        Ogre::SceneManager* mSceneMgr; // Ogre::Root class is owner
        Ogre::SceneNode *mPlayerNode; // Ogre::SceneManager class is owner
    };
}
#endif // AWSCENEDIRECTOR_H
