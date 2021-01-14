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

    /*U = AB * Rx;
    V = AB * Ry;

    std::vector<MyMesh::Point> med_plan;
    med_plan.pushback(I);
    med_plan.pushback(U);
    med_plan.pushback(V);
    med_plan.pushback(AB) // On garde la normale au plan pour vérifier plus simplement si deux plans s'intersectent
    return med_plan;*/
}

bool Geometry::is_intersected(std::vector<MyMesh::Point> P, std::vector<MyMesh::Point> Q)
{
    if(P.at(3)[0]/Q.at(3)[0] == P.at(3)[1]/Q.at(3)[1] && P.at(3)[0]/Q.at(3)[0] == P.at(3)[2]/Q.at(3)[2])
    {
        return false;
    }
    return true;
}

bool Geometry::intersected(Plane p1, Plane p2){
    // On veut savoir les deux plans donnés en arguments s'intersectent
    // Un plan Plane est données par une position et deux vecteurs directeurs.

    // On va donc premierement se placer dans un repere local ayant pour origine la position de p1
    // Pour cela, on récupère la matrice de changment de base

    QMatrix4x4 M = change_of_base(p1);

    bool est_coplanaire = true;

    // On utilise le vecteur u du plan p2 pour creer un quatrieme point partant de la position du plan p1
    // et un cinquieme point à partir du vecteur v de p2
    // On verifie si le polygone ainsi formé est coplanaire
    QVector3D v0 = to_Qvector3D(p1.get_position());
    QVector3D v1 = to_Qvector3D(p1.get_u());
    QVector3D v2 = to_Qvector3D(p1.get_v());
    QVector3D v3 = v0 + to_Qvector3D(p2.get_u());
    QVector3D v4 = v0 + to_Qvector3D(p2.get_v());

    QVector<QVector3D> v_local;
    v_local.append(M * v0);
    v_local.append(M * v1);
    v_local.append(M * v2);
    v_local.append(M * v3);
    v_local.append(M * v4);

    for(QVector3D v : v_local){
        if(v[2] != 0)
            est_coplanaire = false;
    }

    return est_coplanaire;
}

QVector3D Geometry::to_Qvector3D(MyMesh::Point p){
    float x, y ,z;
    x = float(p[0]); y = float(p[1]); z = float(p[2]);

    return QVector3D(x, y, z);
}

QVector3D Geometry::get_normal(Plane p){
    QVector3D u_prime = to_Qvector3D(p.get_u());
    QVector3D v_prime = to_Qvector3D(p.get_v());
    QVector3D w = QVector3D::crossProduct(u_prime ,v_prime);

    return w.normalized();
}

QMatrix4x4 Geometry::change_of_base(Plane p){
    QVector3D u_prime = to_Qvector3D(p.get_u());
    QVector3D w = get_normal(p);                   // Vecteur normal au plan
    QVector3D u = u_prime.normalized();             // Vecteur directeur du                               // Vecteur normal au plan
    QVector3D v = QVector3D::crossProduct(w, u);    // Vecteur directeur du plan
    QVector3D position = to_Qvector3D(p.get_position());

    float Px, Py, Pz;
    Px = - QVector3D::dotProduct(position, u);
    Py = - QVector3D::dotProduct(position, v);
    Pz = - QVector3D::dotProduct(position, w);

    // On récupère la matrice de changment de base
    QMatrix4x4 M;

    M(0, 0) = u.x(); M(0, 1) = u.y(); M(0, 2) = u.z(); M(0, 3) = Px;
    M(1, 0) = v.x(); M(1, 1) = v.y(); M(1, 2) = v.z(); M(1, 3) = Py;
    M(2, 0) = w.x(); M(2, 1) = w.y(); M(2, 2) = w.z(); M(2, 3) = Pz;
    M(3, 0) = 0; M(3, 1) = 0; M(3, 2) = 0; M(3, 3) = 1;

    return M;
}







