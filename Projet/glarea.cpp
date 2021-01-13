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
    "   vec4 pos = vec4(vec3(in_position), 1.0);                   \n"
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
    timer->start();
    elapsedTimer.start();
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

}


void GLArea::makeGLObjects()
{
    for(Mesh p : meshes){
        p.load_data();
    }
}


void GLArea::tearGLObjects()
{
    for(Mesh p : meshes){
        p.destroy_vbos();
    }
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

    for(Mesh p : meshes){
        p.draw(projectionMatrix, viewMatrix, program_mesh);
    }
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



    update();
}

void GLArea::wheelEvent(QWheelEvent *_event){
    float dz = -(float)_event->delta() / 120.0 * 0.2;
    zPos += dz;
    update();
}

void GLArea::onMeshLoaded(MyMesh mesh){
    QVector3D pos(posX, 0, 0);
    posX+=6;
    Mesh newMesh(mesh, pos);
    newMesh.load_data();
    MyMesh box = newMesh.compute_bounding_box();
    Mesh bbox(box, pos);
    bbox.load_data();
    bbox.set_faces_visible(false);
    meshes.push_back(newMesh);
    meshes.push_back(bbox);
}
