#ifndef GEOMETRY_H
#define GEOMETRY_H

#include <iostream>
#include "MeshFormat.h"
#include "plane.h"
#include "line.h"
#include "QVector4D"
#include "QMatrix4x4"
class Geometry
{
public:
    Geometry();
    MyMesh::Point get_midpoint(MyMesh::Point A, MyMesh::Point B); //Recupere le milieur de AB
    MyMesh::Point get_vect(MyMesh::Point A, MyMesh::Point B); //Recupere le vecteur AB
    Plane get_mediator_plan(MyMesh::Point A, MyMesh::Point B); //Recupere le plan mediateur du vecteur AB
    Line get_intersection_line(Plane p, Plane q); //Recupere la droite d'intersection de deux plans

    // Geometry on plane
        //Static Functions
    static bool intersected(Plane p1, Plane p2);
    static QVector3D to_Qvector3D(MyMesh::Point);
    static QMatrix4x4 change_of_base(Plane p);
    static QVector3D get_normal(Plane p);
    static QVector4D get_equation(Plane p);
    static bool is_perpendicular(MyMesh::Point p, MyMesh::Point q); //Renvoie true si les deux vecteurs sont perpendiculaires
};

#endif // GEOMETRY_H
