#ifndef SEEDGENERATOR_H
#define SEEDGENERATOR_H

#include <OpenMesh/Core/IO/MeshIO.hh>

using namespace OpenMesh;
#include <OpenMesh/Core/Mesh/TriMesh_ArrayKernelT.hh>
#include <OpenMesh/Core/Mesh/ArrayKernel.hh>
#include <OpenMesh/Core/Geometry/Vector11T.hh>
#include "MeshFormat.h"
#include <QVector>
#include <QDebug>
#include <cfloat>
#include <random>

class SeedGenerator
{
public:
    SeedGenerator(MyMesh *mesh, int n);
    QVector<MyMesh::Point> get_points();
    MyMesh::Point get_maxCoords();
    MyMesh::Point get_minCoords();
    void generateRand();
    void generateEquidistant(); //Il faut que le nombre de points soit un multiple de 4.
    QVector<MyMesh::Point> generatedPoints;

    MyMesh* mymesh;
    int nbPoints;
};

#endif // SEEDGENERATOR_H
