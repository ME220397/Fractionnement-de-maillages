#include "geometry.h"

Geometry::Geometry()
{

}


MyMesh::Point Geometry::get_midpoint(MyMesh::Point A, MyMesh::Point B)
{
    MyMesh::Point I;//Le milieu
    I[0] = (A[0] + B[0])/2;
    I[1] = (A[1] + B[1])/2;
    I[2] = (A[2] + B[2])/2;
    return I;
}

MyMesh::Point Geometry::get_vect(MyMesh::Point A, MyMesh::Point B)
{
    MyMesh::Point AB;
    AB[0] = B[0]-A[0];
    AB[1] = B[1]-A[1];
    AB[2] = B[2]-A[2];
    return AB;
}

std::vector Geometry::get_mediator_plan(MyMesh::Point A, MyMesh::Point B)
{

}
