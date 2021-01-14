#include "plane.h"

Plane::Plane(MyMesh::Point position, MyMesh::Point u, MyMesh::Point v)
{
    this->position = position;
    this->u = u;
    this->v = v;
}

MyMesh::Point Plane::get_position(){
    return position;
}

MyMesh::Point Plane::get_u(){
    return u;
}

MyMesh::Point Plane::get_v(){
    return v;
}
