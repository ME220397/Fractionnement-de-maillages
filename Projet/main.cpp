// Basé sur :
// CC-BY Edouard.Thiel@univ-amu.fr - 22/01/2019

#include "princ.h"
#include "geometry.h"
#include "plane.h"
#include <QApplication>

void TEST_plane_intersection(){
    Plane p1 = Plane(
                MyMesh::Point(0,0,0),
                MyMesh::Point(0,1,0),
                MyMesh::Point(0,0,1)
    );

    Plane p2 = Plane(
                MyMesh::Point(0,0,0),
                MyMesh::Point(0,1,0),
                MyMesh::Point(1,0,0)
    );

    bool intersected = Geometry::intersected(p1, p2);
    assert(intersected);

    Plane p3 = Plane(
                MyMesh::Point(1,1,0),
                MyMesh::Point(0,1,0),
                MyMesh::Point(1,0,0)
    );

    Plane p4 = Plane(
                MyMesh::Point(0,0,0),
                MyMesh::Point(0,1,0),
                MyMesh::Point(1,0,0)
    );

    intersected = Geometry::intersected(p3, p4);
    assert(!intersected);

}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    TEST_plane_intersection();
    Princ w;
    w.show();
    return a.exec();
}
