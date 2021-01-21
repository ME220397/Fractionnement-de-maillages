#include "physics.h"

Physics::Physics()
{
    init_sim();
}

// Initialisation du monde physique.
void Physics::init_sim(){
    collisionConfiguration = new btDefaultCollisionConfiguration();
    dispatcher = new btCollisionDispatcher(collisionConfiguration);
    overlappingPairCache = new btDbvtBroadphase();
    solver = new btSequentialImpulseConstraintSolver;
    dynamicsWorld = new btDiscreteDynamicsWorld(dispatcher, overlappingPairCache, solver, collisionConfiguration);
    dynamicsWorld->setGravity(btVector3(0, -50, 0));
    qDebug() << "Initialisation de Physics";
}

btDiscreteDynamicsWorld* Physics::get_world(){
    return dynamicsWorld;
}

// Permet de créer l'enveloppe physique du sol.
void Physics::createSol(Boite *sol){
    {
        btCollisionShape* groundshape = new btBoxShape(btVector3(btScalar(sol->cote/2), btScalar(sol->cote/2), btScalar(sol->cote/2)));
        sol->numObj = nb_dynamic_obj;
        nb_dynamic_obj++;
        collisionShapes.push_back(groundshape);

        btTransform groundTransform;
        groundTransform.setIdentity();
        groundTransform.setOrigin(btVector3(sol->position[0], sol->position[1], sol->position[2]));

        btScalar mass(0.);

        bool isDynamic = (mass != 0.f);

        btVector3 localInertia(0,0,0);
        if (isDynamic) groundshape->calculateLocalInertia(mass, localInertia);

        btDefaultMotionState* myMotionState = new btDefaultMotionState(groundTransform);
        btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, myMotionState, groundshape, localInertia);
        btRigidBody* body = new btRigidBody(rbInfo);

        dynamicsWorld->addRigidBody(body);
    }
}

// Permet de créer l'enveloppe physique autour d'un mesh convexe pour gérer les collisions.
void Physics::createConvex(Mesh *mesh){
    {
        btConvexHullShape* convexShape = new btConvexHullShape();
        MyMesh _mymesh = mesh->get_mesh();
        int i = 0;
        for(MyMesh::VertexIter v_it = _mymesh.vertices_begin(); v_it != _mymesh.vertices_end(); v_it++){
            i++;
            VertexHandle current_vertex = *v_it;
            btVector3 point(btScalar(_mymesh.point(current_vertex)[0]), btScalar(_mymesh.point(current_vertex)[1]), btScalar(_mymesh.point(current_vertex)[2]));
            convexShape->addPoint(point, false);
            convexShape->recalcLocalAabb();
        }
        convexShape->setMargin(0);

        collisionShapes.push_back(convexShape);
        mesh->set_numObj(nb_dynamic_obj);

        btTransform convexTransform;
        convexTransform.setIdentity();
        convexTransform.setOrigin(btVector3(mesh->get_position()[0], mesh->get_position()[1], mesh->get_position()[2]));

        btScalar mass(1.f);

        bool isDynamic = (mass != 0.f);

        btVector3 localInertia(0,0,0);
        if (isDynamic) convexShape->calculateLocalInertia(mass, localInertia);

        btDefaultMotionState* myMotionState = new btDefaultMotionState(convexTransform);
        btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, myMotionState, convexShape, localInertia);

        btRigidBody *body = new btRigidBody(rbInfo);
//        body->setLinearVelocity(btVector3(500, 0, 0));

        dynamicsWorld->addRigidBody(body);
        nb_dynamic_obj++;
    }
}

// Detruit le monde physique.
void Physics::delete_sim(){
    for (int i = dynamicsWorld->getNumCollisionObjects() - 1; i >= 0; i--)
        {
            btCollisionObject* obj = dynamicsWorld->getCollisionObjectArray()[i];
            btRigidBody* body = btRigidBody::upcast(obj);
            if (body && body->getMotionState())
            {
                delete body->getMotionState();
            }
            dynamicsWorld->removeCollisionObject(obj);
            delete obj;
        }

        //delete collision shapes
        for (int j = 0; j < collisionShapes.size(); j++)
        {
            btCollisionShape* shape = collisionShapes[j];
            collisionShapes[j] = 0;
            delete shape;
        }

        //delete dynamics world
        delete dynamicsWorld;

        //delete solver
        delete solver;

        //delete broadphase
        delete overlappingPairCache;

        //delete dispatcher
        delete dispatcher;

        delete collisionConfiguration;

        //next line is optional: it will be cleared by the destructor when the array goes out of scope
        collisionShapes.clear();
}
