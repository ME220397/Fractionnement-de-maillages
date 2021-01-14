#ifndef GEOMETRY_H
#define GEOMETRY_H

#include <iostream>

class Geometry
{
public:
    Geometry();
    MyMesh::Point get_midpoint(MyMesh::Point A, MyMesh::Point B); //Recupere le milieur de AB
    MyMesh::Point get_vect(MyMesh::Point A, MyMesh::Point B); //Recupere le vecteur AB
    std::vector<MyMesh::Point> get_mediator_plan(MyMesh::Point A, MyMesh::Point B); //Recupere le plan mediateur du vecteur AB
    bool is_intersected(std::vector<MyMeshPoint> P, std::vector<MyMesh::Point> Q);
    bool colinear(MyMesh::Point A, MyMesh::Point B);
};

#endif // GEOMETRY_H
