// Basé sur :
// CC-BY Edouard.Thiel@univ-amu.fr - 22/01/2019

#include "princ.h"
#include "geometry.h"
#include "plane.h"
#include "voronoi.h"
#include <QApplication>
#include <QVector>

void test_line_intersection(){
    Line l1(MyMesh::Point(0, 0, 0), MyMesh::Point(1, 0, 0));
    Line l2(MyMesh::Point(0, -10, 0), MyMesh::Point(0,1,0));

    bool intersected = Geometry::intersected(l1, l2);
    assert(intersected);

    Line l3(MyMesh::Point(0, 0, 0), MyMesh::Point(1, 0, 0));
    Line l4(MyMesh::Point(0, -10, 0), MyMesh::Point(-1, 0, 0));

    intersected = Geometry::intersected(l3, l4);
    assert(!intersected);

    Line l5(MyMesh::Point(0, 0, 0), MyMesh::Point(0, 1, 1));
    Line l6(MyMesh::Point(0, 1, 0), MyMesh::Point(0,2,2));

    intersected = Geometry::intersected(l5, l6);
    assert(!intersected);

    Line l7(MyMesh::Point(1, 1, 0), MyMesh::Point(2, 2, 0));
    Line l8(MyMesh::Point(4, 2, 0), MyMesh::Point(-3,0,0));

    intersected = Geometry::intersected(l7, l8);
    assert(intersected);

    Line l9(MyMesh::Point(2, 2, 2), MyMesh::Point(1, 1, 1));
    Line l10(MyMesh::Point(0, 2, 0), MyMesh::Point(0,1,2));

    intersected = Geometry::intersected(l9, l10);
    assert(intersected);
}

void test_mesh_creation(){
   QVector<MyMesh::Point> points;
   QVector<QVector<int>> faces;

   // On créer les points
   points.append(MyMesh::Point(-1,-1,-1));
   points.append(MyMesh::Point(1,-1,-1));
   points.append(MyMesh::Point(1,1,-1));
   points.append(MyMesh::Point(-1,1,-1));
   points.append(MyMesh::Point(1,1,1));
   points.append(MyMesh::Point(1,-1,1));
   points.append(MyMesh::Point(-1,-1,1));
   points.append(MyMesh::Point(-1,1,1));

   // on créer les faces
   QVector<int> face0;
   face0.append(0);
   face0.append(1);
   face0.append(2);
   faces.append(face0);

   QVector<int> face1;
   face1.append(2);
   face1.append(3);
   face1.append(0);
   faces.append(face1);

   QVector<int> face2;
   face2.append(1);
   face2.append(5);
   face2.append(4);
   faces.append(face2);

   QVector<int> face3;
   face3.append(4);
   face3.append(2);
   face3.append(1);
   faces.append(face3);

   QVector<int> face4;
   face4.append(4);
   face4.append(5);
   face4.append(6);
   faces.append(face4);

   QVector<int> face5;
   face5.append(6);
   face5.append(7);
   face5.append(4);
   faces.append(face5);

   QVector<int> face6;
   face6.append(7);
   face6.append(6);
   face6.append(0);
   faces.append(face6);

   QVector<int> face7;
   face7.append(0);
   face7.append(3);
   face7.append(7);
   faces.append(face7);

   QVector<int> face8;
   face8.append(2);
   face8.append(4);
   face8.append(7);
   faces.append(face8);

   QVector<int> face9;
   face9.append(7);
   face9.append(3);
   face9.append(2);
   faces.append(face9);

   QVector<int> face10;
   face10.append(1);
   face10.append(5);
   face10.append(6);
   faces.append(face10);

   QVector<int> face11;
   face11.append(6);
   face11.append(0);
   face11.append(1);
   faces.append(face11);
   Mesh mesh(points, faces, QVector3D(0,0,0));
}

void test_plane_intersection(){
    Plane p1 = Plane(
                MyMesh::Point(0,0,0),
                MyMesh::Point(0,1,0),
                MyMesh::Point(0,0,1)
    );

    Plane p2 = Plane(
                MyMesh::Point(0,0,0),
                MyMesh::Point(0,1,0),
                MyMesh::Point(1,0,0)
    );

    bool intersected = Geometry::intersected(p1, p2);
    assert(intersected);

    Plane p3 = Plane(
                MyMesh::Point(1,1,0),
                MyMesh::Point(0,1,0),
                MyMesh::Point(1,0,0)
    );

    Plane p4 = Plane(
                MyMesh::Point(0,0,0),
                MyMesh::Point(0,1,0),
                MyMesh::Point(1,0,0)
    );

    intersected = Geometry::intersected(p3, p4);
    assert(!intersected);

}

void test_get_mediator_plan()
{
    //test du plan médiateur
    Geometry g;
    MyMesh::Point A;
    A[0] = 0; A[1] = 0; A[2] = 0;
    MyMesh::Point B;
    B[0] = 1; B[1] = -1; B[2] = 1;

    Plane P(A, B, g.get_vect(A,B));
    P = g.get_mediator_plane(A,B);
    qDebug()<< P.get_position()[0] << P.get_position()[1] <<P.get_position()[2] << Qt::endl;
    //qDebug()<< g.get_vect(A,B)[0] << g.get_vect(A,B)[1] << g.get_vect(A,B)[2] << Qt::endl ;
    qDebug()<< P.get_u()[0] << P.get_u()[1] <<P.get_u()[2] << Qt::endl;
    qDebug()<< P.get_v()[0] << P.get_v()[1] <<P.get_v()[2] << Qt::endl;
    qDebug() << "OK";
}//qDebug() << "TRES CHIANT";

void test_get_intersection_line()
{
    //test recuperation droite intersection
    Geometry g;

    MyMesh::Point A(-0.5,-0.5,0.5);
    MyMesh::Point u(-1,-1,-1);
    MyMesh::Point v(1,-1,1);
    Plane P(A,u,v);

    MyMesh::Point B(-1,-1,1);
    MyMesh::Point w(0,2,-2);
    MyMesh::Point t(0,0,-2);
    Plane Q(B,w,t);

    if(g.intersected(P,Q) == false)
        qDebug() << "Problème de fonction intersected";

    Line d(A, u);
    d = g.get_intersection_line(P,Q);
    qDebug() << d.get_position()[0] << d.get_position()[1] << d.get_position()[2];
    qDebug() << d.get_u()[0] << d.get_u()[1] << d.get_u()[2];
    qDebug() << "OK";
}


void test_get_intersection_point(){
    Geometry g;

    MyMesh::Point A(-1,0,-1);
    MyMesh::Point u(0,-1,1);
    Line d1(A, u);

    MyMesh::Point B(-1,1,1);
    MyMesh::Point v(0,1,0);
    Line d2(B, v);

    MyMesh::Point pt_inter = g.get_intersection_point(d1, d2);
    qDebug() << pt_inter[0] << pt_inter[1] << pt_inter[2] << Qt::endl;
}


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Princ w;
    w.show();
    //test_line_intersection();
    //test_mesh_creation();
    //test_plane_intersection();
    test_get_mediator_plan();
    //test_get_intersection_line();
    //test_get_intersection_point();
    return a.exec();
}
