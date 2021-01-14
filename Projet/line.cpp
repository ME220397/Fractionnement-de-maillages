#include "line.h"

Line::Line(MyMesh::Point position, MyMesh::Point u)
{
    this->position = position;
    this->u = u;
}

MyMesh::Point Line::get_position(){
    return position;
}

MyMesh::Point Line::get_u(){
    return u;
}
