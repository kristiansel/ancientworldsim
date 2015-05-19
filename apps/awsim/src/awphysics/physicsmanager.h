#ifndef AWPHYSICSMANAGER_H
#define AWPHYSICSMANAGER_H

#include <btBulletDynamicsCommon.h>

#include "../awutility/logger.h"

namespace AWPhysics
{

    class PhysicsManager
    {
    public:
        PhysicsManager();
        virtual ~PhysicsManager();

        btRigidBody* createBox(float x, float y, float z);
        btRigidBody* createPlane(float x, float y, float z);

        void initPhysicsWorld();
        void updatePhysics(float dt);
        void destroyPhysicsWorld();
    protected:
        btRigidBody*    createRigidBody(float mass,
                                        const btTransform& startTransform,
                                        btCollisionShape* shape);

    private:
        btAlignedObjectArray<btCollisionShape*>	m_collisionShapes;
        btBroadphaseInterface*                  m_broadphase;
        btCollisionDispatcher*                  m_dispatcher;
        btConstraintSolver*                     m_solver;
        btDefaultCollisionConfiguration*        m_collisionConfiguration;
        btDiscreteDynamicsWorld*                m_dynamicsWorld;

        AWUtil::Logger mLogger;

    };

}

#endif // AWPHYSICSMANAGER_H
