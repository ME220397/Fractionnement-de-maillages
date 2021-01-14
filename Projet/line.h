#ifndef LINE_H
#define LINE_H

#include "MeshFormat.h"

class Line
{
public:
    Line(MyMesh::Point postition, MyMesh::Point u);

    // Getters
    MyMesh::Point get_position();
    MyMesh::Point get_u();
private:
    MyMesh::Point position;
    MyMesh::Point u;
};

#endif // LINE_H
