#include "shapes/Building.h"
#include <vector>
#include <GL/glew.h>
#include <glm/glm.hpp>

class FloorPlan {
    public:
        std::vector<Building> buildings;
        const int buildingSize = 1;
        const float streetSize = 0.75f;
        const int maxHeight = 10;
        const float gap = 0.25f;
        const float blockSize = float(2 * buildingSize) + gap;
        float gridSize;
        GLuint baseVBO;
        GLuint baseVAO;


        FloorPlan(float size, int numTextures) {
            gridSize = size;

            for (float bi = 0.f; bi <= gridSize; bi += (blockSize + streetSize)) {
                for (float bk = 0.f; bk >= -gridSize; bk -= (blockSize + streetSize)) {
                    for (float k = 0.f; k >= -blockSize; k -= (float(buildingSize) + gap)) {
                        for (float i = 0.f; i <= blockSize; i += ((float(buildingSize) + gap))) {
                            float buildingHeight = arc4random() % maxHeight + 1;
                            Building building = Building();
                            building.updateParams(1, buildingSize, buildingHeight, buildingSize, bi+i, bk+k, arc4random() % numTextures);
                            buildings.push_back(building);
                        }
                    }
                }
            }
//            Building building = Building();
//            building.updateParams(5, 1, 10, 1, 1.25, -3.0f, arc4random() % numTextures);
//            buildings.push_back(building);
        }


		


};
