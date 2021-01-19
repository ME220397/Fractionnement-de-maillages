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

//Génère des points avec des positions aléatoire a l'intérieure de la bounding box de l'objet.
void SeedGenerator::generateRand(){
    MyMesh::Point  max_coord = get_maxCoords();
    MyMesh::Point min_coord = get_minCoords();

    std::random_device rd;
    std::default_random_engine eng(rd());
    std::uniform_real_distribution<> distrX(min_coord[0], max_coord[0]);
    std::uniform_real_distribution<> distrY(min_coord[1], max_coord[1]);
    std::uniform_real_distribution<> distrZ(min_coord[2], max_coord[2]);

    for(int i = 0; i < nbPoints; ++i){
        bool present = false;
        float x = distrX(eng);
        float y = distrY(eng);
        float z = distrZ(eng);
        MyMesh::Point newPoint(x, y ,z);
        if(generatedPoints.size() != 0){
            for(MyMesh::Point p : generatedPoints){
                if(x == p[0] && y == p[1] && z == p[2]){
                    qDebug() << "Point identique a un déjà existant.";
                    present = true;
                }
            }

            if(!present) generatedPoints.push_back(newPoint);
            else i--; // !!!
        } else generatedPoints.push_back(newPoint);
    }
}

//Génère des points tous a égal distance les uns des autres a l'interieur de la bounding box.
void SeedGenerator::generateEquidistant(){

    if(nbPoints%8 != 0) {
        qDebug() << "Le nombre de points n'est pas un multiple de 8";
        return;
    }

    MyMesh::Point max_coord = get_maxCoords();
    MyMesh::Point min_coord = get_minCoords();

    float nbCells = (float)nbPoints/4.0;

    float x_t = max_coord[0] - min_coord[0];
    float y_t = max_coord[1] - min_coord[1];
    float z_t = max_coord[2] - min_coord[2];

    float pas_x = x_t/nbCells;
    float pas_y = y_t/nbCells;
    float pas_z = z_t/nbCells;

    //Parcours des x.
    for(int i = 0; i < nbCells; i++){
        float current_x = min_coord[0] + (pas_x*i);
        float next_x = current_x + pas_x;

        for(int j =0; j < nbCells; j++){
            float current_y = min_coord[1] + (pas_y*j);
            float next_y = current_y + pas_y;
            for(int k =0; k < nbCells; k++){

                float current_z = min_coord[2] + (pas_z*k);
                float next_z = current_z + pas_z;


                float x = (current_x + next_x)/2;
                float y = (current_y + next_y)/2;
                float z = (current_z + next_z)/2;
                MyMesh::Point newPoint(x, y ,z);
                generatedPoints.push_back(newPoint);
            }
        }
    }
}
