#include "boite.h"

Boite::Boite(QVector3D position, float cote, int numObj, int index_debut_vbo)
{
    this->position = position;
    this->numObj = numObj;
    this->cote = cote;
    r = 1.f;
    g = 0.f;
    b = 0.f;
    this->index_debut_vbo = index_debut_vbo;
}

Boite::Boite(QVector3D position, float cote, int numObj, int index_debut_vbo, float r, float g, float b)
{
    this->position = position;
    this->numObj = numObj;
    this->cote = cote;
    this->r = r;
    this->g = g;
    this->b = b;
    this->index_debut_vbo = index_debut_vbo;
}

Boite::Boite(){};

void Boite::create_sol(){
    GLfloat vertices_sol[] = {
        -cote/2, cote/2, cote/2,
        cote/2, cote/2, cote/2,
        cote/2, cote/2, -cote/2,//ABC
        -cote/2, cote/2, cote/2,
        cote/2, cote/2, -cote/2,
        -cote/2, cote/2, -cote/2,//ACD
        -cote/2, cote/2, cote/2,
        cote/2, cote/2, cote/2,
        -cote/2, -cote/2, cote/2,//ABE
        cote/2, cote/2, cote/2,
        cote/2, -cote/2, cote/2,
        -cote/2, -cote/2, cote/2,//BFE
        -cote/2, -cote/2, cote/2,
        cote/2, -cote/2, cote/2,
        -cote/2, -cote/2, -cote/2,//EFH
        cote/2, -cote/2, cote/2,
        cote/2, -cote/2, -cote/2,
        -cote/2, -cote/2, -cote/2,//FGH
        -cote/2, cote/2, -cote/2,
        cote/2, cote/2, -cote/2,
        cote/2, -cote/2, -cote/2,//DCG
        -cote/2, cote/2, -cote/2,
        cote/2, -cote/2, -cote/2,
        -cote/2, -cote/2, -cote/2,//DGH
        -cote/2, cote/2, cote/2,
        -cote/2, -cote/2, -cote/2,
        -cote/2, cote/2, -cote/2,//AHD
        -cote/2, cote/2, cote/2,
        -cote/2, -cote/2, cote/2,
        -cote/2, -cote/2, -cote/2,//AEH
        cote/2, cote/2, cote/2,
        cote/2, -cote/2, cote/2,
        cote/2, cote/2, -cote/2,//BFC
        cote/2, -cote/2, cote/2,
        cote/2, -cote/2, -cote/2,
        cote/2, cote/2, -cote/2//FGC
        };

    GLfloat texCoords_sol[] = {
                0.0f, 0.0f,
                0.0f, 1.0f,
                1.0f, 1.0f,
                1.0f, 1.0f,
                1.0f, 0.0f,
                0.0f, 0.0f
            };
    QVector<GLfloat> vertData_sol;

    for(int i = 0; i < 12*3; ++i){
        for(int j = 0; j < 3; ++j){
            boite_data.append(vertices_sol[i*3+j]);
        }
        for(int j = 0; j < 2; ++j){
            boite_data.append(texCoords_sol[i*2+j]);
        }
    }
}

void Boite::anime(btDiscreteDynamicsWorld* dynamicWorld){
    dynamicWorld->stepSimulation(1.f / 60.f, 10);

    btCollisionObject* obj = dynamicWorld->getCollisionObjectArray()[numObj];
    btRigidBody* body = btRigidBody::upcast(obj);
    btTransform trans;
    if(body && body->getMotionState()){
        body->getMotionState()->getWorldTransform(trans);
    }
    else {
        trans = obj->getWorldTransform();
    }

    this->position.setX(trans.getOrigin().getX());
    this->position.setY(trans.getOrigin().getY());
    this->position.setZ(trans.getOrigin().getZ());
}
