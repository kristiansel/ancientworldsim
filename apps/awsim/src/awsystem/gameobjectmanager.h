#ifndef AWGAMEOBJECTMANAGER_H
#define AWGAMEOBJECTMANAGER_H

#include "gameobject.h"
#include "../awphysics/physicsmanager.h"
#include "../awgraphics/scenedirector.h"

namespace AWSystem
{
    namespace SubSysShared
    {
        struct PosRot // position rotation
        {
            Ogre::SceneNode* scene_node;
            btRigidBody* rigid_body;

            static void updateFromPhysics(PosRot &pos_rot);
            static void updateToGraphics(PosRot &pos_rot);

            struct Position
            {
                float x, y, z;
            } pos;

            struct Rotation
            {
                float w, x, y, z;
            } rot;
        };

    }

    static constexpr float PHYS2GRAPH_LENUNIT = 100.f;
    static constexpr float GRAPH2PHYS_LENUNIT = 1.f/PHYS2GRAPH_LENUNIT;

    class GameObjectManager
    {
    public:
        GameObjectManager(AWGraphics::SceneDirector* sceneDirector, AWPhysics::PhysicsManager* physicsManager);

        GameObject* createBox(float x, float y, float z);
        GameObject* createPlane(float x, float y, float z);

        void updateGraphics();
    private:
        typedef std::list<GameObject> GameObjectVector; // perhaps change allocator or something on this later?
        GameObjectVector mGameObjects;

        std::vector<SubSysShared::PosRot> mObjectPosRots; // Physics, Graphics, AI shared data

        AWGraphics::SceneDirector* mSceneDirector;
        AWPhysics::PhysicsManager* mPhysicsManager;
    };

}

#endif // AWGAMEOBJECTMANAGER_H
