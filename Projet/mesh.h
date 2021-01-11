#ifndef MESH_H
#define MESH_H

#include<MeshFormat.h>
#include<QOpenGLShaderProgram>
#include<QOpenGLBuffer>
#include<QGLWidget>
#include<QVector>
#include<iostream>

class Mesh
{
public:
    Mesh(MyMesh * mesh);
    void load_data();

    // Changer la couleur
    void color_point_by_id(int id, int r, int g, int b);
    void color_point(VertexHandle vh, int r, int g, int b);
    void color_face_by_id(int id, int r, int g, int b);
    void color_face(FaceHandle fh, int r, int g, int b);
    void color_edge_by_id(int id, int r, int g, int b);
    void color_edge(EdgeHandle eh, int r, int g, int b);

    // Changer la taille
    void thickness_point_by_id(int id, float size);
    void thickness_point(VertexHandle vh, float size);
    void thickness_edge_by_id(int id, float size);
    void thickness_edge(EdgeHandle eh, float size);

private:
    MyMesh mesh;
    QOpenGLBuffer vbo;
};

#endif // MESH_H
