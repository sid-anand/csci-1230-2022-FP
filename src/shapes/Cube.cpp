#include "Cube.h"

void Cube::updateParams(int param1) {
    m_vertexData = std::vector<float>();
    m_param1 = param1;
    setVertexData();
}

void Cube::makeTile(glm::vec3 topLeft,
                    glm::vec3 topRight,
                    glm::vec3 bottomLeft,
                    glm::vec3 bottomRight) {
    glm::vec3 topLeftNorm = glm::normalize(glm::cross(bottomLeft - topLeft, topRight - topLeft));
    glm::vec3 bottomLeftNorm = glm::normalize(glm::cross(bottomRight - bottomLeft, topLeft - bottomLeft));
    glm::vec3 bottomRightNorm = glm::normalize(glm::cross(topRight - bottomRight, bottomLeft - bottomRight));
    glm::vec3 topRightNorm = glm::normalize(glm::cross(topLeft - topRight, bottomRight - topRight));

    insertVec3(m_vertexData, topLeft);
    insertVec3(m_vertexData, topLeftNorm);
    insertVec3(m_vertexData, bottomLeft);
    insertVec3(m_vertexData, bottomLeftNorm);
    insertVec3(m_vertexData, bottomRight);
    insertVec3(m_vertexData, bottomRightNorm);

    insertVec3(m_vertexData, topLeft);
    insertVec3(m_vertexData, topLeftNorm);
    insertVec3(m_vertexData, bottomRight);
    insertVec3(m_vertexData, bottomRightNorm);
    insertVec3(m_vertexData, topRight);
    insertVec3(m_vertexData, topRightNorm);
}

void Cube::makeFace(glm::vec3 topLeft,
                    glm::vec3 topRight,
                    glm::vec3 bottomLeft,
                    glm::vec3 bottomRight) {
    glm::vec3 xInc = (topRight - topLeft) / float(m_param1);
    glm::vec3 yInc = (bottomLeft - topLeft) / float(m_param1);
    for (int i = 0; i < m_param1; i++) {
        for (int j = 0; j < m_param1; j++) {
            glm::vec3 tileTopLeft = topLeft + float(j) * xInc + float(i) * yInc;
            makeTile(tileTopLeft, tileTopLeft + xInc, tileTopLeft + yInc, tileTopLeft + xInc + yInc);
        }
    }
}

void Cube::setVertexData() {
    // xy plane (z faces)
    makeFace(glm::vec3(-0.5f, 0.5f, 0.5f),
          glm::vec3(0.5f, 0.5f, 0.5f),
          glm::vec3(-0.5f, -0.5f, 0.5f),
          glm::vec3(0.5f, -0.5f, 0.5f));
    makeFace(glm::vec3(0.5f, 0.5f, -0.5f),
          glm::vec3(-0.5f, 0.5f, -0.5f),
          glm::vec3(0.5f, -0.5f, -0.5f),
          glm::vec3(-0.5f, -0.5f, -0.5f));

    // yz plane (x faces)
    makeFace(glm::vec3(-0.5f, 0.5f, -0.5f),
          glm::vec3(-0.5f, 0.5f, 0.5f),
          glm::vec3(-0.5f, -0.5f, -0.5f),
          glm::vec3(-0.5f, -0.5f, 0.5f));
    makeFace(glm::vec3(0.5f, 0.5f, 0.5f),
          glm::vec3(0.5f, 0.5f, -0.5f),
          glm::vec3(0.5f, -0.5f, 0.5f),
          glm::vec3(0.5f, -0.5f, -0.5f));

    // xz plane (y faces)
    makeFace(glm::vec3(-0.5f, 0.5f, -0.5f),
          glm::vec3(0.5f, 0.5f, -0.5f),
          glm::vec3(-0.5f, 0.5f, 0.5f),
          glm::vec3(0.5f, 0.5f, 0.5f));
    makeFace(glm::vec3(-0.5f, -0.5f, 0.5f),
          glm::vec3(0.5f, -0.5f, 0.5f),
          glm::vec3(-0.5f, -0.5f, -0.5f),
          glm::vec3(0.5f, -0.5f, -0.5f));
}

void Cube::insertVec3(std::vector<float> &data, glm::vec3 v) {
    data.push_back(v.x);
    data.push_back(v.y);
    data.push_back(v.z);
}
