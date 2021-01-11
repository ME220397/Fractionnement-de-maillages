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

    void color_point_by_id(int id, int r, int g, int b);
    void color_point(VertexHandle, int r, int g, int b);
private:
    MyMesh mesh;
    QOpenGLBuffer vbo;
};

#endif // MESH_H
