#ifndef SEEDGENERATOR_H
#define SEEDGENERATOR_H

#include <OpenMesh/Core/IO/MeshIO.hh>

using namespace OpenMesh;
#include <OpenMesh/Core/Mesh/TriMesh_ArrayKernelT.hh>
#include <OpenMesh/Core/Mesh/ArrayKernel.hh>
#include <OpenMesh/Core/Geometry/Vector11T.hh>
#include <QVector>
#include <QDebug>
#include <cfloat>
#include <random>

struct MyTraits : public OpenMesh::DefaultTraits
{
    // use vertex normals and vertex colors
    VertexAttributes( OpenMesh::Attributes::Normal | OpenMesh::Attributes::Color );
    // store the previous halfedge
    HalfedgeAttributes( OpenMesh::Attributes::PrevHalfedge );
    // use face normals face colors
    FaceAttributes( OpenMesh::Attributes::Normal | OpenMesh::Attributes::Color );
    EdgeAttributes( OpenMesh::Attributes::Color );
    // vertex thickness
    VertexTraits{float thickness; float value; Color faceShadingColor;};
    // edge thickness
    EdgeTraits{float thickness;};
};
typedef OpenMesh::TriMesh_ArrayKernelT<MyTraits> MyMesh;


class SeedGenerator
{
public:
    SeedGenerator(MyMesh *mesh, int n);
    QVector<MyMesh::Point> get_points();
    MyMesh::Point get_maxCoords();
    MyMesh::Point get_minCoords();
    void generateRand();
    QVector<MyMesh::Point> generatedPoints;

    MyMesh* mymesh;
    int nbPoints;
};

#endif // SEEDGENERATOR_H
