#include "mesh.h"

Mesh::Mesh(MyMesh *_mesh)
{
    this->mesh = *_mesh;
}

void Mesh::color_point_by_id(int id, int r, int g, int b){
    MyMesh *_mesh = &mesh;

    VertexHandle vh = _mesh->vertex_handle(id);
    _mesh->set_color(vh, MyMesh::Color(r, g, b));
}

void Mesh::color_point(VertexHandle vh, int r, int g, int b){
    MyMesh *_mesh = &mesh;
    _mesh->set_color(vh, MyMesh::Color(r, g, b));
}

