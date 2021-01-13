#ifndef MESH_H
#define MESH_H

#include<MeshFormat.h>
#include<QOpenGLShaderProgram>
#include<QOpenGLBuffer>
#include<QGLWidget>
#include<QVector>
#include<QMatrix4x4>
#include<iostream>

class Mesh
{
public:
    Mesh(MyMesh mesh, QVector3D position);
    Mesh(QVector<MyMesh::Point> points, QVector3D position);

    MyMesh compute_bounding_box();
    QVector3D compute_barycentre();
    void resetAllColorsAndThickness(MyMesh* _mesh);

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

    // Gerer l'affichage
    void destroy_vbos();
    void load_data();
    void draw(QMatrix4x4 projectionMatrix, QMatrix4x4 viewMatrix, QOpenGLShaderProgram * program);

private:
    MyMesh mesh;
    QVector3D position;
    QOpenGLBuffer vbo_point;
    QOpenGLBuffer vbo_line;
    QOpenGLBuffer vbo_face;

    int vert_to_draw;
    int face_to_draw;
    int edge_to_draw;

};

#endif // MESH_H
