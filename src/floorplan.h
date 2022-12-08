#include "shapes/Building.h"
#include <iostream>
#include <ostream>
#include <vector>
#include <GL/glew.h>
#include <glm/glm.hpp>

class FloorPlan {
    public:
        std::vector<Building> buildings;
        const int buildingSize = 4;
        const float streetSize = 0.5f;
        const int maxHeight = 10;
        const float gap = 0.5f;
//        const float blockSize = 4;
        float gridSize;
        GLuint baseVBO;
        GLuint baseVAO;
        const int maxBlockSize = 4;
        std::vector<int> blockSizesX;
        std::vector<int> blockSizesZ;


        FloorPlan(float size, int numTextures) {
            blockSizesX.clear();
            blockSizesZ.clear();

            gridSize = size;
            // randomly generate x dimension from 1-4
            // randomly generate z dimension from 1-4
            // if there's enough space in that dimension, use it, otherwise use 1
            int currentSizeX = 0;
            while (currentSizeX < gridSize) {
                int blockSize = (arc4random() % maxBlockSize) + 1;
                while (currentSizeX + blockSize > gridSize) {
                    blockSize -= 1;
                }
                currentSizeX += blockSize;
                blockSizesX.push_back(blockSize);
            }
            int currentSizeZ = 0;
            while (currentSizeZ < gridSize) {
                int blockSize = (arc4random() % maxBlockSize) + 1;
                while (currentSizeZ + blockSize > gridSize) {
                    blockSize -= 1;
                }
                currentSizeZ += blockSize;
                blockSizesZ.push_back(blockSize);
            }
            // vector 1 is x direction, its like [1, 4, 3, 1, 1]
            // vector 2 is z direction, like     [2, 2, 4, 2]

            // xAccumulator and zAccumulator are basically the coordinates of where the block is
            // note it starts at -streetSize rather than 0 because we add streetSize + the buildingSize in the for loop,
            // and we don't have a street for the first block in either dimension
            for (int i = 0; i < blockSizesX.size(); i++) {
                float xAccumulator = -streetSize;
                float buildingDimensionX = blockSizesX[i];
                xAccumulator += buildingDimensionX + streetSize;
                for (int j = 0; j < blockSizesZ.size(); j++) {
                    float zAccumulator = streetSize;
                    float buildingHeight = arc4random() % maxHeight + 1;
                    Building building = Building();
                    float buildingDimensionZ = blockSizesZ[j];
                    zAccumulator -= buildingDimensionZ + streetSize;
//                    std::cout<< "blah" <<std::endl;
//                    std::cout<< i <<std::endl;
//                    std::cout<< j <<std::endl;
//                    std::cout<< xAccumulator <<std::endl;
//                    std::cout<< zAccumulator <<std::endl;
                    building.updateParams(1, buildingDimensionX, buildingHeight, buildingDimensionZ, xAccumulator, zAccumulator, arc4random() % numTextures);
                    buildings.push_back(building);
                }
            }
//            for (float bi = 0.f; bi <= gridSize; bi += (blockSize + streetSize)) {
//                for (float bk = 0.f; bk >= -gridSize; bk -= (blockSize + streetSize)) {
//                    float buildingHeight = arc4random() % maxHeight + 1;
//                    Building building = Building();
//                    building.updateParams(1, blockSize, buildingHeight, blockSize, bi, bk, arc4random() % numTextures);
//                    buildings.push_back(building);
////                    for (float k = 0.f; k >= -blockSize; k -= (float(buildingSize) + gap)) {
////                        for (float i = 0.f; i <= blockSize; i += ((float(buildingSize) + gap))) {
////                            float buildingHeight = arc4random() % maxHeight + 1;
////                            Building building = Building();
////                            building.updateParams(1, buildingSize, buildingHeight, buildingSize, bi+i, bk+k, arc4random() % numTextures);
////                            buildings.push_back(building);
////                        }
////                    }
//                }
//            }
        }





};
