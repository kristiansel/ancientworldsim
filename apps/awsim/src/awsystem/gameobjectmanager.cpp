#include "gameobjectmanager.h"

namespace AWSystem
{

    GameObjectManager::GameObjectManager(AWGraphics::SceneDirector* sceneDirector,
                                         AWPhysics::PhysicsManager* physicsManager)
        : mSceneDirector(sceneDirector),
          mPhysicsManager(physicsManager)
    {
        mObjectPosRots.reserve(40); // try to front load to avoid dynamic allocation
        //mGameObjects.reserve(40); // try to front load to avoid dynamic allocation
    }

    GameObject* GameObjectManager::createBox(float x, float y, float z) // takes physics units
    {
        mGameObjects.push_back(GameObject());
        GameObject* out = &(mGameObjects.back());

        SubSysShared::PosRot pos_rot;
        pos_rot.rigid_body = mPhysicsManager->createBox(x, y, z);
        pos_rot.scene_node = mSceneDirector->createBox(PHYS2GRAPH_LENUNIT*x,
                                                       PHYS2GRAPH_LENUNIT*y,
                                                       PHYS2GRAPH_LENUNIT*z);

        // update the position and rotation
        SubSysShared::PosRot::updateFromPhysics(pos_rot);
        SubSysShared::PosRot::updateToGraphics(pos_rot);

        mObjectPosRots.push_back(pos_rot); // small POD should be ok to copy into here, or is it moved?

        return out;
    }

    GameObject* GameObjectManager::createPlane(float x, float y, float z) // takes physics units
    {
        mGameObjects.push_back(GameObject());
        GameObject* out = &(mGameObjects.back());

        SubSysShared::PosRot pos_rot;
        pos_rot.rigid_body = mPhysicsManager->createPlane(x, y, z);
        pos_rot.scene_node = mSceneDirector->createPlane(PHYS2GRAPH_LENUNIT*x,
                                                       PHYS2GRAPH_LENUNIT*y,
                                                       PHYS2GRAPH_LENUNIT*z);

        // update the position and rotation
        SubSysShared::PosRot::updateFromPhysics(pos_rot);
        SubSysShared::PosRot::updateToGraphics(pos_rot);

        mObjectPosRots.push_back(pos_rot);

        return out;
    }

    inline void SubSysShared::PosRot::updateFromPhysics(PosRot &pos_rot)
    {
        btTransform world_transform = pos_rot.rigid_body->getWorldTransform();
        btQuaternion rot = world_transform.getRotation();
        btVector3 pos = world_transform.getOrigin();

        pos_rot.pos.x = pos.x();
        pos_rot.pos.y = pos.y();
        pos_rot.pos.z = pos.z();

        pos_rot.rot.w = rot.w();
        pos_rot.rot.x = rot.x();
        pos_rot.rot.y = rot.y();
        pos_rot.rot.z = rot.z();
    }

    void GameObjectManager::updateGraphics()
    {
        for (auto &pos_rot : mObjectPosRots)
        {
            SubSysShared::PosRot::updateToGraphics(pos_rot); // should be inlined
        }
    }

    inline void SubSysShared::PosRot::updateToGraphics(PosRot &pos_rot)
    {
        pos_rot.scene_node->setOrientation(pos_rot.rot.w, pos_rot.rot.x, pos_rot.rot.y, pos_rot.rot.z);
        pos_rot.scene_node->setPosition(PHYS2GRAPH_LENUNIT*pos_rot.pos.x,
                                        PHYS2GRAPH_LENUNIT*pos_rot.pos.y,
                                        PHYS2GRAPH_LENUNIT*pos_rot.pos.z);
    }
}
