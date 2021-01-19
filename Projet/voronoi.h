#ifndef VORONOI_H
#define VORONOI_H
#include <iostream>
#include "MeshFormat.h"
#include <seedgenerator.h>
#include "mesh.h"
#include "plane.h"
#include "geometry.h"

class Voronoi
{
public:
    Voronoi(QVector<MyMesh::Point> points, MyMesh mesh);
    QVector<Plane> get_mediator_planes(MyMesh::Point p, Mesh mesh);
    QVector<Mesh> compute_voronoi();

private:
    QVector<MyMesh::Point> points;
    MyMesh mesh;
};

#endif // VORONOI_H
