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
    static QVector<Plane> get_mediator_planes(MyMesh::Point p, Mesh mesh);
    static QVector<Line> get_intersection_Line_mesh_plane(Plane p, QVector<Plane>);
    static QVector<Mesh> compute_voronoi(Mesh mesh, QVector<MyMesh::Point> points);
    static QVector<MyMesh::Point> get_points(QVector<Line> lines);
    static QVector<Mesh> extract_meshes(QVector<QVector<MyMesh::Point>> points, MyMesh*_mesh);

private:
    QVector<MyMesh::Point> points;
    MyMesh mesh;
};

#endif // VORONOI_H
