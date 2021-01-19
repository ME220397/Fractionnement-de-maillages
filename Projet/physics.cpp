#include "physics.h"

Physics::Physics()
{
    init_sim();
}

void Physics::init_sim(){
    collisionConfiguration = new btDefaultCollisionConfiguration();
    dispatcher = new btCollisionDispatcher(collisionConfiguration);
    overlappingPairCache = new btDbvtBroadphase();
    solver = new btSequentialImpulseConstraintSolver;
    dynamicsWorld = new btDiscreteDynamicsWorld(dispatcher, overlappingPairCache, solver, collisionConfiguration);
    dynamicsWorld->setGravity(btVector3(0, -50, 0));
    qDebug() << "Initialisation de Physics";
}

void Physics::createSol(Boite *sol){
    //Creation du corps du sol
    {
        struct MyShape groundshape;
        groundshape.shape = new btBoxShape(btVector3(btScalar(sol->cote/2), btScalar(sol->cote/2), btScalar(sol->cote/2)));
        groundshape.num_shape = nb_dynamic_obj;
        nb_dynamic_obj++;
        collisionShapes.push_back(groundshape);

        btTransform groundTransform;
        groundTransform.setIdentity();
        groundTransform.setOrigin(btVector3(0, 0, 0));

        btScalar mass(0.);

        bool isDynamic = (mass != 0.f);

        btVector3 localInertia(0,0,0);
        if (isDynamic) groundshape.shape->calculateLocalInertia(mass, localInertia);

        btDefaultMotionState* myMotionState = new btDefaultMotionState(groundTransform);
        btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, myMotionState, groundshape.shape, localInertia);
        btRigidBody* body = new btRigidBody(rbInfo);

        dynamicsWorld->addRigidBody(body);
    }
}

void Physics::createConvex(Mesh *mesh){
    {
        struct MyShape convexShape;

        convexShape.shape = new btConvexHullShape();
        collisionShapes.push_back(convexShape);
        convexShape.num_shape = nb_dynamic_obj;
        nb_dynamic_obj++;
        //Parcourir tous les points du mesh --> btVector3
        //Et ensuite ajouter le point avec convexShape.addPoint(btVector);

        btTransform convexTransform;
        convexTransform.setIdentity();
        convexTransform.setOrigin(btVector3(mesh->get_position()[0], mesh->get_position()[1], mesh->get_position()[2]));

        btScalar mass(.5);

        bool isDynamic = (mass != 0.f);

        btVector3 localInertia(0,0,0);
        if (isDynamic) convexShape.shape->calculateLocalInertia(mass, localInertia);

        btDefaultMotionState* myMotionState = new btDefaultMotionState(convexTransform);
        btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, myMotionState, convexShape.shape, localInertia);
        btRigidBody* body = new btRigidBody(rbInfo);

        dynamicsWorld->addRigidBody(body);
        nb_dynamic_obj++;
    }
}

void Physics::anim(){
    dynamicsWorld->stepSimulation(1.f / 60.f, 10);

    for(int i = 0; i < nb_dynamic_obj; i++){
        btCollisionObject* obj = dynamicsWorld->getCollisionObjectArray()[i];
        btRigidBody* body = btRigidBody::upcast(obj);
        btTransform trans;
        if(body && body->getMotionState()){
            body->getMotionState()->getWorldTransform(trans);
        }
        else {
            trans = obj->getWorldTransform();
        }
        //Attention le sol n'est pas un mesh.
        collisionShapes[i].mesh.set_positionX(trans.getOrigin().getX());
        collisionShapes[i].mesh.set_positionY(trans.getOrigin().getY());
        collisionShapes[i].mesh.set_positionZ(trans.getOrigin().getZ());
    }
}


