#ifndef PLANE_H
#define PLANE_H

#include "MeshFormat.h"

class Plane
{
public:
    Plane(MyMesh::Point postition, MyMesh::Point u, MyMesh::Point v);

    // Getters
    MyMesh::Point get_position();
    MyMesh::Point get_u();
    MyMesh::Point get_v();
private:
    MyMesh::Point position;
    MyMesh::Point u;
    MyMesh::Point v;
};

#endif // PLANE_H
