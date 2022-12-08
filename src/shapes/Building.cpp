#include "Building.h"
#include "Cube.h"
#include "shapes/Cylinder.h"
#include <iostream>

void Building::updateParams(int param1, int size, int height, int depth, float x, float z, int texture) {
    m_vertexData = std::vector<float>();
    m_param1 = param1;
    m_size = size - 1;
    m_depth = depth - 1;
    m_height = 10;
    m_x = x + 0.25f;
    m_z = z + 0.25f;
    m_texture = texture;
    makeBuilding();
}

void Building::makeBuilding() {
    Cube cube = Cube();
    Cylinder cylinder = Cylinder();

    m_vertexData.clear();
    bool cluster = arc4random() % 2;
    if (cluster) {
        for (float i = 0.f; i < float(m_size); i++) {
            for (float k = 0.f; k < float(m_depth); k++) {
                int towerHeight = arc4random() % m_height + 1;
                float xjitter = float(arc4random() % 50)/100.f;
                float zjitter = float(arc4random() % 50)/100.f;
                i += xjitter;
                k += zjitter;

                bool cylindrical = arc4random() % 2;

                if (cylindrical) {
                    for (float j = 0.f; j < float(towerHeight); j++) {
                        int param2 = arc4random() % 3 + 3;
                        cylinder.updateParams(1, param2, i + m_x, j, k + m_z, 0.5f);
                        std::vector<float> cylinder_data = cylinder.getMesh();
                        m_vertexData.insert(m_vertexData.end(), cylinder_data.begin(), cylinder_data.end());
                    }
                } else {
                    for (float j = 0.f; j < float(towerHeight); j++) {
                        cube.updateParams(m_param1, i + m_x, j, k + m_z);
                        std::vector<float> cube_data = cube.getMesh();
                        m_vertexData.insert(m_vertexData.end(), cube_data.begin(), cube_data.end());
                    }
                }
            }
        }
    } else {
        for (float i = 0.f; i < float(m_size); i++) {
            for (float k = 0.f; k < float(m_depth); k++) {
                int towerHeight = arc4random() % m_height + 1;
                for (float j = 0.f; j < float(towerHeight); j++) {
                    cube.updateParams(m_param1, i + m_x, j, k + m_z);
                    std::vector<float> cube_data = cube.getMesh();
                    m_vertexData.insert(m_vertexData.end(), cube_data.begin(), cube_data.end());
                }
            }
        }
    }
}

void Building::insertVec3(std::vector<float> &data, glm::vec3 v) {
    data.push_back(v.x);
    data.push_back(v.y);
    data.push_back(v.z);
}
