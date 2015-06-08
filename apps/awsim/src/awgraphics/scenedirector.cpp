#include "scenedirector.h"

#include <OgreEntity.h>
#include <OgreMeshManager.h>

AWGraphics::SceneDirector::SceneDirector(Ogre::Root* ogre_root, Ogre::SceneManager* scene_mgr)
    : mRoot(ogre_root),
      mSceneMgr(scene_mgr)
{

}

void AWGraphics::SceneDirector::createPlayer()
{
    // Add player entity to the scene
    Ogre::Entity* ogreEntity = mSceneMgr->createEntity("ogrehead.mesh");

    Ogre::SceneNode* parentNode = mSceneMgr->getRootSceneNode()->createChildSceneNode();
    Ogre::SceneNode* ogreNode = parentNode->createChildSceneNode();
    ogreNode->attachObject(ogreEntity);
    ogreNode->rotate(Ogre::Vector3(0.0, 1.0, 0.0), Ogre::Radian(Ogre::Degree(180)));
    ogreNode->setPosition(0, 80, 0);

    mPlayerNode = parentNode;

    // Initialize and add a light
    mSceneMgr->setAmbientLight(Ogre::ColourValue(.5, .5, .5));

    Ogre::Light* light = mSceneMgr->createLight("MainLight");
    light->setType(Ogre::Light::LT_DIRECTIONAL);
    light->setDirection(Ogre::Vector3(0, -1, 1));

    /*// Directional light
    Ogre::Light* directionalLight = mSceneMgr->createLight("DirectionalLight");
    directionalLight->setType(Ogre::Light::LT_DIRECTIONAL);

    directionalLight->setDiffuseColour(Ogre::ColourValue(.8, .6, .2));
    directionalLight->setSpecularColour(Ogre::ColourValue(.8, .6, .2));
    */

    // Add skydome
    mSceneMgr->setSkyDome(true, "CloudySky", 5, 8); // cannot find... :(
}

Ogre::SceneNode* AWGraphics::SceneDirector::createBox(float x, float y, float z)
{
    // Add box entity to the scene
    Ogre::Entity* ogreEntity = mSceneMgr->createEntity("cube.mesh");

    Ogre::SceneNode* ogreNode = mSceneMgr->getRootSceneNode()->createChildSceneNode();
    ogreNode->attachObject(ogreEntity);
    ogreNode->setPosition(x, y, z);

    return ogreNode;
}


Ogre::SceneNode* AWGraphics::SceneDirector::createPlane(float x, float y, float z)
{
    Ogre::Plane plane = Ogre::Plane(Ogre::Vector3::UNIT_Y, 0);

    // Initialize a ground entity for later use
    Ogre::MeshManager::getSingleton().createPlane(
        "ground",
        Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
        plane,
        1500, 1500, 20, 20,
        true,
        1, 5, 5,
        Ogre::Vector3::UNIT_Z);

    //Ogre::MeshManager::getSingleton().createPlane()

    Ogre::Entity* groundEntity = mSceneMgr->createEntity("ground");
    groundEntity->setMaterialName("Rockwall");
    groundEntity->setCastShadows(false);

    Ogre::SceneNode* ogreNode = mSceneMgr->getRootSceneNode()->createChildSceneNode();
    ogreNode->attachObject(groundEntity);
    ogreNode->setPosition(x, y, z);
    ogreNode->setOrientation(Ogre::Quaternion::IDENTITY); // hope this is  a good reference

    return ogreNode;
}


Ogre::SceneNode* AWGraphics::SceneDirector::getPlayerNode()
{
    return mPlayerNode;
}
