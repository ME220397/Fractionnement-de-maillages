// Basé sur :
// CC-BY Edouard.Thiel@univ-amu.fr - 22/01/2019

#include "princ.h"
#include <QDebug>
#include <QFileDialog>
#include "MeshFormat.h"

Princ::Princ(QWidget *parent) : QMainWindow(parent)
{
    setupUi(this);
    connect(this, SIGNAL(mesh_loaded(MyMesh)), glarea, SLOT(onMeshLoaded(MyMesh)));
    connect(this, SIGNAL(seeds_clicked(MyMesh*, int)), glarea, SLOT(getSeeds(MyMesh*, int)));
}

void Princ::on_pushButton_clicked()
{
    // fenêtre de sélection des fichiers
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open Mesh"), "", tr("Mesh Files (*.obj)"));

    // chargement du fichier .obj dans la variable globale "mesh"
    if(OpenMesh::IO::read_mesh(mesh, fileName.toUtf8().constData())){
        mesh_loaded(mesh);
    }
}


void Princ::on_seeds_button_clicked()
{
    emit seeds_clicked(&mesh, 16);//il faut que ce soit un multiple de 8;
}
