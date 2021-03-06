#ifndef MESH_H
#define MESH_H

#include<MeshFormat.h>
#include<QOpenGLShaderProgram>
#include<QOpenGLBuffer>
#include<QGLWidget>
#include<QVector>
#include<QMatrix4x4>
#include<iostream>

#include "btBulletDynamicsCommon.h"

class Mesh
{
public:
    Mesh(MyMesh mesh, QVector3D position);
    Mesh(QVector<MyMesh::Point> points, QVector3D position);
    Mesh(QVector<MyMesh::Point> points, QVector<QVector<int>> faces, QVector3D position);
    Mesh();

    MyMesh compute_bounding_box();
    QVector3D compute_barycentre();
    void resetAllColorsAndThickness(MyMesh* _mesh);

    // Setters
    void set_faces_visible(bool s);
    void set_edges_visible(bool s);
    void set_vertices_visible(bool s);
    void set_positionX(float x);
    void set_positionY(float y);
    void set_positionZ(float z);
    void set_numObj(int i);

    // Getters
    QVector3D get_position();
    MyMesh::Point get_min_bbox();
    MyMesh::Point get_max_bbox();
    QOpenGLBuffer get_vbo();

    int get_numObj();
    // Changer la couleur
    void color_point_by_id(int id, int r, int g, int b);
    void color_point(VertexHandle vh, int r, int g, int b);
    void color_face_by_id(int id, int r, int g, int b);
    void color_face(FaceHandle fh, int r, int g, int b);
    void color_edge_by_id(int id, int r, int g, int b);
    void color_edge(EdgeHandle eh, int r, int g, int b);

    void color_all_faces(int r, int g, int b);
    void color_all_edges(int r, int g, int b);
    void color_all_points(int r, int g, int b);

    // Changer la taille
    void thickness_point_by_id(int id, float size);
    void thickness_point(VertexHandle vh, float size);
    void thickness_edge_by_id(int id, float size);
    void thickness_edge(EdgeHandle eh, float size);

    void set_thickness_all_edges(float f);
    void set_thickness_all_points(float f);

    // Gerer l'affichage
    void destroy_vbos();
    void load_data();
    void draw(QMatrix4x4 projectionMatrix, QMatrix4x4 viewMatrix, QOpenGLShaderProgram * program);
    void anime(btDiscreteDynamicsWorld *dynamicWorld);

    MyMesh get_mesh();

private:

    void build_mesh(QVector<MyMesh::Point> points, QVector<QVector<int>> faces);
    void compute_min_max_box();

    MyMesh mesh;
    QVector3D position;
    QOpenGLBuffer vbo_point;
    QOpenGLBuffer vbo_line;
    QOpenGLBuffer vbo_face;

    int vert_to_draw;
    int face_to_draw;
    int edge_to_draw;

    bool show_faces;
    bool show_edges;
    bool show_points;

    float thick_edge;
    float thick_point;
    MyMesh::Point min;
    MyMesh::Point max;
    int num_obj;
};

#endif // MESH_H
