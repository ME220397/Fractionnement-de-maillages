#include "geometry.h"
#include <QMatrix4x4>

Geometry::Geometry()
{

}


MyMesh::Point Geometry::get_midpoint(MyMesh::Point A, MyMesh::Point B)
{
    MyMesh::Point I;//Le milieu
    I[0] = (A[0] + B[0])/2;
    I[1] = (A[1] + B[1])/2;
    I[2] = (A[2] + B[2])/2;
    return I;
}

MyMesh::Point Geometry::get_vect(MyMesh::Point A, MyMesh::Point B)
{
    MyMesh::Point AB;//Le vecteur allant de A vers B
    AB[0] = B[0]-A[0];
    AB[1] = B[1]-A[1];
    AB[2] = B[2]-A[2];
    return AB;
}

std::vector<MyMesh::Point> Geometry::get_mediator_plan(MyMesh::Point A, MyMesh::Point B)
{
    MyMesh::Point I; //Le milieu de AB
    I = get_midpoint(A, B);

    MyMesh::Point AB; //Le vecteur allant de A vers B
    AB = get_vect(A, B);

    MyMesh::Point U; //Le premier vecteur du plan mediateur
    MyMesh::Point V; //Le second vecteur du plan mediateur

    QMatrix4x4 Rx; //Creation de la matrice de rotation autour de l'axe x de 90 degres
    Rx.setToIdentity();
    Rx.rotate(90, QVector3D(1, 0, 0));

    QMatrix4x4 Ry; //Creation de la matrice de rotation autour de l'axe y de 90 degres
    Ry.setToIdentity();
    Ry.rotate(90, QVector3D(0, 1, 0));

    U = AB * Rx;
    V = AB * Ry;

    std::vector<MyMesh::Point> med_plan;
    med_plan.pushback(I);
    med_plan.pushback(U);
    med_plan.pushback(V);
    med_plan.pushback(AB) // On garde la normale au plan pour vérifier plus simplement si deux plans s'intersectent
    return med_plan;
}


bool Geometry::colinear(MyMesh::Point A, MyMesh::Point B)
{
    if(B[0] != 0, B[1] != 0, B[2] != 0)
    {

    }
}


bool Geometry::is_intersected(std::vector<MyMesh::Point> P, std::vector<MyMesh::Point> Q)//Verification si une coordonées des normales vaut 0 à faire
{
    if(P.at(3)[0]/Q.at(3)[0] == P.at(3)[1]/Q.at(3)[1] && P.at(3)[0]/Q.at(3)[0] == P.at(3)[2]/Q.at(3)[2])
    {
        return false;
    }
    return true;
}











