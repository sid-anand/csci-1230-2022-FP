#include "Building.h"
#include "Cube.h"
#include <iostream>

void Building::updateParams(int param1, int size, int height, float x, float z) {
    m_vertexData = std::vector<float>();
    m_param1 = param1;
    m_size = size;
    m_height = height;
    m_x = x;
    m_z = z;
    makeBuilding();
}

void Building::makeBuilding() {
    Cube cube = Cube();

    m_vertexData.clear();

    for (float i = 0.f; i < 1.f + (1.f * (m_size - 1)); i++) {
        for (float j = 0.f; j < 1.f  + (1.f * (m_height - 1)); j++) {
            cube.updateParams(m_param1, i + m_x, j, m_z);
            std::vector<float> cube_data = cube.getMesh();
            m_vertexData.insert(m_vertexData.end(), cube_data.begin(), cube_data.end());
        }
    }
}

void Building::insertVec3(std::vector<float> &data, glm::vec3 v) {
    data.push_back(v.x);
    data.push_back(v.y);
    data.push_back(v.z);
}
