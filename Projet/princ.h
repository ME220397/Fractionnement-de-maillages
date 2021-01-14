// Basé sur :
// CC-BY Edouard.Thiel@univ-amu.fr - 22/01/2019

#ifndef PRINC_H
#define PRINC_H

#include "ui_princ.h"
#include "MeshFormat.h"

class Princ : public QMainWindow, private Ui::Princ
{
    Q_OBJECT

public:
    explicit Princ(QWidget *parent = nullptr);

signals:
    void mesh_loaded(MyMesh mesh);
private slots:
    void on_pushButton_clicked();
private:
    MyMesh mesh;
};

#endif // PRINC_H
