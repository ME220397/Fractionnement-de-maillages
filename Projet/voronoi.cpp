#include "voronoi.h"

Voronoi::Voronoi(QVector<MyMesh::Point> points, MyMesh mesh)
{
    this->points = points;
    this->mesh = mesh;
}

QVector<Plane> get_mediator_planes(MyMesh::Point p, Mesh mesh)
{
    MyMesh mymesh = mesh.get_mesh();
    MyMesh *_mesh = &mymesh;
    QVector<Plane> planes;
    for(MyMesh::VertexIter v_it = _mesh->vertices_begin(); v_it != _mesh->vertices_end(); v_it++){
        planes.append(Geometry::get_mediator_plan(p, _mesh->point(*v_it)));
    }
    return planes;
}


QVector<Mesh> Voronoi::compute_voronoi()
{
    //Recuperer les plans du mesh
    //Pour chaque point du nuage recuperer les plan mediateur avec les points du mesh qui le contient
    //intersection des plans et des droites obtenues
    //creation de nouveaux mesh dans un QVector<Mesh>

}
