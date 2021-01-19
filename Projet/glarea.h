// Basé sur :
// CC-BY Edouard.Thiel@univ-amu.fr - 22/01/2019

#ifndef GLAREA_H
#define GLAREA_H

#include <QKeyEvent>
#include <QTimer>
#include <QElapsedTimer>
#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QOpenGLBuffer>
#include <QOpenGLShaderProgram>
#include <QOpenGLTexture>

#include "mesh.h"
#include "seedgenerator.h"
#include "boite.h"
#include "physics.h"

class GLArea : public QOpenGLWidget,
               protected QOpenGLFunctions
{
    Q_OBJECT

public:
    explicit GLArea(QWidget *parent = nullptr);
    ~GLArea() override;


protected slots:
    void onTimeout();
    void onMeshLoaded(MyMesh mesh);
    void on_vue_clicked();
    void getSeeds(MyMesh *mesh, int nbSeeds);

protected:
    void initializeGL() override;
    void doProjection();
    void resizeGL(int w, int h) override;
    void paintGL() override;
    void keyPressEvent(QKeyEvent *ev) override;
    void keyReleaseEvent(QKeyEvent *ev) override;
    void mousePressEvent(QMouseEvent *ev) override;
    void mouseReleaseEvent(QMouseEvent *ev) override;
    void mouseMoveEvent(QMouseEvent *ev) override;
    virtual void wheelEvent( QWheelEvent* ) override;

private:
    float xRot=20.0f, yRot=0.0f, zRot=0.0f;
    float xPos=0.0f,  yPos=0.0f, zPos=-50.0f;
    QTimer *timer = nullptr;
    QElapsedTimer elapsedTimer;
    float dt = 0;
    float windowRatio = 1.0f;
    QPoint lastPos;

    Physics *physics;
    Boite *sol;

    QOpenGLShaderProgram *program_sol;
    QOpenGLShaderProgram *program_mesh;
    QOpenGLBuffer vbo_sol;
    QOpenGLBuffer vbo_particule;
    QOpenGLTexture *texture[1];


    void makeGLObjects();
    void tearGLObjects();

    vector<Mesh> meshes;
    vector<Mesh> printableMesh;

    bool face_mode;
};

#endif // GLAREA_H
