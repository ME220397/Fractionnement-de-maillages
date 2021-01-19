#include "voronoi.h"

Voronoi::Voronoi(QVector<MyMesh::Point> points, MyMesh mesh)
{
    this->points = points;
    this->mesh = mesh;
}

QVector<Plane> Voronoi::get_mediator_planes(MyMesh::Point p, Mesh mesh)
{
    MyMesh mymesh = mesh.get_mesh();//
    MyMesh *_mesh = &mymesh;
    QVector<Plane> planes;
    for(MyMesh::VertexIter v_it = _mesh->vertices_begin(); v_it != _mesh->vertices_end(); v_it++){
        planes.append(Geometry::get_mediator_plan(p, _mesh->point(*v_it))); //On ajoute les plans mediateurs entre les sommets parcourus et le point p
    }
    return planes;
}

QVector<Line> Voronoi::get_intersection_Line_mesh_plane(Plane p, QVector<Plane> planes_mesh, MyMesh::Point min, MyMesh::Point max)//renvoie les droites d intersection entre un plan p et les plan du mesh
{ // p sera un plan mediateur
    QVector<Line> inter_line;
    Line line(MyMesh::Point(0,0,0), MyMesh::Point(0,0,0));
    for(Plane q:planes_mesh){
        if(Geometry::intersected(p, q)){ //On verifie si les plans p et q s'intersectent
            line = Geometry::get_intersection_line(p, q); //On recupere la droite d'intersection entre les deux plans
            MyMesh::Point pos(line.get_position()[0], line.get_position()[1], line.get_position()[2]);


            //Verification de si la droite traverse bien le cube
            if(pos[0] <= max[0] && pos[1] <= max[1] && pos[2]<=max[2] && pos[0] >=min[0] && pos[1]>=min[1] && pos[2]>=min[2]){
                inter_line.append(line); //Ajout de la droite a la liste
            }
        }
    }
    return inter_line;
}

QVector<Mesh> Voronoi::compute_voronoi()
{
    //Recuperer les plans du mesh
    //Pour chaque point du nuage recuperer les plan mediateur avec les points du mesh qui le contient
    //intersection des plans et des droites obtenues
    //creation de nouveaux mesh dans un QVector<Mesh>

}
