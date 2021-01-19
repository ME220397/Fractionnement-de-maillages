#ifndef PHYSICS_H
#define PHYSICS_H

#include "btBulletDynamicsCommon.h"
#include "boite.h"
#include "mesh.h"

struct MyShape {
    Mesh mesh;
    btCollisionShape* shape;
    int num_shape;
};

class Physics
{
public:
    Physics();
    void init_sim();
    void createSol(Boite *sol);
    void createConvex(Mesh *mesh);
    void anim();

    //keep track of the shapes, we release memory at exit.
    //make sure to re-use collision shapes among rigid bodies whenever possible!
    btAlignedObjectArray<MyShape> collisionShapes;

private:
    int nb_dynamic_obj = 0;
    ///collision configuration contains default setup for memory, collision setup. Advanced users can create their own configuration.
    btDefaultCollisionConfiguration* collisionConfiguration;
    ///use the default collision dispatcher. For parallel processing you can use a diffent dispatcher (see Extras/BulletMultiThreaded)
    btCollisionDispatcher* dispatcher;
    ///btDbvtBroadphase is a good general purpose broadphase. You can also try out btAxis3Sweep.
    btBroadphaseInterface* overlappingPairCache;
    ///the default constraint solver. For parallel processing you can use a different solver (see Extras/BulletMultiThreaded)
    btSequentialImpulseConstraintSolver* solver;

    btDiscreteDynamicsWorld* dynamicsWorld;
};

#endif // PHYSICS_H
