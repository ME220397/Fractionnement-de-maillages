#ifndef VORONOI_H
#define VORONOI_H
#include <iostream>
#include "MeshFormat.h"
#include <seedgenerator.h>
#include "mesh.h"

class Voronoi
{
public:
    Voronoi(QVector<MyMesh::Point> points, MyMesh mesh);
private:
    QVector<MyMesh::Point> points;
    MyMesh mesh;
};

#endif // VORONOI_H
