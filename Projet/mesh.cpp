#include "mesh.h"
#include "geometry.h"

Mesh::Mesh(MyMesh mesh, QVector3D position)
{
    this->mesh = mesh;
    this->position = position;
    this->show_faces = true;
    this->show_edges = true;
    this->show_points = true;
    this->thick_edge = 1.0f;
    this->thick_point = 1.0f;
    resetAllColorsAndThickness(&this->mesh);
}

Mesh::Mesh(QVector<MyMesh::Point> points, QVector3D position){
    for(MyMesh::Point p : points){
        this->mesh.add_vertex(p);
    }
    this->position = position;
    this->show_faces = false;
    this->show_edges = false;
    this->show_points = true;
    this->thick_edge = 1.0f;
    this->thick_point = 1.0f;
    resetAllColorsAndThickness(&this->mesh);
}

Mesh::Mesh(QVector<MyMesh::Point> points, QVector<QVector<int>> faces, QVector3D position){
    // Build mesh
    build_mesh(points, faces);
    this->position = position;
    this->show_points = true;
    this->show_faces = true;
    this->show_edges = true;
    this->thick_point = 1.0f;
    this->thick_edge = 1.0f;
    resetAllColorsAndThickness(&this->mesh);
}

// Setters

void Mesh::set_faces_visible(bool s){
    show_faces = s;
}

void Mesh::set_edges_visible(bool s){
    show_edges = s;
}

void Mesh::set_vertices_visible(bool s){
    show_points = s;
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

void Mesh::color_all_faces(int r, int g, int b){
    MyMesh *_mesh = &mesh;
    for(MyMesh::FaceIter f_it = _mesh->faces_begin(); f_it != _mesh->faces_end(); f_it++){
        color_face(*f_it, r, g, b);
    }
}

void Mesh::color_all_edges(int r, int g, int b){
    MyMesh *_mesh = &mesh;
    for(MyMesh::EdgeIter e_it = _mesh->edges_begin(); e_it != _mesh->edges_end(); e_it++){
        color_edge(*e_it, r, g, b);
    }
}

void Mesh::color_all_points(int r, int g, int b){
    MyMesh *_mesh = &mesh;
    for(MyMesh::VertexIter v_it = _mesh->vertices_begin(); v_it != _mesh->vertices_end(); v_it++){
        color_point(*v_it, r, g, b);
    }
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

void Mesh::set_thickness_all_edges(float f){
    MyMesh *_mesh = &mesh;
    thick_edge = f;
    for(MyMesh::EdgeIter e_it = _mesh->edges_begin(); e_it != _mesh->edges_end(); e_it++){
        thickness_edge(*e_it, f);
    }
}

void Mesh::set_thickness_all_points(float f){
    MyMesh *_mesh = &mesh;
    thick_point = f;
    for(MyMesh::VertexIter v_it = _mesh->vertices_begin(); v_it != _mesh->vertices_end(); v_it++){
        thickness_point(*v_it, f);
    }
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
        face_vertices[cpt] = float(a[0]);face_vertices[cpt+1] = float(a[1]);face_vertices[cpt+2] = float(a[2]);
        face_vertices[cpt+3] = float(b[0]);face_vertices[cpt+4] = float(b[1]);face_vertices[cpt+5] = float(b[2]);
        face_vertices[cpt+6] = float(c[0]);face_vertices[cpt+7] = float(c[1]);face_vertices[cpt+8] = float(c[2]);

        // On recupere les couleurs associé aux sommets
        MyMesh::Color col_v1 = _mesh->color(*f_it);
        MyMesh::Color col_v2 = _mesh->color(*f_it);
        MyMesh::Color col_v3 = _mesh->color(*f_it);

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
        vbo_face.allocate(face_data_vertices.constData(), face_data_vertices.count() * int(sizeof(GLfloat)));
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
        MyMesh::Color col_v1 = _mesh->color(*e_it);
        MyMesh::Color col_v2 = _mesh->color(*e_it);

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
        vbo_line.allocate(edge_data_vert.constData(), edge_data_vert.count() * int(sizeof(GLfloat)));
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

        cpt += 3;
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
        vbo_point.allocate(vert_data.constData(), vert_data.count() * int(sizeof(GLfloat)));
    }
}

void Mesh::draw(QMatrix4x4 projectionMatrix, QMatrix4x4 viewMatrix, QOpenGLShaderProgram *program){
    if(edge_to_draw > 0 && show_edges){
        vbo_line.bind();
        program->bind();
        QMatrix4x4 modelLineMatrix;
        modelLineMatrix.translate(position);

        program->setUniformValue("projectionMatrix", projectionMatrix);
        program->setUniformValue("viewMatrix", viewMatrix);
        program->setUniformValue("modelMatrix", modelLineMatrix);
        program->setUniformValue("size", 1.0f);

        program->setAttributeBuffer("in_position", GL_FLOAT ,0, 3,  6*sizeof(GLfloat));
        program->setAttributeBuffer("col", GL_FLOAT, 3*sizeof (GLfloat), 3, 6*sizeof(GLfloat));

        program->enableAttributeArray("in_position");
        program->enableAttributeArray("col");

        glLineWidth(thick_edge); //
        glDrawArrays(GL_LINES, 0, edge_to_draw*2);

        program->disableAttributeArray("in_position");
        program->disableAttributeArray("col");

        program->release();
    }

    if(face_to_draw > 0 && show_faces){
        vbo_face.bind();
        program->bind();
        QMatrix4x4 modelFaceMatrix;
        modelFaceMatrix.translate(position);

        program->setUniformValue("projectionMatrix", projectionMatrix);
        program->setUniformValue("viewMatrix", viewMatrix);
        program->setUniformValue("modelMatrix", modelFaceMatrix);
        program->setUniformValue("size", 1.0f);

        program->setAttributeBuffer("in_position", GL_FLOAT ,0, 3,  6*sizeof(GLfloat));
        program->setAttributeBuffer("col", GL_FLOAT, 3*sizeof (GLfloat), 3, 6*sizeof(GLfloat));

        program->enableAttributeArray("in_position");
        program->enableAttributeArray("col");

        glDrawArrays(GL_TRIANGLES, 0, face_to_draw*3);

        program->disableAttributeArray("in_position");
        program->disableAttributeArray("col");

        program->release();
    }

    if(vert_to_draw > 0 && show_points){
        vbo_point.bind();
        program->bind();
        QMatrix4x4 modelVertMatrix;
        modelVertMatrix.translate(position);

        program->setUniformValue("projectionMatrix", projectionMatrix);
        program->setUniformValue("viewMatrix", viewMatrix);
        program->setUniformValue("modelMatrix", modelVertMatrix);
        program->setUniformValue("size", 1.0F);

        program->setAttributeBuffer("in_position", GL_FLOAT ,0, 3,  6*sizeof(GLfloat));
        program->setAttributeBuffer("col", GL_FLOAT, 3*sizeof (GLfloat), 3, 6*sizeof(GLfloat));

        program->enableAttributeArray("in_position");
        program->enableAttributeArray("col");

        glPointSize(thick_point);
        glDrawArrays(GL_POINTS, 0, vert_to_draw);

        program->disableAttributeArray("in_position");
        program->disableAttributeArray("col");

        program->release();
    }
}

MyMesh Mesh::compute_bounding_box(){
    MyMesh *_mesh = &mesh;
    MyMesh::Point min, max;
    MyMesh bbox;
    if((int)_mesh->n_vertices() > 0){
        VertexHandle first = _mesh->vertex_handle(0);
        min = _mesh->point(first); max = _mesh->point(first);

        for(MyMesh::VertexIter v_it = _mesh->vertices_begin(); v_it != _mesh->vertices_end(); v_it++ ){
            MyMesh::Point current = _mesh->point(*v_it);

            if(min[0] > current[0])
                min[0] = current[0];
            if(min[1] > current[1])
                min[1] = current[1];
            if(min[2] > current[2])
                min[2] = current[2];

            if(max[0] < current[0])
                max[0] = current[0];
            if(max[1] < current[1])
                max[1] = current[1];
            if(max[2] < current[2])
                max[2] = current[2];
        }

        //Creation des points de la bounding box
        MyMesh::VertexHandle vhandle[8];

        vhandle[0] = bbox.add_vertex(MyMesh::Point(min[0], min[1], max[2])); // -1 -1 1
        vhandle[1] = bbox.add_vertex(MyMesh::Point(max[0], min[1], max[2])); // 1 -1 1
        vhandle[2] = bbox.add_vertex(max); // 1 1 1
        vhandle[3] = bbox.add_vertex(MyMesh::Point(min[0], max[1], max[2])); // -1 1 1
        vhandle[4] = bbox.add_vertex(min); // -1 -1 -1
        vhandle[5] = bbox.add_vertex(MyMesh::Point(max[0], min[1], min[2])); // 1 -1 -1
        vhandle[6] = bbox.add_vertex(MyMesh::Point(max[0], max[1], min[2])); // 1 1 -1
        vhandle[7] = bbox.add_vertex(MyMesh::Point(min[0], max[1], min[2])); // -1 1 -1

        std::vector<MyMesh::VertexHandle>  face_vhandles;

        // face 0 1 2
        face_vhandles.clear();
        face_vhandles.push_back(vhandle[0]);
        face_vhandles.push_back(vhandle[1]);
        face_vhandles.push_back(vhandle[2]);
        bbox.add_face(face_vhandles);

        // face 2 3 0
        face_vhandles.clear();
        face_vhandles.push_back(vhandle[2]);
        face_vhandles.push_back(vhandle[3]);
        face_vhandles.push_back(vhandle[0]);
        bbox.add_face(face_vhandles);

        // face 7 6 5
        face_vhandles.clear();
        face_vhandles.push_back(vhandle[7]);
        face_vhandles.push_back(vhandle[6]);
        face_vhandles.push_back(vhandle[5]);
        bbox.add_face(face_vhandles);

        // face 5 4 7
        face_vhandles.clear();
        face_vhandles.push_back(vhandle[5]);
        face_vhandles.push_back(vhandle[4]);
        face_vhandles.push_back(vhandle[7]);
        bbox.add_face(face_vhandles);

        // face 1 0 4
        face_vhandles.clear();
        face_vhandles.push_back(vhandle[1]);
        face_vhandles.push_back(vhandle[0]);
        face_vhandles.push_back(vhandle[4]);
        bbox.add_face(face_vhandles);

        // face 4 5 1
        face_vhandles.clear();
        face_vhandles.push_back(vhandle[4]);
        face_vhandles.push_back(vhandle[5]);
        face_vhandles.push_back(vhandle[1]);
        bbox.add_face(face_vhandles);

        // face 2 1 5
        face_vhandles.clear();
        face_vhandles.push_back(vhandle[2]);
        face_vhandles.push_back(vhandle[1]);
        face_vhandles.push_back(vhandle[5]);
        bbox.add_face(face_vhandles);

        // face 5 6 2
        face_vhandles.clear();
        face_vhandles.push_back(vhandle[5]);
        face_vhandles.push_back(vhandle[6]);
        face_vhandles.push_back(vhandle[2]);
        bbox.add_face(face_vhandles);

        // face 3 2 6
        face_vhandles.clear();
        face_vhandles.push_back(vhandle[3]);
        face_vhandles.push_back(vhandle[2]);
        face_vhandles.push_back(vhandle[6]);
        bbox.add_face(face_vhandles);

        // face 6 7 3
        face_vhandles.clear();
        face_vhandles.push_back(vhandle[6]);
        face_vhandles.push_back(vhandle[7]);
        face_vhandles.push_back(vhandle[3]);
        bbox.add_face(face_vhandles);

        // face 0 3 7
        face_vhandles.clear();
        face_vhandles.push_back(vhandle[0]);
        face_vhandles.push_back(vhandle[3]);
        face_vhandles.push_back(vhandle[7]);
        bbox.add_face(face_vhandles);

        // face 7 4 0
        face_vhandles.clear();
        face_vhandles.push_back(vhandle[7]);
        face_vhandles.push_back(vhandle[4]);
        face_vhandles.push_back(vhandle[0]);
        bbox.add_face(face_vhandles);
    }

    return bbox;
}

void Mesh::destroy_vbos(){
    vbo_face.destroy();
    vbo_line.destroy();
    vbo_point.destroy();
}

void Mesh::resetAllColorsAndThickness(MyMesh* _mesh)
{
    for (MyMesh::VertexIter curVert = _mesh->vertices_begin(); curVert != _mesh->vertices_end(); curVert++)
    {
        _mesh->data(*curVert).thickness = 1;
        _mesh->set_color(*curVert, MyMesh::Color(0, 0, 0));
    }

    for (MyMesh::FaceIter curFace = _mesh->faces_begin(); curFace != _mesh->faces_end(); curFace++)
    {
        _mesh->set_color(*curFace, MyMesh::Color(150, 150, 150));
    }

    for (MyMesh::EdgeIter curEdge = _mesh->edges_begin(); curEdge != _mesh->edges_end(); curEdge++)
    {
        _mesh->data(*curEdge).thickness = 1;
        _mesh->set_color(*curEdge, MyMesh::Color(0, 0, 0));
    }
}

void Mesh::build_mesh(QVector<MyMesh::Point> points, QVector<QVector<int>> faces){
    // On ajoute les points au maillage
    for(MyMesh::Point p : points)
        mesh.add_vertex(p);

    // Les Faces doivent être triangulaires
    for(QVector<int> face : faces){
       // On recupère les id des points
       int id0, id1, id2;
       id0 = face.at(0); id1 = face.at(1); id2 = face.at(2);

       // Trouvé un moyen de creer le Mesh
    }
}
