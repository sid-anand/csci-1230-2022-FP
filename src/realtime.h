#pragma once

// Defined before including GLEW to suppress deprecation messages on macOS
#include "camera/camera.h"
#include "shapes/Building.h"
#include "shapes/Cone.h"
#include "shapes/Cube.h"
#include "shapes/Cylinder.h"
#include "shapes/Ground.h"
#include "shapes/Sphere.h"
#include "utils/sceneparser.h"
#ifdef __APPLE__
#define GL_SILENCE_DEPRECATION
#endif
#include <GL/glew.h>
#include <glm/glm.hpp>

#include <unordered_map>
#include <QElapsedTimer>
#include <QOpenGLWidget>
#include <QTime>
#include <QTimer>

class Realtime : public QOpenGLWidget
{
public:
    Realtime(QWidget *parent = nullptr);
    void finish();                                      // Called on program exit
    void sceneChanged();
    void settingsChanged();

public slots:
    void tick(QTimerEvent* event);                      // Called once per tick of m_timer

protected:
    void initializeGL() override;                       // Called once at the start of the program
    void paintGL() override;                            // Called whenever the OpenGL context changes or by an update() request
    void resizeGL(int width, int height) override;      // Called when window size changes

private:
    void keyPressEvent(QKeyEvent *event) override;
    void keyReleaseEvent(QKeyEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void timerEvent(QTimerEvent *event) override;

    // Tick Related Variables
    int m_timer;                                        // Stores timer which attempts to run ~60 times per second
    QElapsedTimer m_elapsedTimer;                       // Stores timer which keeps track of actual time between frames

    // Input Related Variables
    bool m_mouseDown = false;                           // Stores state of left mouse button
    glm::vec2 m_prev_mouse_pos;                         // Stores mouse position
    std::unordered_map<Qt::Key, bool> m_keyMap;         // Stores whether keys are pressed or not

    // Device Correction Variables
    int m_devicePixelRatio;

    // Additional methods / fields
    void setupVBOVAO(GLuint *vbo, GLuint *vao, std::vector<GLfloat> mesh);
    GLuint createTexture(std::string filepath);
    void setupSkybox();
    void drawSkybox();

    RenderData m_renderData;
    Camera m_camera;
    GLuint m_phong_shader;

    bool m_initializedGL = false;
    bool m_loadedScene = false;

    QImage m_image;
    GLuint m_texture;
    std::vector<Building> m_buildings;
    std::vector<GLuint> m_vbos;
    std::vector<GLuint> m_vaos;
    std::vector<GLuint> textures;
    Ground m_ground;
    GLuint m_ground_vbo;
    GLuint m_ground_vao;
    GLuint m_ground_texture;

    int m_grid_size = 50;

    GLuint m_skybox_shader;
    GLuint m_skybox_vao, m_skybox_vbo, m_skybox_ebo;
    GLuint m_cubemap_texture;

    float m_distanceBezier;
};
