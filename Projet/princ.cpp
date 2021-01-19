// Basé sur :
// CC-BY Edouard.Thiel@univ-amu.fr - 22/01/2019

#include "princ.h"
#include <QDebug>
#include <QFileDialog>
#include "MeshFormat.h"
#include "voronoi.h"

Princ::Princ(QWidget *parent) : QMainWindow(parent)
{
    setupUi(this);
    connect(this, SIGNAL(mesh_loaded(MyMesh)), glarea, SLOT(onMeshLoaded(MyMesh)));
    connect(vue, SIGNAL(clicked()), glarea, SLOT(on_vue_clicked()));
    connect(this, SIGNAL(seeds_clicked(MyMesh*, int)), glarea, SLOT(getSeeds(MyMesh*, int)));
}

void test_get_planes(Mesh mesh)
{
    QVector<Plane> planes;
    planes = Geometry::get_planes(mesh);
    for(Plane p:planes){
        qDebug() << "pos :" << Geometry::to_Qvector3D(p.get_position());
        qDebug() << "vec1 :" << Geometry::to_Qvector3D(p.get_u()) << "vec2 :" << Geometry::to_Qvector3D(p.get_v());
    }

    qDebug() << "number of planes : " << planes.size();
}

void test_get_mediator_planes(Mesh mesh)
{
    QVector<Plane> planes;
    planes = Voronoi::get_mediator_planes(MyMesh::Point(0,0,0), mesh);
    for(Plane p:planes){
        qDebug() << "pos :" << Geometry::to_Qvector3D(p.get_position());
        qDebug() << "vec1 :" << Geometry::to_Qvector3D(p.get_u()) << "vec2 :" << Geometry::to_Qvector3D(p.get_v());
    }
}

void Princ::on_pushButton_clicked()
{
    // fenêtre de sélection des fichiers
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open Mesh"), "", tr("Mesh Files (*.obj)"));

    // chargement du fichier .obj dans la variable globale "mesh"
    if(OpenMesh::IO::read_mesh(mesh, fileName.toUtf8().constData())){
        mesh_loaded(mesh);
    }
    Mesh _mesh(mesh,QVector3D(0,0,0));
    test_get_planes(_mesh);
    //test_get_mediator_planes(_mesh);
}


void Princ::on_seeds_button_clicked()
{
    emit seeds_clicked(&mesh, 16);//il faut que ce soit un multiple de 8;
}
