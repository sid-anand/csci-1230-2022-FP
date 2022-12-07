#include "realtime.h"

#include <QCoreApplication>
#include <QMouseEvent>
#include <QKeyEvent>
#include <iostream>
#include "floorplan.h"
#include "settings.h"
#include "shapes/Building.h"
#include "utils/sceneparser.h"
#include "utils/shaderloader.h"
#include "glm/gtx/string_cast.hpp"

// ================== Project 5: Lights, Camera

Realtime::Realtime(QWidget *parent)
    : QOpenGLWidget(parent), m_camera()
{
    m_prev_mouse_pos = glm::vec2(size().width()/2, size().height()/2);
    setMouseTracking(true);
    setFocusPolicy(Qt::StrongFocus);

    m_keyMap[Qt::Key_W]       = false;
    m_keyMap[Qt::Key_A]       = false;
    m_keyMap[Qt::Key_S]       = false;
    m_keyMap[Qt::Key_D]       = false;
    m_keyMap[Qt::Key_Control] = false;
    m_keyMap[Qt::Key_Space]   = false;

    // If you must use this function, do not edit anything above this
}

void Realtime::finish() {
    killTimer(m_timer);
    this->makeCurrent();

    // Students: anything requiring OpenGL calls when the program exits should be done here

    for (GLuint vbo : m_vbos) {
        glDeleteBuffers(1, &vbo);
    }

    for (GLuint vao : m_vaos) {
        glDeleteVertexArrays(1, &vao);
    }

    glDeleteProgram(m_phong_shader);

    this->doneCurrent();
}

void Realtime::initializeGL() {
    m_devicePixelRatio = this->devicePixelRatio();

    m_timer = startTimer(1000/60);
    m_elapsedTimer.start();

    // Initializing GL.
    // GLEW (GL Extension Wrangler) provides access to OpenGL functions.
    glewExperimental = GL_TRUE;
    GLenum err = glewInit();
    if (err != GLEW_OK) {
        std::cerr << "Error while initializing GL: " << glewGetErrorString(err) << std::endl;
    }
    std::cout << "Initialized GL: Version " << glewGetString(GLEW_VERSION) << std::endl;

    m_initializedGL = true;

    // Allows OpenGL to draw objects appropriately on top of one another
    glEnable(GL_DEPTH_TEST);
    // Tells OpenGL to only draw the front face
    glEnable(GL_CULL_FACE);
    // Tells OpenGL how big the screen is
    glViewport(0, 0, size().width() * m_devicePixelRatio, size().height() * m_devicePixelRatio);

    // Students: anything requiring OpenGL calls when the program starts should be done here

    // Set the clear color
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    // Load shaders
    m_phong_shader = ShaderLoader::createShaderProgram(":/resources/shaders/phong.vert", ":/resources/shaders/phong.frag");
    m_skybox_shader = ShaderLoader::createShaderProgram(":/resources/shaders/skybox.vert", ":/resources/shaders/skybox.frag");

    FloorPlan fp = FloorPlan(10.0f, 1);
    m_buildings = fp.buildings;
    for(int i = 0; i < m_buildings.size(); i++){
        GLuint currVBO;
        GLuint currVAO;
        m_vbos.push_back(currVBO);
        m_vaos.push_back(currVAO);
        setupVBOVAO(&currVBO, &currVAO, m_buildings[i].getMesh());
    }

    QString brick_filepath = QString(":/resources/images/building.jpg");
    m_image = QImage(brick_filepath);
    m_image = m_image.convertToFormat(QImage::Format_RGBA8888).mirrored();
    glGenTextures(1, &m_brick_texture);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, m_brick_texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_image.width(), m_image.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, m_image.bits());
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glBindTexture(GL_TEXTURE_2D, 0);

    glUseProgram(m_phong_shader);
    glUniform1i(glGetUniformLocation(m_phong_shader, "texture1"), 0);
    glUseProgram(0);

    setupSkybox();

    m_renderData.cameraData = SceneCameraData{
            glm::vec4(0, 0, 0, 1),
            glm::vec4(0, 0, -1, 0),
            glm::vec4(0, 1, 0, 0),
            30
    };

    m_camera = Camera(size().width(), size().height(), m_renderData.cameraData, settings.nearPlane, 100.f);
    m_camera.setBezierPoints(glm::vec3(5, 5, 5), glm::vec3(9, 2, 9), glm::vec3(9, 2, 2), glm::vec3(5, 9, 2));
    m_distanceBezier = 0;
}

void Realtime::paintGL() {
    // Students: anything requiring OpenGL calls every frame should be done here

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glUseProgram(m_phong_shader);

    glUniformMatrix4fv(glGetUniformLocation(m_phong_shader, "view"), 1, GL_FALSE, &m_camera.getViewMatrix()[0][0]);
    glUniformMatrix4fv(glGetUniformLocation(m_phong_shader, "proj"), 1, GL_FALSE, &m_camera.getProjMatrix()[0][0]);
    for (int i = 0; i < m_buildings.size(); i++) {
        Building building = m_buildings[i];
        GLuint buildingVAO = m_vaos[i];
        glBindVertexArray(buildingVAO);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, m_brick_texture);
        glDrawArrays(GL_TRIANGLES, 0, building.getMesh().size() / 8);
        glBindTexture(GL_TEXTURE_2D, 0);
        glBindVertexArray(0);
    }

    // simulating the moonlight
    glm::vec4 lightDirection = glm::vec4(1, -3, -2, 0);
    glm::vec4 lightColor = glm::vec4(1, 1, 1, 1);
    glUniform4fv(glGetUniformLocation(m_phong_shader, "cameraPos"), 1, &m_camera.getPos()[0]);
    glUniform4fv(glGetUniformLocation(m_phong_shader, "lightDirection"), 1, &lightDirection[0]);
    glUniform4fv(glGetUniformLocation(m_phong_shader, "lightColor"), 1, &lightColor[0]);

    glUseProgram(0);

    // Draw skybox last for performance reasons
    drawSkybox();
}

void Realtime::resizeGL(int w, int h) {
    // Tells OpenGL how big the screen is
    glViewport(0, 0, size().width() * m_devicePixelRatio, size().height() * m_devicePixelRatio);

    // Students: anything requiring OpenGL calls when the program starts should be done here

    if (m_loadedScene) {
        m_camera.setDimensions(size().width(), size().height());
    }
}

void Realtime::sceneChanged() {
    SceneParser::parse(settings.sceneFilePath, m_renderData);
    m_loadedScene = true;
    m_camera = Camera(size().width(), size().height(), m_renderData.cameraData, settings.nearPlane, settings.farPlane);

    update(); // asks for a PaintGL() call to occur
}

void Realtime::settingsChanged() {
    if (!m_initializedGL) {
        return;
    }

    if (m_loadedScene) {
        m_camera.setPlanes(settings.nearPlane, settings.farPlane);
    }

    update(); // asks for a PaintGL() call to occur
}

void Realtime::setupSkybox() {
    std::vector<GLfloat> skyboxVertices =
    {
        //   Coordinates
        -1.0f, -1.0f,  1.0f,//        7--------6
         1.0f, -1.0f,  1.0f,//       /|       /|
         1.0f, -1.0f, -1.0f,//      4--------5 |
        -1.0f, -1.0f, -1.0f,//      | |      | |
        -1.0f,  1.0f,  1.0f,//      | 3------|-2
         1.0f,  1.0f,  1.0f,//      |/       |/
         1.0f,  1.0f, -1.0f,//      0--------1
        -1.0f,  1.0f, -1.0f
    };

    std::vector<GLuint> skyboxIndices =
    {
        // Right
        1, 2, 6,
        6, 5, 1,
        // Left
        0, 4, 7,
        7, 3, 0,
        // Top
        4, 5, 6,
        6, 7, 4,
        // Bottom
        0, 3, 2,
        2, 1, 0,
        // Back
        0, 1, 5,
        5, 4, 0,
        // Front
        3, 7, 6,
        6, 2, 3
    };

    glGenVertexArrays(1, &m_skybox_vao);
    glGenBuffers(1, &m_skybox_vbo);
    glGenBuffers(1, &m_skybox_ebo);
    glBindVertexArray(m_skybox_vao);
    glBindBuffer(GL_ARRAY_BUFFER, m_skybox_vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * skyboxVertices.size(), skyboxVertices.data(), GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_skybox_ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLfloat) * skyboxIndices.size(), skyboxIndices.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    std::string facesCubemap[6] =
    {
        ":/resources/images/right.png",
        ":/resources/images/left.png",
        ":/resources/images/top.png",
        ":/resources/images/bottom.png",
        ":/resources/images/front.png",
        ":/resources/images/back.png"
    };

    glGenTextures(1, &m_cubemap_texture);
    glBindTexture(GL_TEXTURE_CUBE_MAP, m_cubemap_texture);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    for (int i = 0; i < 6; i++) {
        QImage image = QImage(QString(facesCubemap[i].c_str()));
        image = image.convertToFormat(QImage::Format_RGBA8888); // not mirrored
        glTexImage2D
        (
            GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
            0,
            GL_RGBA,
            image.width(),
            image.height(),
            0,
            GL_RGBA,
            GL_UNSIGNED_BYTE,
            image.bits()
        );
    }

    glBindTexture(GL_TEXTURE_CUBE_MAP, 0);

    glUseProgram(m_skybox_shader);
    glUniform1i(glGetUniformLocation(m_skybox_shader, "skybox"), 0);
    glUseProgram(0);
}

void Realtime::drawSkybox() {
    // Cubemap always has depth of 1.0, so we need GL_LEQUAL, not GL_LESS
    glDepthFunc(GL_LEQUAL);

    glUseProgram(m_skybox_shader);
    // Get rid of last row/col to remove translation of skybox
    glm::mat4 view = glm::mat4(glm::mat3(m_camera.getViewMatrix()));
    glUniformMatrix4fv(glGetUniformLocation(m_skybox_shader, "view"), 1, GL_FALSE, &view[0][0]);
    glUniformMatrix4fv(glGetUniformLocation(m_skybox_shader, "projection"), 1, GL_FALSE, &m_camera.getProjMatrix()[0][0]);

    glDisable(GL_CULL_FACE);
    glBindVertexArray(m_skybox_vao);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, m_cubemap_texture);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
    glEnable(GL_CULL_FACE);

    // Switch back to the normal depth function
    glDepthFunc(GL_LESS);
}

void Realtime::setupVBOVAO(GLuint *vbo, GLuint *vao, std::vector<GLfloat> mesh) {
    // Generate and bind VBO
    glGenBuffers(1, vbo);
    glBindBuffer(GL_ARRAY_BUFFER, *vbo);

    // Pass triangle mesh into VBO
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * mesh.size(), mesh.data(), GL_STATIC_DRAW);

    // Generate and bind VAO
    glGenVertexArrays(1, vao);
    glBindVertexArray(*vao);

    // Add attributes to VAO
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 8, 0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 8, reinterpret_cast<void*>(sizeof(GLfloat) * 3));
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 8, reinterpret_cast<void*>(sizeof(GLfloat) * 6));

    // Unbind VBO and VAO
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

// ================== Project 6: Action!

void Realtime::keyPressEvent(QKeyEvent *event) {
    m_keyMap[Qt::Key(event->key())] = true;
}

void Realtime::keyReleaseEvent(QKeyEvent *event) {
    m_keyMap[Qt::Key(event->key())] = false;
}

void Realtime::mousePressEvent(QMouseEvent *event) {
    if (event->buttons().testFlag(Qt::LeftButton)) {
        m_mouseDown = true;
        m_prev_mouse_pos = glm::vec2(event->position().x(), event->position().y());
    }
}

void Realtime::mouseReleaseEvent(QMouseEvent *event) {
    if (!event->buttons().testFlag(Qt::LeftButton)) {
        m_mouseDown = false;
    }
}

void Realtime::mouseMoveEvent(QMouseEvent *event) {
    if (m_mouseDown) {
        int posX = event->position().x();
        int posY = event->position().y();
        int deltaX = posX - m_prev_mouse_pos.x;
        int deltaY = posY - m_prev_mouse_pos.y;
        m_prev_mouse_pos = glm::vec2(posX, posY);

        // Use deltaX and deltaY here to rotate
        m_camera.rotate(deltaX, deltaY);

        update(); // asks for a PaintGL() call to occur
    }
}

void Realtime::timerEvent(QTimerEvent *event) {
    int elapsedms   = m_elapsedTimer.elapsed();
    float deltaTime = elapsedms * 0.001f;
    m_elapsedTimer.restart();

    // Use deltaTime and m_keyMap here to move around
    if (m_keyMap[Qt::Key_W]) {
        m_camera.translate(CameraDirection::FORWARD, deltaTime);
    } else if (m_keyMap[Qt::Key_S]) {
        m_camera.translate(CameraDirection::BACKWARD, deltaTime);
    } else if (m_keyMap[Qt::Key_A]) {
        m_camera.translate(CameraDirection::LEFT, deltaTime);
    } else if (m_keyMap[Qt::Key_D]) {
        m_camera.translate(CameraDirection::RIGHT, deltaTime);
    } else if (m_keyMap[Qt::Key_Space]) {
        m_camera.translate(CameraDirection::UP, deltaTime);
    } else if (m_keyMap[Qt::Key_Control] || m_keyMap[Qt::Key_Meta]) {
        m_camera.translate(CameraDirection::DOWN, deltaTime);
    } else if (m_keyMap[Qt::Key_G]) {
        m_distanceBezier += deltaTime / 5.f;
        if (m_distanceBezier > 1) {
            m_distanceBezier = 0;
        }
        m_camera.moveAlongBezierCurve(m_distanceBezier);
    }

    update(); // asks for a PaintGL() call to occur
}
