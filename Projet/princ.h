// Basé sur :
// CC-BY Edouard.Thiel@univ-amu.fr - 22/01/2019

#ifndef PRINC_H
#define PRINC_H

#include "ui_princ.h"
#include "MeshFormat.h"
#include "geometry.h"

class Princ : public QMainWindow, private Ui::Princ
{
    Q_OBJECT

public:
    explicit Princ(QWidget *parent = nullptr);

signals:
    void mesh_loaded(MyMesh mesh);
    void seeds_clicked(MyMesh *mesh, int nbSeeds);

private slots:
    void on_pushButton_clicked();
    void on_seeds_button_clicked();

private:
    MyMesh mesh;
};

#endif // PRINC_H
