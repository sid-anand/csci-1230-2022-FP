//#include "Cube.h"

//void Cube::updateParams(int param1, float x, float y, float z) {
//    m_vertexData = std::vector<float>();
//    m_param1 = param1;
//    m_x = x;
//    m_y = y;
//    m_z = z;
//    setVertexData();
//}

//void Cube::makeTile(glm::vec3 topLeft,
//                    glm::vec3 topRight,
//                    glm::vec3 bottomLeft,
//                    glm::vec3 bottomRight) {
//    glm::vec3 topLeftNorm = glm::normalize(glm::cross(bottomLeft - topLeft, topRight - topLeft));
//    glm::vec3 bottomLeftNorm = glm::normalize(glm::cross(bottomRight - bottomLeft, topLeft - bottomLeft));
//    glm::vec3 bottomRightNorm = glm::normalize(glm::cross(topRight - bottomRight, bottomLeft - bottomRight));
//    glm::vec3 topRightNorm = glm::normalize(glm::cross(topLeft - topRight, bottomRight - topRight));

//    insertVec3(m_vertexData, topLeft);
//    insertVec3(m_vertexData, topLeftNorm);
//    insertVec2(m_vertexData, glm::vec2{0, 1});
//    insertVec3(m_vertexData, bottomLeft);
//    insertVec3(m_vertexData, bottomLeftNorm);
//    insertVec2(m_vertexData, glm::vec2{0, 0});
//    insertVec3(m_vertexData, bottomRight);
//    insertVec3(m_vertexData, bottomRightNorm);
//    insertVec2(m_vertexData, glm::vec2{1, 0});

//    insertVec3(m_vertexData, topLeft);
//    insertVec3(m_vertexData, topLeftNorm);
//    insertVec2(m_vertexData, glm::vec2{1, 1});
//    insertVec3(m_vertexData, bottomRight);
//    insertVec3(m_vertexData, bottomRightNorm);
//    insertVec2(m_vertexData, glm::vec2{0, 1});
//    insertVec3(m_vertexData, topRight);
//    insertVec3(m_vertexData, topRightNorm);
//    insertVec2(m_vertexData, glm::vec2{1, 0});
//}

//void Cube::makeFace(glm::vec3 topLeft,
//                    glm::vec3 topRight,
//                    glm::vec3 bottomLeft,
//                    glm::vec3 bottomRight) {
//    glm::vec3 xInc = (topRight - topLeft) / float(m_param1);
//    glm::vec3 yInc = (bottomLeft - topLeft) / float(m_param1);
//    for (int i = 0; i < m_param1; i++) {
//        for (int j = 0; j < m_param1; j++) {
//            glm::vec3 tileTopLeft = topLeft + float(j) * xInc + float(i) * yInc;
//            makeTile(tileTopLeft, tileTopLeft + xInc, tileTopLeft + yInc, tileTopLeft + xInc + yInc);
//        }
//    }
//}

//void Cube::setVertexData() {
//    // xy plane (z faces)
////    makeFace(glm::vec3(-0.5f, 0.5f, 0.5f),
////          glm::vec3(0.5f, 0.5f, 0.5f),
////          glm::vec3(-0.5f, -0.5f, 0.5f),
////          glm::vec3(0.5f, -0.5f, 0.5f));
//    makeFace(glm::vec3(m_x,       m_y + 1.f, m_z + 1.f),
//             glm::vec3(m_x + 1.f, m_y + 1.f, m_z + 1.f),
//             glm::vec3(m_x,       m_y,       m_z + 1.f),
//             glm::vec3(m_x + 1.f, m_y,       m_z + 1.f));
////    makeFace(glm::vec3(0.5f, 0.5f, -0.5f),
////          glm::vec3(-0.5f, 0.5f, -0.5f),
////          glm::vec3(0.5f, -0.5f, -0.5f),
////          glm::vec3(-0.5f, -0.5f, -0.5f));
//    makeFace(glm::vec3(m_x,       m_y + 1.f, m_z + 1),
//             glm::vec3(m_x + 1.f, m_y + 1.f, m_z + 1.f),
//             glm::vec3(m_x,       m_y,       m_z + 1.f),
//             glm::vec3(m_x + 1.f, m_y,       m_z + 1.f));

//    // yz plane (x faces)
//    makeFace(glm::vec3(-0.5f, 0.5f, -0.5f),
//          glm::vec3(-0.5f, 0.5f, 0.5f),
//          glm::vec3(-0.5f, -0.5f, -0.5f),
//          glm::vec3(-0.5f, -0.5f, 0.5f));
//    makeFace(glm::vec3(0.5f, 0.5f, 0.5f),
//          glm::vec3(0.5f, 0.5f, -0.5f),
//          glm::vec3(0.5f, -0.5f, 0.5f),
//          glm::vec3(0.5f, -0.5f, -0.5f));

//    // xz plane (y faces)
//    makeFace(glm::vec3(-0.5f, 0.5f, -0.5f),
//          glm::vec3(0.5f, 0.5f, -0.5f),
//          glm::vec3(-0.5f, 0.5f, 0.5f),
//          glm::vec3(0.5f, 0.5f, 0.5f));
//    makeFace(glm::vec3(-0.5f, -0.5f, 0.5f),
//          glm::vec3(0.5f, -0.5f, 0.5f),
//          glm::vec3(-0.5f, -0.5f, -0.5f),
//          glm::vec3(0.5f, -0.5f, -0.5f));
//}

//void Cube::insertVec2(std::vector<float> &data, glm::vec2 v) {
//    data.push_back(v.x);
//    data.push_back(v.y);
//}

//void Cube::insertVec3(std::vector<float> &data, glm::vec3 v) {
//    data.push_back(v.x);
//    data.push_back(v.y);
//    data.push_back(v.z);
//}

#include "Cube.h"
#include <iostream>

void Cube::updateParams(int param1, float x, float y, float z) {
    m_vertexData = std::vector<float>();
    m_param1 = param1;
    m_x = x;
    m_y = y;
    m_z = z;
    setVertexData();
}

void Cube::makeTile(glm::vec3 topLeft,
                    glm::vec3 topRight,
                    glm::vec3 bottomLeft,
                    glm::vec3 bottomRight) {
    glm::vec3 norm1 = glm::normalize(glm::cross((bottomLeft - bottomRight), (bottomLeft - topLeft)));
    glm::vec3 norm2 = glm::normalize(glm::cross((topRight - topLeft), (topRight - bottomRight)));

    insertVec3(m_vertexData, topLeft);
    insertVec3(m_vertexData, norm1);
    insertVec2(m_vertexData, glm::vec2{0.f, 1.f});
    insertVec3(m_vertexData, bottomLeft);
    insertVec3(m_vertexData, norm1);
    insertVec2(m_vertexData, glm::vec2{0.f, 0.f});
    insertVec3(m_vertexData, bottomRight);
    insertVec3(m_vertexData, norm1);
    insertVec2(m_vertexData, glm::vec2{1.f, 0.f});
    insertVec3(m_vertexData, topRight);
    insertVec3(m_vertexData, norm2);
    insertVec2(m_vertexData, glm::vec2{1.f, 1.f});
    insertVec3(m_vertexData, topLeft);
    insertVec3(m_vertexData, norm2);
    insertVec2(m_vertexData, glm::vec2{0.f, 1.f});
    insertVec3(m_vertexData, bottomRight);
    insertVec3(m_vertexData, norm2);
    insertVec2(m_vertexData, glm::vec2{1.f, 0.f});
}

void Cube::makeFace(glm::vec3 topLeft,
                    glm::vec3 topRight,
                    glm::vec3 bottomLeft,
                    glm::vec3 bottomRight) {
    glm::vec3 normal = glm::normalize(glm::cross((bottomLeft - bottomRight), (bottomLeft - topLeft)));

    int axis;
    for (int i = 0; i < 3; i ++) {
        if (normal[i] != 0) {
            axis = i;
        }
    }

    float width;
    float height;


    switch (axis) {
        case 0 : {
            width = topRight[2] - topLeft[2];
            height = topRight[1] - bottomRight[1];

            for (int r = 0; r < m_param1; r++) {
                for (int c = 0; c < m_param1; c++) {
                    glm::vec3 transform_vec = glm::vec3{0, -r*(height / m_param1), c*(width / m_param1)};
                    glm::vec3 horizontal_transform = glm::vec3 {0, 0, (width / m_param1)};
                    glm::vec3 vertical_transform = glm::vec3 {0, -(height / m_param1), 0};

                    glm::vec3 newTopLeft = topLeft + transform_vec;
                    glm::vec3 newTopRight = newTopLeft + horizontal_transform;
                    glm::vec3 newBottomLeft = newTopLeft + vertical_transform;
                    glm::vec3 newBottomRight = newTopRight + vertical_transform;

                    makeTile(newTopLeft, newTopRight, newBottomLeft, newBottomRight);
                }
            }

            break;
        }
        case 1 : {
            width = topRight[0] - topLeft[0];
            height = topRight[2] - bottomRight[2];

            for (int r = 0; r < m_param1; r++) {
                for (int c = 0; c < m_param1; c++) {

                    glm::vec3 transform_vec = glm::vec3{c*(width / m_param1), 0, -r*(height / m_param1)};
                    glm::vec3 horizontal_transform = glm::vec3 {(width / m_param1), 0, 0};
                    glm::vec3 vertical_transform = glm::vec3 {0, 0, -(height / m_param1)};

                    glm::vec3 newTopLeft = topLeft + transform_vec;
                    glm::vec3 newTopRight = newTopLeft + horizontal_transform;
                    glm::vec3 newBottomLeft = newTopLeft + vertical_transform;
                    glm::vec3 newBottomRight = newTopRight + vertical_transform;

                    makeTile(newTopLeft, newTopRight, newBottomLeft, newBottomRight);
                }
            }
            break;
        }
        case 2 : {

            width = topRight[0] - topLeft[0];
            height = topRight[1] - bottomRight[1];

            for (int r = 0; r < m_param1; r++) {
                for (int c = 0; c < m_param1; c++) {

                    glm::vec3 transform_vec = glm::vec3{c*(width / m_param1), -r*(height / m_param1), 0};
                    glm::vec3 horizontal_transform = glm::vec3 {(width / m_param1), 0, 0};
                    glm::vec3 vertical_transform = glm::vec3 {0, -(height / m_param1), 0};

                    glm::vec3 newTopLeft = topLeft + transform_vec;
                    glm::vec3 newTopRight = newTopLeft + horizontal_transform;
                    glm::vec3 newBottomLeft = newTopLeft + vertical_transform;
                    glm::vec3 newBottomRight = newTopRight + vertical_transform;

                    makeTile(newTopLeft, newTopRight, newBottomLeft, newBottomRight);
                }
            }
            break;
        }
    }
}

void Cube::setVertexData() {

     makeFace(glm::vec3(m_x,       m_y + 1.f, m_z + 1.f),
              glm::vec3(m_x + 1.f, m_y + 1.f, m_z + 1.f),
              glm::vec3(m_x,       m_y,       m_z + 1.f),
              glm::vec3(m_x + 1.f, m_y,       m_z + 1.f));

     makeFace(glm::vec3(m_x,       m_y,       m_z),
              glm::vec3(m_x + 1.f, m_y,       m_z),
              glm::vec3(m_x,       m_y + 1.f, m_z),
              glm::vec3(m_x + 1.f, m_y + 1.f, m_z));

     makeFace(glm::vec3(m_x + 1.f, m_y,       m_z),
              glm::vec3(m_x + 1.f, m_y,       m_z + 1.f),
              glm::vec3(m_x + 1.f, m_y + 1.f, m_z),
              glm::vec3(m_x + 1.f, m_y + 1.f, m_z + 1.f));

     makeFace(glm::vec3(m_x,       m_y + 1.f, m_z),
              glm::vec3(m_x,       m_y + 1.f, m_z + 1.f),
              glm::vec3(m_x,       m_y,       m_z),
              glm::vec3(m_x,       m_y,       m_z + 1.f));

     makeFace(glm::vec3(m_x,       m_y + 1.f, m_z),
              glm::vec3(m_x + 1.f, m_y + 1.f, m_z),
              glm::vec3(m_x,       m_y + 1.f, m_z + 1.f),
              glm::vec3(m_x + 1.f, m_y + 1.f, m_z + 1.f));

     makeFace(glm::vec3(m_x,       m_y,       m_z + 1.f),
              glm::vec3(m_x + 1.f, m_y,       m_z + 1.f),
              glm::vec3(m_x,       m_y,       m_z),
              glm::vec3(m_x + 1.f, m_y,       m_z));

}

void Cube::insertVec2(std::vector<float> &data, glm::vec2 v) {
    data.push_back(v.x);
    data.push_back(v.y);
}

void Cube::insertVec3(std::vector<float> &data, glm::vec3 v) {
    data.push_back(v.x);
    data.push_back(v.y);
    data.push_back(v.z);
}
