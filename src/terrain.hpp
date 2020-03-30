#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>
#include "shader.hpp"

#include "vertix.hpp"


namespace Red {
	class Terrain {
	public:
		Terrain();
		~Terrain();
		void loadScene();
		void clearScene();
		void generateTerrain(int x, int z);
		void generateIndicies(unsigned int xsize, unsigned int ysize);
		void generateTextureCords(unsigned int xsize, unsigned int ysize);
		void setupModel();
		void draw(glm::mat4 projection, glm::mat4 view);
		void update(double t, double dt);
		void setTextures(unsigned int tex1, unsigned int tex2);
		bool wireframe = false;
	private:
		std::vector<Verticies> vertex;
		std::vector<float> vertices;
		std::vector<unsigned int> indicies;
		Shader* ourShader = nullptr;
		unsigned int VBO = 0, VAO = 0, EBO = 0;
		unsigned int textureID = 0, textureID2 = 0;
		unsigned int example = 0;
	};
}