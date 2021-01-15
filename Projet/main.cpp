// Basé sur :
// CC-BY Edouard.Thiel@univ-amu.fr - 22/01/2019

#include "princ.h"
#include <QApplication>

#include "geometry.h"

void test_get_mediator_plan()
{
    //test du plan médiateur
    Geometry g;
    MyMesh::Point A;
    A[0] = 1; A[1] = 1; A[2] = 1;
    MyMesh::Point B;
    B[0] = 2; B[1] = 7; B[2] = 18;

    Plane P(A, B, g.get_vect(A,B));
    P = g.get_mediator_plan(A,B);
    qDebug()<< P.get_position()[0] << P.get_position()[1] <<P.get_position()[2] << Qt::endl;
    qDebug()<< g.get_vect(A,B)[0] << g.get_vect(A,B)[1] << g.get_vect(A,B)[2] << Qt::endl ;
    qDebug()<< P.get_u()[0] << P.get_u()[1] <<P.get_u()[2] << Qt::endl;
    qDebug()<< P.get_v()[0] << P.get_v()[1] <<P.get_v()[2] << Qt::endl;
}

void test_get_intersection_line()
{
    //test recuperation droite intersection
    Geometry g;

    MyMesh::Point A(1,2,2);
    MyMesh::Point u(1,1,0);
    MyMesh::Point v(1,2,0);
    Plane P(A,u,v);

    MyMesh::Point B(-2,1,-2);
    MyMesh::Point w(-1,-1,-1);
    MyMesh::Point t(1,-1,1);
    Plane Q(B,w,t);

    if(g.intersected(P,Q) == true)
        qDebug() << "Problème de fonction intersected" << Qt::endl;

    Line d(A, u);
    d = g.get_intersection_line(P,Q);
    qDebug() << d.get_position()[0] << d.get_position()[1] << d.get_position()[2] << Qt::endl;
    qDebug() << d.get_u()[0] << d.get_u()[1] << d.get_u()[2] << Qt::endl;
}


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Princ w;
    w.show();

    //test_get_mediator_plan();
    //test_get_intersection_line();

    return a.exec();
}
