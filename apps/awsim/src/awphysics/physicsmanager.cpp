#include "physicsmanager.h"

AWPhysics::PhysicsManager::PhysicsManager()
    :
      m_broadphase(nullptr),
      m_dispatcher(nullptr),
      m_solver(nullptr),
      m_collisionConfiguration(nullptr),
      m_dynamicsWorld(nullptr),
      mLogger("awphysics.log")
{
    mLogger << "PhysicsManager initialized";
}

AWPhysics::PhysicsManager::~PhysicsManager()
{
    destroyPhysicsWorld();
    mLogger << "PhysicsManager destroyed";
}

void AWPhysics::PhysicsManager::initPhysicsWorld()
{
    // This leaks if called twice in a row. Therefore destroy first always:
    destroyPhysicsWorld();

    mLogger << "creating physics world";
    ///collision configuration contains default setup for memory, collision setup
    m_collisionConfiguration = new btDefaultCollisionConfiguration();
    //m_collisionConfiguration->setConvexConvexMultipointIterations();

    ///use the default collision dispatcher. For parallel processing you can use a diffent dispatcher (see Extras/BulletMultiThreaded)
    m_dispatcher = new	btCollisionDispatcher(m_collisionConfiguration);

    m_broadphase = new btDbvtBroadphase();

    ///the default constraint solver. For parallel processing you can use a different solver (see Extras/BulletMultiThreaded)
    btSequentialImpulseConstraintSolver* sol = new btSequentialImpulseConstraintSolver;
    m_solver = sol;

    m_dynamicsWorld = new btDiscreteDynamicsWorld(m_dispatcher, m_broadphase, m_solver, m_collisionConfiguration);

    m_dynamicsWorld->setGravity(btVector3(0, -10, 0));
    mLogger << "finished creating physics world";
}

void AWPhysics::PhysicsManager::destroyPhysicsWorld()
{
    if (m_dynamicsWorld)
    {
        mLogger << "destroying physics world";

        for (int i = m_dynamicsWorld->getNumConstraints() - 1; i >= 0; i--)
        {
            m_dynamicsWorld->removeConstraint(m_dynamicsWorld->getConstraint(i));
        }

        for (int i = m_dynamicsWorld->getNumCollisionObjects() - 1; i >= 0; i--)
        {
            btCollisionObject* obj = m_dynamicsWorld->getCollisionObjectArray()[i];
            btRigidBody* body = btRigidBody::upcast(obj);

            if (body && body->getMotionState())
            {
                delete body->getMotionState();
            }

            m_dynamicsWorld->removeCollisionObject(obj);
            delete obj;
        }
    }

    //delete collision shapes
    for (int j = 0; j<m_collisionShapes.size(); j++)
    {
        btCollisionShape* shape = m_collisionShapes[j];
        delete shape;
    }
    m_collisionShapes.clear();

    delete m_dynamicsWorld;
    m_dynamicsWorld=nullptr;

    delete m_solver;
    m_solver=nullptr;

    delete m_broadphase;
    m_broadphase=nullptr;

    delete m_dispatcher;
    m_dispatcher=nullptr;

    delete m_collisionConfiguration;
    m_collisionConfiguration=nullptr;
}

void AWPhysics::PhysicsManager::updatePhysics(float dt)
{
    // implement                    timeStep, maxSubSteps,
    m_dynamicsWorld->stepSimulation(dt, 4);

    //mLogger << "updating physics";
    /*
    for (int i = m_dynamicsWorld->getNumCollisionObjects() - 1; i >= 0; i--)
    {
        btCollisionObject* obj = m_dynamicsWorld->getCollisionObjectArray()[i];
        btRigidBody* body = btRigidBody::upcast(obj);
        if (body && body->getMotionState())
        {
            btTransform b_transf;
            body->getMotionState()->getWorldTransform(b_transf);

            btVector3 b_pos = b_transf.getOrigin();

            //mLogger.reportStringStream(std::stringstream() <<
             //          "RigidBody #" << i << ": " << b_pos.x() << ", " << b_pos.y() << ", " << b_pos.z()
             //                          );
        }
    }
    */

    // recompile please

}

btRigidBody*	AWPhysics::PhysicsManager::createRigidBody(float mass, const btTransform& startTransform, btCollisionShape* shape)
{
    btAssert((!shape || shape->getShapeType() != INVALID_SHAPE_PROXYTYPE));

    //rigidbody is dynamic if and only if mass is non zero, otherwise static
    bool isDynamic = (mass != 0.f);

    btVector3 localInertia(0, 0, 0);
    if (isDynamic)
        shape->calculateLocalInertia(mass, localInertia);

    //using motionstate is recommended, it provides interpolation capabilities, and only synchronizes 'active' objects

#define USE_MOTIONSTATE 1
#ifdef USE_MOTIONSTATE
    btDefaultMotionState* myMotionState = new btDefaultMotionState(startTransform);

    btRigidBody::btRigidBodyConstructionInfo cInfo(mass, myMotionState, shape, localInertia);

    btRigidBody* body = new btRigidBody(cInfo);
    //body->setContactProcessingThreshold(m_defaultContactProcessingThreshold);

#else
    btRigidBody* body = new btRigidBody(mass, 0, shape, localInertia);
    body->setWorldTransform(startTransform);
#endif//

    body->setUserIndex(-1);
    m_dynamicsWorld->addRigidBody(body);
    return body;
}

btRigidBody* AWPhysics::PhysicsManager::createBox(float x, float y, float z)
{
    return createRigidBody( 10.f,
                            btTransform( btTransform().getRotation(),
                                         btVector3(x, y, z) ),
                            new btBoxShape( btVector3(1.0, 1.0, 1.0) ) );

    mLogger << "creating Box rigid body";
}

btRigidBody* AWPhysics::PhysicsManager::createPlane(float x, float y, float z)
{
    return createRigidBody( 0.f, // zero mass == static object
                            btTransform( btQuaternion::getIdentity(), // wow valgrind memory analyzer found the old bug here...
                                         btVector3(x, y, z) ),
                            //                      normal                    constant
                            new btStaticPlaneShape( btVector3(0.0, 1.0, 0.0), btScalar(0.0)));
                            // assume that the plane is placed at: (x, y, z) = scalar*normal

    mLogger << "creating Plane rigid body";
}



