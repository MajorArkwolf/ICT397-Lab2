#include "terrain.hpp"
#include <glad/glad.h>
#include <glm/vec3.hpp>
#include <array>
#include "example1.hpp"
#include <fstream>

Red::Terrain::Terrain() {
    ourShader = new Shader("./shader/vert.vs", "./shader/frag.fs");
    loadScene();
}

Red::Terrain::~Terrain() {
    delete ourShader;
    vertex.clear();
    vertices.clear();
    indicies.clear();
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
}

void Red::Terrain::setupModel() {
    // world space positions of our cubes
    for (auto& e : vertex) {
        vertices.push_back(e.x);
        vertices.push_back(e.y);
        vertices.push_back(e.z);
        vertices.push_back(e.td1);
        vertices.push_back(e.td2);
    }

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), &vertices[0], GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indicies.size() * sizeof(unsigned int), &indicies[0], GL_STATIC_DRAW);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // texture coord attribute
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
}

void Red::Terrain::clearScene() {
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    vertex.clear();
    vertices.clear();
    indicies.clear();
    VAO = 0;
    VBO = 0;
    EBO = 0;
}

void Red::Terrain::generateTerrain(int x, int z) {
    Verticies newVertex{};
    for (int i = 0; i < x; ++i) {        
        for (int j = 0; j < z; ++j) {
            newVertex.x = i;
            newVertex.z = j;
            vertex.push_back(newVertex);
            newVertex = {};
        }
    }
}

void Red::Terrain::generateIndicies(unsigned int xsize, unsigned int ysize) {
    for (unsigned int x = 0; x < xsize - 1; ++x) {
        for (unsigned int y = 0; y < ysize - 1; ++y) {
            indicies.push_back(unsigned int(x * ysize + y)); // 0
            indicies.push_back(unsigned int((x * ysize + 1) + y)); // 1
            indicies.push_back(unsigned int(((x + 1) * ysize) + y)); // 3
            indicies.push_back(unsigned int((x * ysize + 1) + y)); // 1
            indicies.push_back(unsigned int((x + 1) * ysize) + (y + 1)); // 2
            indicies.push_back(unsigned int(((x + 1) * ysize) + y)); // 3
        }
    }
}
    

void Red::Terrain::setTextures(unsigned int tex1, unsigned int tex2) {
    this->textureID = tex1;
    this->textureID2 = tex2;

    ourShader->use();
    ourShader->setInt("texture1", 0);
    ourShader->setInt("texture2", 1);
}

void Red::Terrain::draw(glm::mat4 projection, glm::mat4 view) {
    if (wireframe) {
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    }
    else {
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    }
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, textureID);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, textureID2);
    ourShader->setMat4("projection", projection);
    ourShader->setMat4("view", view);
    
    // calculate the model matrix for each object and pass it to shader before drawing
    glm::mat4 model = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
    model = glm::translate(model, glm::vec3{});
    float angle = 20.0f * 0;
    //model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
    model = glm::translate(model, glm::vec3(0.0f, -1.75f, 0.0f));
    ourShader->setMat4("model", model);
    ourShader->use();
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, indicies.size(), GL_UNSIGNED_INT, nullptr);
    glBindVertexArray(0);
    glActiveTexture(GL_TEXTURE0);
}
void Red::Terrain::update(double t, double dt) {

}

void Red::Terrain::generateTextureCords(unsigned int xsize, unsigned int ysize) {
    for (unsigned int x = 0; x < xsize - 1; ++x) {
        for (unsigned int y = 0; y < ysize - 1; y += 2) {
            if (x % 2 == 0) {
                vertex.at((x * ysize) + y).td1 = 0.0f;
                vertex.at((x * ysize) + y).td2 = 0.0f;
                vertex.at((x * ysize) + y + 1u).td1 = 1.0f;
                vertex.at((x * ysize) + y + 1u).td2 = 0.0f;
            }
            else {
                vertex.at((x * ysize) + y).td1 = 0.0f;
                vertex.at((x * ysize) + y).td2 = 1.0f;
                vertex.at((x * ysize) + y + 1u).td1 = 1.0f;
                vertex.at((x * ysize) + y + 1u).td2 = 1.0f;
            }
        }
    }
}

void Red::Terrain::loadScene() {
    if (example == 0) {
        unsigned int xsize = 129;
        unsigned int zsize = 129;
        clearScene();
        generateTerrain(xsize, zsize);
        generateIndicies(xsize, zsize);
        generateTextureCords(xsize, zsize);
        Example1::loadHeightMap(vertex);
        Example1::upScale(vertex, 3);
        setupModel();
        ++example;
    }
    else if (example == 1) {

        int maxHeight = 100;
        int minHeight = 0;
        int hSize = 512;
        unsigned int xsize = hSize;
        unsigned int zsize = hSize;
        int iterations = 256;
        float weight = 0.1;
        int postSmoothingIterations = 20;
        clearScene();
        generateTerrain(xsize, zsize);
        generateIndicies(xsize, zsize);
        generateTextureCords(xsize, zsize);
        //Example2::genFaultFormation(vertex, 256, 512, 0, 255, 0.1, 20, 0);
        Example2::genFaultFormation(vertex, iterations, hSize, minHeight,
            maxHeight, weight,
            postSmoothingIterations, false);
        setupModel();
        example = 0;
    }
}

