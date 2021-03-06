#include "voronoi.h"

Voronoi::Voronoi(QVector<MyMesh::Point> points, MyMesh mesh)
{
    this->points = points;
    this->mesh = mesh;
}

QVector<Plane> Voronoi::get_mediator_planes(MyMesh::Point p, Mesh mesh)
{
    MyMesh mymesh = mesh.get_mesh();
    MyMesh *_mesh = &mymesh;
    QVector<Plane> planes;
    for(MyMesh::VertexIter v_it = _mesh->vertices_begin(); v_it != _mesh->vertices_end(); v_it++){
        planes.append(Geometry::get_mediator_plane(p, _mesh->point(*v_it))); //On ajoute les plans mediateurs entre les sommets parcourus et le point p
    }
    return planes;
}

QVector<Line> Voronoi::get_intersection_Line_mesh_plane(Plane p, QVector<Plane> planes_mesh)//renvoie les droites d intersection entre un plan p et les plan du mesh
{ // p sera un plan mediateur
    /*qDebug() << min[0] << min[1] << min[2];
    qDebug() << max[0] << max[1] << max[2];*/
    QVector<Line> inter_line;
    Line line(MyMesh::Point(0,0,0), MyMesh::Point(0,0,0));

    //qDebug() <<"p " << Geometry::to_Qvector3D(p.get_position())<< Geometry::to_Qvector3D(p.get_u()) <<Geometry::to_Qvector3D(p.get_v());
    for(Plane q:planes_mesh){
        //qDebug() <<"q " << Geometry::to_Qvector3D(q.get_position()) << Geometry::to_Qvector3D(q.get_u()) <<Geometry::to_Qvector3D(q.get_v());

        if(Geometry::intersected(p, q)){ //On verifie si les plans p et q s'intersectent
            //qDebug() << "Is intersected ";
            line = Geometry::get_intersection_line(p, q); //On recupere la droite d'intersection entre les deux plans
            //qDebug() << "pos " <<Geometry::to_Qvector3D(line.get_position()) ;
            //qDebug() << "u " << Geometry::to_Qvector3D(line.get_u()) ;
            //MyMesh::Point pos(line.get_position()[0], line.get_position()[1], line.get_position()[2]);

            //Verification de si la droite traverse bien le cube
            //if(pos[0] <= max[0] && pos[1] <= max[1] && pos[2]<=max[2] && pos[0] >=min[0] && pos[1]>=min[1] && pos[2]>=min[2]){
            inter_line.append(line); //Ajout de la droite a la liste
                //qDebug() << "oui";



        }
        //qDebug() << "BOUCLE" ;

    }
    //qDebug() <<  inter_line.size() <<"END BOUCLE";
    return inter_line;
}

QVector<MyMesh::Point> Voronoi::get_points(QVector<Line> lines){
    QVector<MyMesh::Point> points;
    points.append(Geometry::get_intersection_point(lines[0], lines[1]));

    points.append(Geometry::get_intersection_point(lines[1], lines[2]));

    points.append(Geometry::get_intersection_point(lines[0], lines[2]));
    //qDebug() << Geometry::to_Qvector3D(lines[0].get_position()) << Geometry::to_Qvector3D(lines[0].get_u());
    //qDebug()<<Geometry::to_Qvector3D(lines[2].get_position()) << Geometry::to_Qvector3D(lines[2].get_u());
    return points;
}

QVector<Mesh> Voronoi::extract_meshes(QVector<QVector<MyMesh::Point> > v_points, MyMesh *_mesh){
    QVector<Mesh> meshes;
    for(int i = 0; i<v_points.size(); i++){
        QVector<MyMesh::Point> mesh_points;
        QVector<QVector<int>> faces;
        MyMesh::Point p0 = _mesh->point(_mesh->vertex_handle(i));
        MyMesh::Point a, b, c;
        a = v_points[i].at(0);
        b = v_points[i].at(1);
        c = v_points[i].at(2);

        mesh_points.append(p0);
        mesh_points.append(a);
        mesh_points.append(b);
        mesh_points.append(c);

        faces.append({1, 1, 0});
        faces.append({2, 3, 0});
        faces.append({1, 3, 0});
        faces.append({1, 3, 2});

        MyMesh::Point B = Geometry::compute_barycentre(mesh_points);
        meshes.append(Mesh(mesh_points, faces, Geometry::to_Qvector3D(B)));
    }

    // On extraite le dernier mesh
    /*QVector<MyMesh::Point> pts;
    QVector<QVector<int>> faces;
    for(int i = 0; i<v_points.size(); i++){
        QVector<int> face;
        MyMesh::Point a, b, c;
        a = v_points[i].at(0);
        b = v_points[i].at(1);
        c = v_points[i].at(2);

        if(!pts.contains(a))
            pts.append(a);
        if(!pts.contains(b))
            pts.append(b);
        if(!pts.contains(c))
            pts.append(c);

        face.append(pts.indexOf(a));
        face.append(pts.indexOf(b));
        face.append(pts.indexOf(c));

        faces.append(face);
    }*/
    return meshes;
}

QVector<Mesh> Voronoi::compute_voronoi(Mesh mesh, QVector<MyMesh::Point> points)
{
    //Recuperer les plans du mesh
    QVector<QVector<Plane>> planes = Geometry::get_planes(mesh);

    //Pour chaque point du nuage recuperer les plan mediateur avec les points du mesh qui le contient
    MyMesh mymesh = mesh.get_mesh();
    MyMesh *_mesh = &mymesh;
    QVector<Plane> med_planes;
    //int cpt = 0;
    for(MyMesh::Point p:points){
        //qDebug() << "points :" << Geometry::to_Qvector3D(p);
        for(MyMesh::VertexIter v_it = _mesh->vertices_begin(); v_it != _mesh->vertices_end(); v_it++){
            med_planes.append(Geometry::get_mediator_plane(p, _mesh->point(*v_it)));
            //qDebug() << Geometry::to_Qvector3D(med_planes[cpt++].get_position());
        }
    }
    //intersection des plans obtenues

    QVector<QVector<Line>> lines;
    QVector<Line> line;
    for(int i=0; i< med_planes.size(); i++){
        line = get_intersection_Line_mesh_plane(med_planes[i], planes[i]);

        //qDebug() << line.size();
        /*for(Line l:line)
            qDebug() << Geometry::to_Qvector3D(l.get_position());*/
        lines.append(line);
        line.clear();
    }
    for(MyMesh::VertexIter v_it=_mesh->vertices_begin(); v_it!=_mesh->vertices_end(); v_it++){
        VertexHandle vh = *v_it;
        //qDebug() << vh.idx();
    }
    //Intersection des droites
    QVector<QVector<MyMesh::Point>> inter_points;
    for(QVector<Line> l:lines){
        inter_points.append(get_points(l));
    }
    for(QVector<MyMesh::Point> pts:inter_points){
        for(MyMesh::Point pt:pts)
        {
            //qDebug() << Geometry::to_Qvector3D(pt);
        }
        //qDebug() << "Fin line";
    }
    //qDebug() << "Boucle line";
    //creation de nouveaux mesh dans un QVector<Mesh>

}
