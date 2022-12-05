#include <vector>
#include <GL/glew.h>
#include <glm/glm.hpp>

class FloorPlan {
public:
	std::vector<Building> buildings;
    std::vector<std::string> textures;
	const int blockSize = 6.0f;
	const int streetSize = 1.0f;
    int fullBlockSize = blockSize + 2 * streetSize;
	int gridSize;
	GLuint baseVBO;
	GLuint baseVAO;

  	
    FloorPlan(int size) {
		gridSize = size;

		for (int k = -streetSize; k > -gridSize - (fullBlockSize/2); k -= (fullBlockSize/2)) {
            for(int i = 0; i < gridSize + (fullBlockSize/2); i += streetSize){
                if (i % fullBlockSize != blockSize && i % fullBlockSize != 0) {
                    float buildingSize = rand() % 2 + 1;
                    float buildingHeight = rand() % 25 + 1;
                    Building* building = Building(i, k, buildingSize, buildingHeight, textures[rand() % textures.size()]);
                    buildings.push_back(building);

                }
            }
        }
  	}

  /*Start by drawing the blocks
  (The regions where the buildings will sit on top of)*/
  	void draw(GLuint shader) {

		glColor4f(0.0, 1.0, 0.0, 1.0f);
		glGenBuffers(1, &baseVBO);
        glBindBuffer(GL_ARRAY_BUFFER, baseVBO);
		glUseProgram(shader);
		std::vector<float> data;
        for(int j = 0; j < gridSize; j += 12){
            for(int i = 0; i < gridSize; i += 12){
				// // bottom left
				// data.push(0.0f + i);
				// data.push(0.0f);
				// data.push(0.0f - j);

				// // bottom right
				// data.push(0.0f + block + i);
				// data.push(0.0f);
				// data.push(0.0f - j);

				// // top right
				// data.push(0.0f + block + i);
				// data.push(0.0f);
				// data.push(0.0f - block - j);

				// // top left
				// data.push(0.0f + i);
				// data.push(0.0f);
				// data.push(0.0f - block - j);



				// top left
                data.push_back(0.0f + i);
                data.push_back(0.0f);
                data.push_back(0.0f - blockSize - j);

				// bottom left
                data.push_back(0.0f + i);
                data.push_back(0.0f);
                data.push_back(0.0f - j);

				// top right
                data.push_back(0.0f + blockSize + i);
                data.push_back(0.0f);
                data.push_back(0.0f - blockSize - j);

				// top right
                data.push_back(0.0f + blockSize + i);
                data.push_back(0.0f);
                data.push_back(0.0f - blockSize - j);

				// bottom left
                data.push_back(0.0f + i);
                data.push_back(0.0f);
                data.push_back(0.0f - j);

				// bottom right
                data.push_back(0.0f + blockSize + i);
                data.push_back(0.0f);
                data.push_back(0.0f - j);
			}
		}
        glBufferData(GL_ARRAY_BUFFER, sizeof(float) * data.size(), data.data(), GL_STATIC_DRAW);
        glGenVertexArrays(1, &baseVAO);
        glBindVertexArray(baseVAO);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), reinterpret_cast<void*>(0 * sizeof(float)));
        // glEnableVertexAttribArray(1);
        // glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6* sizeof(float), reinterpret_cast<void*>(3 * sizeof(float)));
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glDrawArrays(GL_TRIANGLES, 0, pow((gridSize / 12), 2));
        glBindVertexArray(0);
		

	// glColor4f(0.0, 0.0, 1.0, 1.0);//Now draw the outer boundaries
	// glBegin(GL_LINES);//Start drawing lines. Let's start with the left boundary
	
	// //Bottom left corner of the map
	// glVertex3f(-2.0f, 0.0f, 2.0f);
	// //Top left corner of the map
	// glVertex3f(-2.0f, 0.0f, -_size - 8);
	
	// /*Next, let's do the back boundary. 
	// Continuing from where we left off, this is the top left corner of the map*/
	// glVertex3f(-2.0f, 0.0f, -_size - 8);
	// //Top right corner of the map
	// glVertex3f(_size + 8, 0.0f, -_size - 8);

	// //Right boundary: top right corner of the map
	// glVertex3f(_size + 8, 0.0f, -_size - 8);
	// //Bottom right corner of the map
	// glVertex3f(_size + 8, 0.0f, 2.0f);

	// //Front boundary: bottom right corner of the map
	// glVertex3f(_size + 8, 0.0f, 2.0f);
	// //Back to where we started: the bottom left corner of the map
	// glVertex3f(-2.0f, 0.0f, 2.0f);

	// glEnd();

	//Draw Buildings
	for(int i = 0; i < buildings.size(); i++){
		Building currBuilding = buildings[i];
		(*currBuilding)->draw();
	}
  }

};
