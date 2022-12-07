#include "Cylinder.h"

void Cylinder::updateParams(int param1, int param2, float x, float y, float z, float radius) {
    m_vertexData = std::vector<float>();
    m_param1 = param1;
    m_param2 = param2;
    m_x = x;
    m_y = y;
    m_z = z;
    m_radius = radius;
    setVertexData();
}

void Cylinder::makeTile(glm::vec3 topLeft,
                        glm::vec3 topRight,
                        glm::vec3 bottomLeft,
                        glm::vec3 bottomRight,
                        int cap) {
    glm::vec3 topLeftNorm;
    glm::vec3 bottomLeftNorm;
    glm::vec3 bottomRightNorm;
    glm::vec3 topRightNorm;
    if (cap == 0) {
        topLeftNorm = glm::normalize(glm::vec3(topLeft.x, 0, topLeft.z));
        bottomLeftNorm = glm::normalize(glm::vec3(bottomLeft.x, 0, bottomLeft.z));
        bottomRightNorm = glm::normalize(glm::vec3(bottomRight.x, 0, bottomRight.z));
        topRightNorm = glm::normalize(glm::vec3(topRight.x, 0, topRight.z));
    } else if (cap == -1) {
        topLeftNorm = glm::vec3(0, -1, 0);
        bottomLeftNorm = glm::vec3(0, -1, 0);
        bottomRightNorm = glm::vec3(0, -1, 0);
        topRightNorm = glm::vec3(0, -1, 0);
    } else {
        topLeftNorm = glm::vec3(0, 1, 0);
        bottomLeftNorm = glm::vec3(0, 1, 0);
        bottomRightNorm = glm::vec3(0, 1, 0);
        topRightNorm = glm::vec3(0, 1, 0);
    }

    insertVec3(m_vertexData, topLeft);
    insertVec3(m_vertexData, topLeftNorm);
    insertVec2(m_vertexData, glm::vec2{0.f, 1.f});
    insertVec3(m_vertexData, bottomLeft);
    insertVec3(m_vertexData, bottomLeftNorm);
    insertVec2(m_vertexData, glm::vec2{0.f, 0.f});
    insertVec3(m_vertexData, topRight);
    insertVec3(m_vertexData, topRightNorm);
    insertVec2(m_vertexData, glm::vec2{1.f, 1.f});

    insertVec3(m_vertexData, topRight);
    insertVec3(m_vertexData, topRightNorm);
    insertVec2(m_vertexData, glm::vec2{1.f, 1.f});
    insertVec3(m_vertexData, bottomLeft);
    insertVec3(m_vertexData, bottomLeftNorm);
    insertVec2(m_vertexData, glm::vec2{0.f, 0.f});
    insertVec3(m_vertexData, bottomRight);
    insertVec3(m_vertexData, bottomRightNorm);
    insertVec2(m_vertexData, glm::vec2{1.f, 0.f});
}

void Cylinder::setVertexData() {
    auto cylindrical = [](float r, float theta, float y) {
        return glm::vec3{
            r * sin(theta),
            y,
            r * cos(theta)
        };
    };

    int param1 = std::max(m_param1, 1);
    int param2 = std::max(m_param2, 3);

    float thetaStep = glm::radians(360.f / param2);
    float yStep = 1.0f / param1;
    float rStep = -0.5f / param1;

    for (int i = 0; i < param2; i++) {
        float currentTheta = i * thetaStep;
        float nextTheta = currentTheta + thetaStep;
        for (int j = 0; j < param1; j++) {
            float currentY = j * yStep - 0.5f;
            float nextY = currentY + yStep;
            float currentR = j * rStep + 0.5f;
            float nextR = currentR + rStep;
            // side
            makeTile(cylindrical(0.5f, currentTheta, nextY),
                     cylindrical(0.5f, nextTheta, nextY),
                     cylindrical(0.5f, currentTheta, currentY),
                     cylindrical(0.5f, nextTheta, currentY),
                     0);
            // top cap
            makeTile(cylindrical(nextR, currentTheta, 0.5f),
                     cylindrical(nextR, nextTheta, 0.5f),
                     cylindrical(currentR, currentTheta, 0.5f),
                     cylindrical(currentR, nextTheta, 0.5f),
                     1);
            // bottom cap
            makeTile(cylindrical(nextR, nextTheta, -0.5f),
                     cylindrical(nextR, currentTheta, -0.5f),
                     cylindrical(currentR, nextTheta, -0.5f),
                     cylindrical(currentR, currentTheta, -0.5f),
                     -1);
        }
    }
}

void Cylinder::insertVec2(std::vector<float> &data, glm::vec2 v) {
    data.push_back(v.x);
    data.push_back(v.y);
}

void Cylinder::insertVec3(std::vector<float> &data, glm::vec3 v) {
    data.push_back(v.x);
    data.push_back(v.y);
    data.push_back(v.z);
}
