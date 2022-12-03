#include "realtime.h"

#include <QCoreApplication>
#include <QMouseEvent>
#include <QKeyEvent>
#include <iostream>
#include "settings.h"
#include "utils/sceneparser.h"
#include "utils/shaderloader.h"

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

    glDeleteBuffers(1, &m_cone_vbo);
    glDeleteBuffers(1, &m_cube_vbo);
    glDeleteBuffers(1, &m_cylinder_vbo);
    glDeleteBuffers(1, &m_sphere_vbo);

    glDeleteVertexArrays(1, &m_cone_vao);
    glDeleteVertexArrays(1, &m_cube_vao);
    glDeleteVertexArrays(1, &m_cylinder_vao);
    glDeleteVertexArrays(1, &m_sphere_vao);

    glDeleteProgram(m_phong_shader);
    glDeleteProgram(m_texture_shader);

    glDeleteVertexArrays(1, &m_fullscreen_vao);
    glDeleteBuffers(1, &m_fullscreen_vbo);

    glDeleteTextures(1, &m_fbo_texture);
    glDeleteRenderbuffers(1, &m_fbo_renderbuffer);
    glDeleteFramebuffers(1, &m_fbo);

    this->doneCurrent();
}

void Realtime::initializeGL() {
    m_devicePixelRatio = this->devicePixelRatio();

    m_defaultFBO = 2;
    m_screen_width = size().width() * m_devicePixelRatio;
    m_screen_height = size().height() * m_devicePixelRatio;
    m_fbo_width = m_screen_width;
    m_fbo_height = m_screen_height;

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
    m_texture_shader = ShaderLoader::createShaderProgram(":/resources/shaders/texture.vert", ":/resources/shaders/texture.frag");

    // Initialize meshes and VBOs/VAOs
    m_cone.updateParams(settings.shapeParameter1, settings.shapeParameter2);
    setupVBOVAO(&m_cone_vbo, &m_cone_vao, m_cone.getMesh());

    m_cube.updateParams(settings.shapeParameter1);
    setupVBOVAO(&m_cube_vbo, &m_cube_vao, m_cube.getMesh());

    m_cylinder.updateParams(settings.shapeParameter1, settings.shapeParameter2);
    setupVBOVAO(&m_cylinder_vbo, &m_cylinder_vao, m_cylinder.getMesh());

    m_sphere.updateParams(settings.shapeParameter1, settings.shapeParameter2);
    setupVBOVAO(&m_sphere_vbo, &m_sphere_vao, m_sphere.getMesh());

    // Generate mesh for fullscreen quad
    std::vector<GLfloat> fullscreen_quad_data =
    {
      -1.0f,  1.0f, 0.0f, // top left
      0.0f, 1.0f,
      -1.0f, -1.0f, 0.0f, // bottom left
      0.0f, 0.0f,
      1.0f, -1.0f, 0.0f, // bottom right
      1.0f, 0.0f,
      1.0f,  1.0f, 0.0f, // top right
      1.0f, 1.0f,
      -1.0f,  1.0f, 0.0f, // top left
      0.0f, 1.0f,
      1.0f, -1.0f, 0.0f, // bottom right
      1.0f, 0.0f
    };

    // Generate and bind a VBO and a VAO for a fullscreen quad
    glGenBuffers(1, &m_fullscreen_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, m_fullscreen_vbo);
    glBufferData(GL_ARRAY_BUFFER, fullscreen_quad_data.size() * sizeof(GLfloat), fullscreen_quad_data.data(), GL_STATIC_DRAW);
    glGenVertexArrays(1, &m_fullscreen_vao);
    glBindVertexArray(m_fullscreen_vao);

    // Set VAO attributes for fullscreen quad
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), reinterpret_cast<void*>(0));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), reinterpret_cast<void*>(sizeof(GLfloat) * 3));

    // Unbind the fullscreen quad's VBO and VAO
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    makeFBO();
}

void Realtime::paintGL() {
    // Students: anything requiring OpenGL calls every frame should be done here

    // Bind and clear the FBO
    glBindFramebuffer(GL_FRAMEBUFFER, m_fbo);
    glViewport(0, 0, m_fbo_width, m_fbo_height);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Bind the shader
    glUseProgram(m_phong_shader);

    // Declare global uniforms
    glUniformMatrix4fv(glGetUniformLocation(m_phong_shader, "view"), 1, GL_FALSE, &m_camera.getViewMatrix()[0][0]);
    glUniformMatrix4fv(glGetUniformLocation(m_phong_shader, "proj"), 1, GL_FALSE, &m_camera.getProjMatrix()[0][0]);
    glUniform4fv(glGetUniformLocation(m_phong_shader, "cameraPos"), 1, &m_camera.getPos()[0]);
    glUniform1f(glGetUniformLocation(m_phong_shader, "ka"), m_renderData.globalData.ka);
    glUniform1f(glGetUniformLocation(m_phong_shader, "kd"), m_renderData.globalData.kd);
    glUniform1f(glGetUniformLocation(m_phong_shader, "ks"), m_renderData.globalData.ks);

    for (int i = 0; i < m_renderData.lights.size(); i++) {
        SceneLightData &light = m_renderData.lights[i];
            std::string iStr = std::to_string(i);
            GLuint lightTypeID = glGetUniformLocation(m_phong_shader, ("lights[" + iStr + "].type").c_str());
            glUniform1i(lightTypeID, int(light.type));
            GLuint lightColorID = glGetUniformLocation(m_phong_shader, ("lights[" + iStr + "].color").c_str());
            glUniform4fv(lightColorID, 1, &light.color[0]);
            GLuint lightFunctionID = glGetUniformLocation(m_phong_shader, ("lights[" + iStr + "].function").c_str());
            glUniform3fv(lightFunctionID, 1, &light.function[0]);
            GLuint lightPosID = glGetUniformLocation(m_phong_shader, ("lights[" + iStr + "].pos").c_str());
            glUniform4fv(lightPosID, 1, &light.pos[0]);
            GLuint lightDirID = glGetUniformLocation(m_phong_shader, ("lights[" + iStr + "].dir").c_str());
            glUniform4fv(lightDirID, 1, &light.dir[0]);
            GLuint lightPenumbraID = glGetUniformLocation(m_phong_shader, ("lights[" + iStr + "].penumbra").c_str());
            glUniform1f(lightPenumbraID, light.penumbra);
            GLuint lightAngleID = glGetUniformLocation(m_phong_shader, ("lights[" + iStr + "].angle").c_str());
            glUniform1f(lightAngleID, light.angle);
    }

    // Only want to include the lights we're handling
    glUniform1i(glGetUniformLocation(m_phong_shader, "numLights"), m_renderData.lights.size());

    // Declare primitive-specific uniforms, and bind and draw the VAOs
    for (RenderShapeData &shape : m_renderData.shapes) {
        SceneMaterial mat = shape.primitive.material;

        glUniformMatrix4fv(glGetUniformLocation(m_phong_shader, "model"), 1, GL_FALSE, &shape.ctm[0][0]);
        glUniform4fv(glGetUniformLocation(m_phong_shader, "ambientColor"), 1, &mat.cAmbient[0]);
        glUniform4fv(glGetUniformLocation(m_phong_shader, "diffuseColor"), 1, &mat.cDiffuse[0]);
        glUniform4fv(glGetUniformLocation(m_phong_shader, "specularColor"), 1, &mat.cSpecular[0]);
        glUniform1f(glGetUniformLocation(m_phong_shader, "shininess"), mat.shininess);

        if (shape.primitive.type == PrimitiveType::PRIMITIVE_CONE) {
            glBindVertexArray(m_cone_vao);
            glDrawArrays(GL_TRIANGLES, 0, m_cone.getMesh().size() / 6);
        } else if (shape.primitive.type == PrimitiveType::PRIMITIVE_CUBE) {
            glBindVertexArray(m_cube_vao);
            glDrawArrays(GL_TRIANGLES, 0, m_cube.getMesh().size() / 6);
        } else if (shape.primitive.type == PrimitiveType::PRIMITIVE_CYLINDER) {
            glBindVertexArray(m_cylinder_vao);
            glDrawArrays(GL_TRIANGLES, 0, m_cylinder.getMesh().size() / 6);
        } else if (shape.primitive.type == PrimitiveType::PRIMITIVE_SPHERE) {
            glBindVertexArray(m_sphere_vao);
            glDrawArrays(GL_TRIANGLES, 0, m_sphere.getMesh().size() / 6);
        }
    }

    // Unbind the VAO
    glBindVertexArray(0);

    // Unbind the shader
    glUseProgram(0);

    // Bind and clear the default FBO
    glBindFramebuffer(GL_FRAMEBUFFER, m_defaultFBO);
    glViewport(0, 0, m_screen_width, m_screen_height);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Call paintTexture to draw our FBO color attachment texture
    paintTexture(m_fbo_texture);
}

void Realtime::resizeGL(int w, int h) {
    // Tells OpenGL how big the screen is
    glViewport(0, 0, size().width() * m_devicePixelRatio, size().height() * m_devicePixelRatio);

    // Students: anything requiring OpenGL calls when the program starts should be done here

    glDeleteTextures(1, &m_fbo_texture);
    glDeleteRenderbuffers(1, &m_fbo_renderbuffer);
    glDeleteFramebuffers(1, &m_fbo);

    m_screen_width = size().width() * m_devicePixelRatio;
    m_screen_height = size().height() * m_devicePixelRatio;
    m_fbo_width = m_screen_width;
    m_fbo_height = m_screen_height;

    makeFBO();

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

    m_cone.updateParams(settings.shapeParameter1, settings.shapeParameter2);
    m_cube.updateParams(settings.shapeParameter1);
    m_cylinder.updateParams(settings.shapeParameter1, settings.shapeParameter2);
    m_sphere.updateParams(settings.shapeParameter1, settings.shapeParameter2);

    makeCurrent();
    updateVBO(m_cone_vbo, m_cone.getMesh());
    updateVBO(m_cube_vbo, m_cube.getMesh());
    updateVBO(m_cylinder_vbo, m_cylinder.getMesh());
    updateVBO(m_sphere_vbo, m_sphere.getMesh());
    doneCurrent();

    update(); // asks for a PaintGL() call to occur
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
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 6, 0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 6, reinterpret_cast<void*>(sizeof(GLfloat) * 3));

    // Unbind VBO and VAO
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void Realtime::updateVBO(GLuint vbo, std::vector<GLfloat> mesh) {
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * mesh.size(), mesh.data(), GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void Realtime::makeFBO() {
    glGenTextures(1, &m_fbo_texture);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, m_fbo_texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_fbo_width, m_fbo_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glBindTexture(GL_TEXTURE_2D, 0);

    glGenRenderbuffers(1, &m_fbo_renderbuffer);
    glBindRenderbuffer(GL_RENDERBUFFER, m_fbo_renderbuffer);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, m_fbo_width, m_fbo_height);
    glBindRenderbuffer(GL_RENDERBUFFER, 0);

    glGenFramebuffers(1, &m_fbo);
    glBindFramebuffer(GL_FRAMEBUFFER, m_fbo);

    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_fbo_texture, 0);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_fbo_renderbuffer);

    glBindFramebuffer(GL_FRAMEBUFFER, m_defaultFBO);
}

void Realtime::paintTexture(GLuint texture) {
    glUseProgram(m_texture_shader);
    glUniform1i(glGetUniformLocation(m_texture_shader, "tex"), 0);
    glUniform1i(glGetUniformLocation(m_texture_shader, "width"), m_fbo_width);
    glUniform1i(glGetUniformLocation(m_texture_shader, "height"), m_fbo_height);
    glUniform1i(glGetUniformLocation(m_texture_shader, "perPixelFilter"), settings.perPixelFilter);
    glUniform1i(glGetUniformLocation(m_texture_shader, "kernelBasedFilter"), settings.kernelBasedFilter);

    glBindVertexArray(m_fullscreen_vao);
    glBindTexture(GL_TEXTURE_2D, texture);

    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindTexture(GL_TEXTURE_2D, 0);
    glBindVertexArray(0);
    glUseProgram(0);
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
    }

    update(); // asks for a PaintGL() call to occur
}
