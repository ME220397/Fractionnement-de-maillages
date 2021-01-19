#ifndef CUBE_H
#define CUBE_H

#include <QVector3D>
#include <QDebug>
#include <QOpenGLFunctions>
#include <cmath>
#include "btBulletDynamicsCommon.h"

class Boite
{
public:
    Boite(QVector3D position, float cote, int num_cube, int index_debut_vbo);
    Boite(QVector3D position, float cote, int num_cube, int index_debut_vbo, float r, float g, float b);
    Boite();
    void create_boite();
    void create_sol();
    void anime(btDiscreteDynamicsWorld* dynamicWorld);

    QVector3D position;
    QVector3D translate;
    float cote;
    int numObj;
    int index_debut_vbo;

    float r, g, b;

    QVector<GLfloat> boite_data;
};

#endif // CUBE_H
