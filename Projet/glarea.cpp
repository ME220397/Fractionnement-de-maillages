// Basé sur :
// CC-BY Edouard.Thiel@univ-amu.fr - 22/01/2019

#include "glarea.h"
#include <QDebug>
#include <QSurfaceFormat>
#include <QMatrix4x4>


static const char *vertexShaderSourceM =
    "attribute vec4 in_position;               \n"
    "attribute lowp vec4 col;               \n"
    "varying lowp vec4 color;               \n"
    "uniform mat4 projectionMatrix;         \n"
    "uniform mat4 viewMatrix;               \n"
    "uniform mat4 modelMatrix;              \n"
    "uniform float size;                    \n"
    "void main() {                          \n"
    "   color = col;                        \n"
    "   vec4 pos = vec4(vec3(in_position)*size, 1.0);                   \n"
    "   gl_Position = projectionMatrix * viewMatrix * modelMatrix * pos;          \n"
    "}                                      \n";

// shader appliqué à un fragment = un morceau rasterisé d'une primitive graphique
static const char *fragmentShaderSourceM =
    "varying lowp vec4 color;               \n"
    "void main() {                          \n"
    "   gl_FragColor = color;               \n"
    "}                                      \n";

GLArea::GLArea(QWidget *parent) :
    QOpenGLWidget(parent)
{
    QSurfaceFormat sf;
    sf.setDepthBufferSize(24);
    sf.setSamples(16);
    setFormat(sf);

    setEnabled(true);                   // événements clavier et souris
    setFocusPolicy(Qt::StrongFocus);    // accepte focus
    setFocus();                         // donne le focus

    timer = new QTimer(this);
    timer->setInterval(50);           // msec
    connect (timer, SIGNAL(timeout()), this, SLOT(onTimeout()));
//    timer->start();
    elapsedTimer.start();


    physics = new Physics();

    face_mode = true;
}


GLArea::~GLArea()
{
    delete timer;

    // Contrairement aux méthodes virtuelles initializeGL, resizeGL et repaintGL,
    // dans le destructeur le contexte GL n'est pas automatiquement rendu courant.
    makeCurrent();
    tearGLObjects();
    doneCurrent();
}


void GLArea::initializeGL()
{
    initializeOpenGLFunctions();
    glClearColor(0.7f,0.7f,0.7f,1.0f);
    glEnable(GL_DEPTH_TEST);

    makeGLObjects();

    program_mesh = new QOpenGLShaderProgram(this);
    program_mesh->addShaderFromSourceCode(QOpenGLShader::Vertex, vertexShaderSourceM);
    program_mesh->addShaderFromSourceCode(QOpenGLShader::Fragment, fragmentShaderSourceM);
    if (! program_mesh->link()) {  // édition de lien des shaders dans le shader program
       qWarning("Failed to compile and link shader program:");
       qWarning() << program_mesh->log();
    }

    program_sol = new QOpenGLShaderProgram(this);
        program_sol->addShaderFromSourceFile(QOpenGLShader::Vertex, ":/shaders/simple.vsh");
        program_sol->addShaderFromSourceFile(QOpenGLShader::Fragment, ":/shaders/simple.fsh");
        if (! program_sol->link()) {  // édition de lien des shaders dans le shader program
            qWarning("Failed to compile and link shader program:");
            qWarning() << program_sol->log();
        }
    program_sol->setUniformValue("texture", 0);
}


void GLArea::makeGLObjects()
{
    for(Mesh p : meshes){
        p.load_data();
//        physics->createConvex(&p);
    }

    // Création du sol
    sol = new Boite(QVector3D(0.0f, -55.0f, 0.0f), 100.0, 0, 0);
    sol->create_sol();
    physics->createSol(sol);
    vbo_sol.create();
    vbo_sol.bind();
    vbo_sol.allocate(sol->boite_data.constData(), sol->boite_data.count() * int(sizeof(GLfloat)));

    // Création de textures
    QImage image_sol(":/textures/ground.jpg");
    if (image_sol.isNull())
        qDebug() << "load image ground.jpg failed";
    texture[0] = new QOpenGLTexture(image_sol);

}


void GLArea::tearGLObjects()
{
    for(Mesh p : meshes){
        p.destroy_vbos();
    }

    vbo_sol.destroy();
    physics->delete_sim();
    delete texture[1];
}


void GLArea::resizeGL(int w, int h)
{
    glViewport(0, 0, w, h);
    windowRatio = float(w) / h;
}


void GLArea::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    // Matrice de projection
    QMatrix4x4 projectionMatrix;
    projectionMatrix.perspective(45.0f, windowRatio, 1.0f, 1000.0f);

    // Matrice de vue (caméra)
    QMatrix4x4 viewMatrix;
    viewMatrix.translate(xPos, yPos, zPos);
    viewMatrix.rotate(xRot, 1, 0, 0);
    viewMatrix.rotate(yRot, 0, 1, 0);
    viewMatrix.rotate(zRot, 0, 0, 1);


    for(Mesh p : printableMesh){
        p.draw(projectionMatrix, viewMatrix, program_mesh);
    }

    // Affichage du sol
    vbo_sol.bind();
    program_sol->bind(); // active le shader program du sol

    QMatrix4x4 modelMatrixSol;
//    modelMatrixSol.setToIdentity();
    modelMatrixSol.translate(sol->position);
    program_sol->setUniformValue("projectionMatrix", projectionMatrix);
    program_sol->setUniformValue("viewMatrix", viewMatrix);
    program_sol->setUniformValue("modelMatrix", modelMatrixSol);

    program_sol->setAttributeBuffer("in_position", GL_FLOAT, 0, 3, 5 * sizeof(GLfloat));
    program_sol->setAttributeBuffer("in_uv", GL_FLOAT, 3 * sizeof(GLfloat), 2, 5 * sizeof(GLfloat));
    program_sol->enableAttributeArray("in_position");
    program_sol->enableAttributeArray("in_uv");

    texture[0]->bind();
    glDrawArrays(GL_TRIANGLES, 0, 6);
    texture[0]->release();

    program_sol->disableAttributeArray("in_position");
    program_sol->disableAttributeArray("in_uv");
    program_sol->release();
}


void GLArea::keyPressEvent(QKeyEvent *ev)
{
    float da = 1.0f;

    switch(ev->key()) {
        case Qt::Key_A :
            xRot -= da;
            update();
            break;

        case Qt::Key_Q :
            xRot += da;
            update();
            break;

        case Qt::Key_Z :
            yRot -= da;
            update();
            break;

        case Qt::Key_S :
            yRot += da;
            update();
            break;

        case Qt::Key_E :
            zRot -= da;
            update();
            break;

        case Qt::Key_D :
            zRot += da;
            update();
            break;

        case Qt::Key_T :
            if(timer->isActive()) timer->stop();
            else timer->start();
            break;
    }
}


void GLArea::keyReleaseEvent(QKeyEvent *ev)
{
    qDebug() << __FUNCTION__ << ev->text();
}


void GLArea::mousePressEvent(QMouseEvent *ev)
{
    lastPos = ev->pos();
}


void GLArea::mouseReleaseEvent(QMouseEvent *ev)
{
    qDebug() << __FUNCTION__ << ev->x() << ev->y() << ev->button();
}


void GLArea::mouseMoveEvent(QMouseEvent *ev)
{
    int dx = ev->x() - lastPos.x();
    int dy = ev->y() - lastPos.y();

    if (ev->buttons() & Qt::LeftButton) {
        xRot += dy;
        yRot += dx;
        update();
    } else if (ev->buttons() & Qt::RightButton) {
        xPos += dx/10.0f;
        yPos -= dy/10.0f;
        update();
    } else if (ev->buttons() & Qt::MidButton) {
        xPos += dx/10.0f;
        zPos += dy;
        update();
    }

    lastPos = ev->pos();
}


void GLArea::onTimeout()
{
    static qint64 old_chrono = elapsedTimer.elapsed(); // static : initialisation la première fois et conserve la dernière valeur
    qint64 chrono = elapsedTimer.elapsed();
    dt = (chrono - old_chrono) / 1000.0f;
    old_chrono = chrono;

    sol->anime(physics->get_world());
    for(Mesh &mesh : printableMesh){
        mesh.anime(physics->get_world());
        if(mesh.get_position()[1] <= -3.9 && (voroDone == false)){
            voroDone = true;
            createVoronoi(mesh);
        }
    }

    update();
}

void GLArea::wheelEvent(QWheelEvent *_event){
    float dz = -(float)_event->delta() / 120.0 * 0.2;
    zPos += dz;
    update();
}

void GLArea::onMeshLoaded(MyMesh mesh){
    meshes.clear();
    QVector3D pos(0, 0, 0);
    Mesh newMesh(mesh, pos);
    newMesh.load_data();
    MyMesh box = newMesh.compute_bounding_box();
    Mesh bbox(box, pos);
    bbox.color_all_edges(255, 0, 0);
    bbox.color_all_points(0, 0, 255);
    bbox.set_thickness_all_edges(10.0f);
    bbox.set_thickness_all_points(2.f);
    bbox.load_data();
    bbox.set_faces_visible(false);
    physics->createConvex(&newMesh);
    printableMesh.push_back(newMesh);
    meshes.push_back(newMesh);
    meshes.push_back(bbox);
}

void GLArea::on_vue_clicked(){
    if(!meshes.empty()){
        if(face_mode){
            meshes.at(0).color_all_edges(255, 140, 0);
            meshes.at(0).load_data();
            face_mode = false;
        }
        else{
            meshes.at(0).color_all_edges(0, 0, 0);
            meshes.at(0).load_data();
            face_mode = true;
        }
        meshes.at(0).set_faces_visible(face_mode);
    }
}

void GLArea::getSeeds(MyMesh *mesh, int nbSeeds){
    qDebug() << __FUNCTION__;
    SeedGenerator seeds(mesh, nbSeeds);
    seeds.generateRand();
//    seeds.generateEquidistant();
    generatedSeeds = seeds.get_points();
    Mesh seedsMesh(generatedSeeds, QVector3D(0.0f, 0.0f, 0.0f));
    seedsMesh.set_thickness_all_points(15.f);
    seedsMesh.color_all_points(255, 0, 0);
    seedsMesh.load_data();
    meshes.push_back(seedsMesh);
}

void GLArea::createVoronoi(Mesh mesh){
    voronoiMeshes = Voronoi::compute_voronoi(mesh, generatedSeeds);
    for(Mesh &mesh : voronoiMeshes){
        physics->createConvex(&mesh);
    }
}
