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

Plane Geometry::get_mediator_plan(MyMesh::Point A, MyMesh::Point B)
{
    MyMesh::Point I; //Le milieu de AB
    I = get_midpoint(A, B);

    MyMesh::Point AB; //Le vecteur allant de A vers B
    AB = get_vect(A, B);

    QMatrix4x4 Rx; //Creation de la matrice de rotation autour de l'axe x de 90 degres
    Rx.setToIdentity();
    Rx.rotate(90, QVector3D(1, 0, 0));

    MyMesh::Point u;
    MyMesh::Point v;

    QMatrix4x4 Ry; //Creation de la matrice de rotation autour de l'axe y de 90 degres
    Ry.setToIdentity();
    Ry.rotate(90, QVector3D(0, 1, 0));

    QMatrix4x4 Rz; //Creation de la matrice de rotation autour de l'axe y de 90 degres
    Rz.setToIdentity();
    Rz.rotate(90, QVector3D(0, 0, 1));

    if(AB[0] == 0 && AB[1] == 0)
    {
        u[0]=AB[2]; u[1] = 0; u[2] = 0;
        v[0] = 0; v[1] = AB[2] ; v[2] = 0;
    }
    else if(AB[0] == 0 && AB[2] == 0)
    {
        u[0]=AB[1]; u[1] = 0; u[2] = 0;
        v[0] = 0; v[1] = 0 ; v[2] = AB[1];
    }
    else if(AB[1] == 0 && AB[2] == 0)
    {
        u[0]=0; u[1] = 0; u[2] = AB[0];
        v[0] = 0; v[1] = AB[0] ; v[2] = 0;
    }
    else
    {
        QVector3D U = Rx * to_Qvector3D(AB);
        QVector3D V = Ry * to_Qvector3D(AB);
        u = to_point(U);
        v = to_point(V);
        if(u==v){

            V = Rz * to_Qvector3D(AB);
            v = to_point(V);
        }
    }
    Plane P(I, MyMesh::Point(u[0], u[1], u[2]), MyMesh::Point(v[0], v[1], v[2]));
    return P;
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

     // On verifie si le polygone ainsi formé est planaire
    for(QVector3D v : v_local){
        if(v[2] != 0)
            est_coplanaire = false;
    }

    return est_coplanaire?false:true;
}

QVector3D Geometry::to_Qvector3D(MyMesh::Point p){
    float x, y ,z;
    x = float(p[0]); y = float(p[1]); z = float(p[2]);

    return QVector3D(x, y, z);
}

MyMesh::Point Geometry::to_point(QVector3D q){
    return MyMesh::Point(q[0], q[1], q[2]);
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

QMatrix4x4 Geometry::change_of_base(MyMesh::Point P, MyMesh::Point U, MyMesh::Point V){
    QVector3D u_prime = to_Qvector3D(U);
    QVector3D v_prime = to_Qvector3D(V);
    QVector3D w = QVector3D::crossProduct(u_prime, v_prime);
    w.normalize();                                  // Vecteur normal au plan
    QVector3D u = u_prime.normalized();             // Vecteur directeur du                               // Vecteur normal au plan
    QVector3D v = QVector3D::crossProduct(w, u);    // Vecteur directeur du plan
    QVector3D position = to_Qvector3D(P);

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

QVector4D Geometry::get_equation(Plane p)
{
    MyMesh::Point I; //Recuperation d'un point de p
    I = p.get_position();

    QVector3D n; //Recuperation de la normale
    n = get_normal(p);

    QVector4D eq_p(n[0], n[1], n[2],-(I[0]*n[0] + I[1]*n[1] + I[2]*n[2])); //coefficients de l'equation du plan p
    return eq_p;
}



bool Geometry::is_perpendicular(MyMesh::Point p, MyMesh::Point q)
{
    if(p[0]*q[0] + p[1]*q[1] + p[2]*q[2] == 0) //Verification du produit scalaire
    {
        return true; //Cas Perpendiculaire
    }
    return false;
}


Line Geometry::get_intersection_line(Plane p, Plane q)
{

    QVector4D eq_p = get_equation(p); //Equation cartesienne de p : ax + by + cz +d = 0
    float a, b, c, d;
    a = eq_p[0];
    b = eq_p[1];
    c = eq_p[2];
    d = eq_p[3];
    QVector3D N = get_normal(p); //Normale de p
    MyMesh::Point n;
    n = to_point(N);
    MyMesh::Point pos_q2; //Un point de q
    pos_q2 = q.get_position();
    MyMesh::Point pos_q; //Un deuxieme point de q
    pos_q = q.get_position();

    MyMesh::Point u_q; //Premier vecteur directeur de q
    u_q = q.get_u();

    if(is_perpendicular(n, u_q)) //Si la droite def par pos_q et u_q est parallele a p
    {
        pos_q2 += q.get_u();
        u_q = q.get_v(); //On prend le deuxieme vecteur directeur car on sait qu'au moins un de ces deux vecteurs coupe le plan p
    }
    else
        pos_q2 += q.get_v();


    float t; //coefficient de la droite : pos_q + t*u_q
    t = -(a*pos_q[0] + b*pos_q[1] + c*pos_q[2] + d)/(a*u_q[0] + b*u_q[1] + c*u_q[2]);
    //Le denominateur n'est pas nul car c'est le produit scalaire
    //entre la normale du plan p et la droite choisis non-perpendiculaire du plan q

    MyMesh::Point X;
    X[0] = pos_q[0] + t*u_q[0];
    X[1] = pos_q[1] + t*u_q[1];
    X[2] = pos_q[2] + t*u_q[2];

    t = -(a*pos_q2[0] + b*pos_q2[1] + c*pos_q2[2] + d)/(a*u_q[0] + b*u_q[1] + c*u_q[2]);

    MyMesh::Point Y;
    Y[0] = pos_q2[0] + t*u_q[0];
    Y[1] = pos_q2[1] + t*u_q[1];
    Y[2] = pos_q2[2] + t*u_q[2];


    MyMesh::Point XY; //On a defini la droite comme etant un point et un vecteur
    XY = get_vect(X, Y);

    Line intersection_line(X, XY);
    return intersection_line;
}

int Geometry::determinant(MyMesh::Point A, MyMesh::Point B, MyMesh::Point P){
    // Les points doivent etre dans le même plan. La coordonnée Z n'est pas utilisé.
    // Un fois que l'on a recuperé les points du triangle
    // On verifie qu'un point P se situe a gauche de AB
    /*
                | Ax Bx Px |        det > 0 -> P à gauche
          det = | Ay By Py |        det < 0 -> P à droite
                | 1  1  1  |        det = 0 -> P inclu dans le segment
                  | By Py |      | Ax Py |      | Ay Bx |
          det = Ax| 1  1  | - Bx | 1  1  | + Px | 1  1 |
     */
    int x = 0, y = 1;
    int det = A[x] * (B[y] - P[y]) - B[x] * (A[y] - P[y]) + P[x] * (A[y] - B[y]);

    return det;
}

bool Geometry::intersected(Line l1, Line l2){
    MyMesh::Point p = l1.get_position();
    MyMesh::Point u = l1.get_u();
    MyMesh::Point v = l2.get_u();

    QMatrix4x4 M = Geometry::change_of_base(p, u ,v);

    MyMesh::Point local_p = Geometry::to_point(M * Geometry::to_Qvector3D(p));
    MyMesh::Point local_u = Geometry::to_point(M * Geometry::to_Qvector3D(u));
    MyMesh::Point local_v = Geometry::to_point(M * Geometry::to_Qvector3D(v));

    MyMesh::Point q0 = local_p + local_u;
    MyMesh::Point q1 = local_p + local_v;

    int det = Geometry::determinant(local_p, q0, q1);

    if(det != 0){
        return true;
    }

    return false;
}


MyMesh::Point Geometry::get_intersection_point(Line d1, Line d2)
{
    MyMesh::Point A; //Un point de la droite d1
    A = d1.get_position();
    MyMesh::Point U; // Vecteur directeur de la droite d1
    U = d1.get_u();
    //Notation pour simplifier la lecture du code
    float a = A[0] ; float b = A[1] ; float c = A[2];
    float u = U[0] ; float v = U[1] ; float w = U[2];

    MyMesh::Point B; //Un point de la droite d2
    B = d2.get_position();
    MyMesh::Point V; //Vecteur de la droite d2
    V = d2.get_u();
    //Notation pour simplifier la lecture du code
    float x = B[0] ; float y = B[1] ; float z = B[2];
    float i = V[0] ; float j = V[1] ; float k = V[2];

    if(A == B){ //Gestion du cas où les points A et B sont les mêmes
        return A;
    }

    float alpha = 0; float beta = 0;

    //Differents cas a verifier (on cherche alpha ou beta)
    if(u == 0){

        if(i == 0){

            if(v == 0){

                beta = (b - y)/j;
            }
            else{ // w ne peut pas etre egal a 0 sinon la droite d1 ne serait pas une droite
                alpha = (z*j + b*k - y*k - c*j)/(j*w - v*k);
            }
        }
        else{
            beta = (a - x)/i;
        }
    }
    else{
        if(i == 0){
            alpha = (x - a)/u;
        }
        else{
            alpha = (a*j - x*j - b*j + y*i)/(v*i - u*j);
        }
    }

    //qDebug() << alpha << beta << Qt::endl;

    //Recuperation du point d'intersection
    MyMesh::Point pt_inter(0,0,0);
    if(alpha == 0)
    {
        pt_inter[0] = x + beta*i;
        pt_inter[1] = y + beta*j;
        pt_inter[2] = z + beta*k;
    }
    else{
        pt_inter[0] = a + alpha*u;
        pt_inter[1] = b + alpha*v;
        pt_inter[2] = c + alpha*w;
    }

    return pt_inter;
}


QVector<Plane> Geometry::get_planes(Mesh mesh){ //Recupere les plans correspondants aux faces des mesh

    MyMesh mymesh = mesh.get_mesh();

    // Rajout d'un propriété aux faces
    OpenMesh::FPropHandleT<bool> prop_plane;
    mymesh.add_property(prop_plane, "prop_plane");

    // On initialise la proprété des faces à true
    MyMesh *_mesh = &mymesh;
    for(MyMesh::FaceIter f_it = _mesh->faces_begin(); f_it != _mesh->faces_end(); f_it++){
        _mesh->property(prop_plane, *f_it) = true;
    }
    // Pour chaque Edge on calcule l'angle dihedre
    for (MyMesh::EdgeIter e_it = _mesh->edges_begin(); e_it != _mesh->edges_end(); e_it++) {
        float angle = _mesh->calc_dihedral_angle(*e_it);
        //qDebug() << "angle : " << angle;
        if(angle == 0){
            // Les deux faces de l'edge appartiennent au même plan
            // On en mets une a false
            HalfedgeHandle eh = _mesh->halfedge_handle(*e_it, 0);
            FaceHandle fh = _mesh->face_handle(eh);
            _mesh->property(prop_plane, fh) = false;
        }
    }

    //MyMesh::Point barycentre(0,0,0); //Initialisation
    MyMesh::Point A(0,0,0);
    MyMesh::Point B;
    MyMesh::Point C;
    QVector<Plane> planes;
    Plane P(A,A,A);

    //Calcul du barycentre
    for(MyMesh::FaceIter f_it = _mesh->faces_begin(); f_it != _mesh->faces_end(); f_it++){
        if(_mesh->property(prop_plane, *f_it)){
            /*for(MyMesh::FaceVertexIter fv_it = _mesh->fv_iter(*f_it); fv_it.is_valid(); ++fv_it){
                barycentre += _mesh->point(*fv_it);
            }
            barycentre /= 3;*/

            //Calcul de deux points du mesh
            MyMesh::FaceVertexIter fv_it = _mesh->fv_iter(*f_it);
            A = _mesh->point(*fv_it);
            fv_it++;
            B = _mesh->point(*fv_it);
            fv_it++;
            C = _mesh->point(*fv_it);
            P = Plane(A, get_vect(A, B), get_vect(A, C)); //Recuperation du  plan en prenant un point et deux vecteurs directeurs
            planes.append(P);
            //barycentre = MyMesh::Point(0,0,0); //Remise a 0
        }
    }
    return planes;
}
