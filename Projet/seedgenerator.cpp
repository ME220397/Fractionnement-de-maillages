#include "seedgenerator.h"

SeedGenerator::SeedGenerator(MyMesh* mesh, int n){
    this->mymesh = mesh;
    this->nbPoints = n;
}

QVector<MyMesh::Point> SeedGenerator::get_points(){
    if(generatedPoints.isEmpty()){
        qDebug() << "Il n'y a eu de génération de points";
        return generatedPoints;
    }
    else return generatedPoints;
}

MyMesh::Point SeedGenerator::get_maxCoords(){
    MyMesh::Point  max_coord = MyMesh::Point(-FLT_MAX, -FLT_MAX, -FLT_MAX);
    for(MyMesh::VertexIter v_it = mymesh->vertices_begin(); v_it != mymesh->vertices_end(); ++v_it)
    {
        VertexHandle vh = *v_it;
        MyMesh::Point p = mymesh->point(vh);
        if(max_coord[0] < p[0]){
            max_coord[0] = p[0];
        }
        if(max_coord[1] < p[1]){
            max_coord[1] = p[1];
        }
        if(max_coord[2] < p[2]){
            max_coord[2] = p[2];
        }
    }

    return max_coord;
}

MyMesh::Point SeedGenerator::get_minCoords(){
    MyMesh::Point  min_coord = MyMesh::Point(FLT_MAX, FLT_MAX, FLT_MAX);
    for(MyMesh::VertexIter v_it = mymesh->vertices_begin(); v_it != mymesh->vertices_end(); ++v_it)
    {
        VertexHandle vh = *v_it;
        MyMesh::Point p = mymesh->point(vh);
        if(min_coord[0] > p[0]){
            min_coord[0] = p[0];
        }
        if(min_coord[1] > p[1]){
            min_coord[1] = p[1];
        }
        if(min_coord[2] > p[2]){
            min_coord[2] = p[2];
        }
    }

    return min_coord;
}

void SeedGenerator::generateRand(){
    MyMesh::Point  max_coord = get_minCoords();
    MyMesh::Point min_coord = get_minCoords();

    std::random_device rd;
    std::default_random_engine eng(rd());
    std::uniform_real_distribution<> distrX(min_coord[0], max_coord[0]);
    std::uniform_real_distribution<> distrY(min_coord[1], max_coord[1]);
    std::uniform_real_distribution<> distrZ(min_coord[2], max_coord[2]);

    for(int i = 0; i < nbPoints; ++i){
        float x = distrX(eng);
        float y = distrY(eng);
        float z = distrZ(eng);
        MyMesh::Point newPoint(x, y ,z);
        generatedPoints.push_back(newPoint);
    }
}
