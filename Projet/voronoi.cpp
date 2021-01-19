#include "voronoi.h"

Voronoi::Voronoi(QVector<MyMesh::Point> points, MyMesh mesh)
{
    this->points = points;
    this->mesh = mesh;
}
