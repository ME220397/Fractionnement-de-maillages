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

void Boite::create_boite(){
//    -cote/2, cote/2, cote/2,//A
//    cote/2, cote/2, cote/2,//B
//    cote/2, cote/2, -cote/2,//C
//    -cote/2, cote/2, -cote/2,//D
//    -cote/2, -cote/2, cote/2,//E
//    cote/2, -cote/2, cote/2,//F
//    cote/2, -cote/2, -cote/2,//G
//   -cote/2, -cote/2, -cote/2,//H
    QVector<GLfloat> color_cube = {r ,g ,b};

    QVector<GLfloat> vertices_cube = {
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

    for(int i = 0; i < 12*3; ++i){
        for(int j = 0; j < 3; ++j){
            boite_data.append(vertices_cube[i*3+j]);
        }
        for(int j = 0; j < 3; ++j){
            boite_data.append(color_cube[j]);
        }
    }
}

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

    GLfloat color_sol_g[] = {
            0.0f, 1.0f, 0.0f
        };

    QVector<GLfloat> vertData_sol;

    for(int i = 0; i < 12*3; ++i){
        for(int j = 0; j < 3; ++j){
            boite_data.append(vertices_sol[i*3+j]);
        }
        for(int j = 0; j < 3; ++j){
            boite_data.append(color_sol_g[j]);
        }
    }
}

void Boite::anime(btDiscreteDynamicsWorld* dynamicWorld){
    dynamicWorld->stepSimulation(1.f / 600.f, 10);

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
