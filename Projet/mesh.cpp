#include "mesh.h"

Mesh::Mesh(MyMesh *_mesh)
{
    this->mesh = *_mesh;
}

// Fonctions pour changer la couleur

void Mesh::color_point_by_id(int id, int r, int g, int b){
    MyMesh *_mesh = &mesh;

    VertexHandle vh = _mesh->vertex_handle(id);
    _mesh->set_color(vh, MyMesh::Color(r, g, b));
}

void Mesh::color_point(VertexHandle vh, int r, int g, int b){
    MyMesh *_mesh = &mesh;
    _mesh->set_color(vh, MyMesh::Color(r, g, b));
}

void Mesh::color_face_by_id(int id, int r, int g, int b){
    MyMesh *_mesh = &mesh;

    FaceHandle fh = _mesh->face_handle(id);
    _mesh->set_color(fh, MyMesh::Color(r, g, b));
}

void Mesh::color_face(FaceHandle fh, int r, int g, int b){
    MyMesh *_mesh = &mesh;
    _mesh->set_color(fh, MyMesh::Color(r, g, b));
}

void Mesh::color_edge_by_id(int id, int r, int g, int b){
    MyMesh *_mesh = &mesh;

    EdgeHandle eh = _mesh->edge_handle(id);
    _mesh->set_color(eh, MyMesh::Color(r, g, b));
}

void Mesh::color_edge(EdgeHandle eh, int r, int g, int b){
    MyMesh *_mesh = &mesh;
    _mesh->set_color(eh, MyMesh::Color(r, g, b));
}

// Fonctions pour changer la taille

void Mesh::thickness_point_by_id(int id, float size){
    MyMesh *_mesh = &mesh;

    VertexHandle vh = _mesh->vertex_handle(id);
    _mesh->data(vh).thickness = size;
}

void Mesh::thickness_point(VertexHandle vh, float size){
    MyMesh *_mesh = &mesh;
    _mesh->data(vh).thickness = size;
}

void Mesh::thickness_edge_by_id(int id, float size){
    MyMesh *_mesh = &mesh;

    EdgeHandle eh = _mesh->edge_handle(id);
    _mesh->data(eh).thickness = size;
}

void Mesh::thickness_edge(EdgeHandle eh, float size){
    MyMesh *_mesh = &mesh;
    _mesh->data(eh).thickness = size;
}
