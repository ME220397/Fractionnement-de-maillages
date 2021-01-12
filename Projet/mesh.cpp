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

// Gestion de l'affichage
void Mesh::load_data(){
    MyMesh *_mesh = &mesh;

    // On recupere la taille des sommets, arêtes et faces
    int n_faces = _mesh->n_faces();
    int n_edges = _mesh->n_edges();
    int n_vertices = _mesh->n_vertices();

    face_to_draw = n_faces;
    edge_to_draw = n_edges;
    vert_to_draw = n_vertices;

    // Chargement des faces
    GLfloat face_vertices[n_faces*3*3];
    GLfloat face_color[n_faces*3*3];
    int cpt = 0;
    for(MyMesh::FaceIter f_it = _mesh->faces_begin(); f_it != _mesh->faces_end(); f_it++){
        // On recupère les vertexhandle de la face
        HalfedgeHandle heh = _mesh->halfedge_handle(*f_it);
        HalfedgeHandle next = _mesh->next_halfedge_handle(heh);
        VertexHandle v1, v2, v3;

        v1 = _mesh->from_vertex_handle(heh);
        v2 = _mesh->to_vertex_handle(heh);
        v3 = _mesh->to_vertex_handle(next);

        // On recupere les points
        MyMesh::Point a,b,c;

        a = _mesh->point(v1);
        b = _mesh->point(v2);
        c = _mesh->point(v3);

        // On les ajoute à notre liste de sommets
        face_vertices[cpt] = a[0];face_vertices[cpt+1] = a[1];face_vertices[cpt+2] = a[2];
        face_vertices[cpt+3] = b[0];face_vertices[cpt+4] = b[1];face_vertices[cpt+5] = b[2];
        face_vertices[cpt+6] = c[0];face_vertices[cpt+7] = c[1];face_vertices[cpt+8] = c[2];

        // On recupere les couleurs associé aux sommets
        MyMesh::Color col_v1 = _mesh->color(v1);
        MyMesh::Color col_v2 = _mesh->color(v2);
        MyMesh::Color col_v3 = _mesh->color(v3);

        face_color[cpt] = float(col_v1[0])/255.f;face_color[cpt+1] = float(col_v1[1])/255.f;face_color[cpt+2] = float(col_v1[2])/255.f;
        face_color[cpt+3] = float(col_v2[0])/255.f;face_color[cpt+4] = float(col_v2[1])/255.f;face_color[cpt+5] = float(col_v2[2])/255.f;
        face_color[cpt+6] = float(col_v3[0])/255.f;face_color[cpt+7] = float(col_v3[1])/255.f;face_color[cpt+8] = float(col_v3[2])/255.f;

        cpt += 9;
    }

    QVector<GLfloat> face_data_vertices;
    for(int i=0; i<n_faces*3; i++){
        for(int j=0; j<3; j++){
            face_data_vertices.append(face_vertices[i*3+j]);
        }
        for(int j=0; j<3; j++){
            face_data_vertices.append(face_color[i*3+j]);
        }
    }

    if(n_faces > 0){
        vbo_face.create();
        vbo_face.bind();
        vbo_face.allocate(face_data_vertices.constData(), face_data_vertices.count() * sizeof(GLfloat));
    }

    // Chargement des arêtes
    GLfloat edge_vertices[n_edges*2*3];
    GLfloat edge_color[n_edges*2*3];
    cpt = 0;
    for(MyMesh::EdgeIter e_it = _mesh->edges_begin(); e_it != _mesh->edges_end(); e_it++){
        // On recupère les vertexHandle
        HalfedgeHandle heh = _mesh->halfedge_handle(*e_it, 0);

        VertexHandle v1, v2;
        v1 = _mesh->from_vertex_handle(heh);
        v2 = _mesh->to_vertex_handle(heh);

        // On recupère les sommets
        MyMesh::Point a,b;
        a = _mesh->point(v1);
        b = _mesh->point(v2);

        // On les ajoute à la liste
        edge_vertices[cpt] = a[0];edge_vertices[cpt+1] = a[1];edge_vertices[cpt+2] = a[2];
        edge_vertices[cpt+3] = b[0];edge_vertices[cpt+4] = b[1];edge_vertices[cpt+5] = b[2];

        // On récupère les couleurs
        MyMesh::Color col_v1 = _mesh->color(v1);
        MyMesh::Color col_v2 = _mesh->color(v2);

        edge_color[cpt] = float(col_v1[0])/255.f;edge_color[cpt+1] = float(col_v1[1])/255.f;edge_color[cpt+2] = float(col_v1[2])/255.f;
        edge_color[cpt+3] = float(col_v2[0])/255.f;edge_color[cpt+4] = float(col_v2[1])/255.f;edge_color[cpt+5] = float(col_v2[2])/255.f;

        cpt += 6;
    }

    QVector<GLfloat> edge_data_vert;
    for(int i=0; i<n_edges*2; i++){
        for(int j = 0; j<3; j++){
            edge_data_vert.append(edge_vertices[i*3+j]);
        }
        for(int j = 0; j<3; j++){
            edge_data_vert.append(edge_color[i*3+j]);
        }
    }

    if(n_edges > 0){
        vbo_line.create();
        vbo_line.bind();
        vbo_line.allocate(edge_data_vert.constData(), edge_data_vert.count() * sizeof(GLfloat));
    }
    // Chargement des points
    GLfloat vertices[n_vertices*3];
    GLfloat vert_color[n_vertices*3];
    cpt=0;
    for(MyMesh::VertexIter v_it = _mesh->vertices_begin(); v_it != _mesh->vertices_end(); v_it++){
        // on récupère le sommet
        MyMesh::Point a = _mesh->point(*v_it);
        vertices[cpt] = a[0];vertices[cpt+1] = a[1];vertices[cpt+2] = a[2];

        // On récupère la couleur
        MyMesh::Color col_v = _mesh->color(*v_it);
        vert_color[cpt] = float(col_v[0])/255.f;vert_color[cpt+1] = float(col_v[1])/255.f;vert_color[cpt+2] = float(col_v[2])/255.f;
    }

    QVector<GLfloat> vert_data;
    for(int i = 0; i<n_vertices; i++){
        for(int j=0; j<3; j++){
            vert_data.append(vertices[i*3+j]);
        }
        for(int j=0; j<3; j++){
            vert_data.append(vert_color[i*3+j]);
        }
    }

    if(n_vertices > 0){
        vbo_point.create();
        vbo_point.bind();
        vbo_point.allocate(vert_data.constData(), vert_data.count() * sizeof(GLfloat));
    }
}
