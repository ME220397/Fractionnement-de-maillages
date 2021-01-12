#ifndef GEOMETRY_H
#define GEOMETRY_H

#include <iostream>

class Geometry
{
public:
    Geometry();
    MyMesh::Point get_midpoint(MyMesh::Point A, MyMesh::Point B); //Recupere le milieur de AB
    MyMesh::Point get_vect(MyMesh::Point A, MyMesh::Point B); //Recupere le vecteur AB
    std::vector get_mediator_plan(MyMesh::Point A, MyMesh::Point B); //Recupere le plan mediateur du vecteur AB
};

#endif // GEOMETRY_H
