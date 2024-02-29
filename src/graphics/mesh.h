#ifndef MESH_H
#define MESH_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <vector>
#include <glm/glm.hpp>

#include "shader.h"
#include "texture.h"

struct Vertex {
    glm::vec3 pos;
    glm::vec3 normal;
    glm::vec2 texCoord;

    static std::vector<struct Vertex> genList(float *verticies, int noVerticies);
};
using Vertex = struct Vertex;

class Mesh
{
public:
    std::vector<Vertex> verticies;
    std::vector<unsigned int> indicies;
    unsigned int VAO;

    std::vector<Texture> textures;

    Mesh();
    Mesh(const std::vector<Vertex> &verticies,
         const std::vector<unsigned int> &indicies,
         const std::vector<Texture> &textures);

    void render(Shader shader);
    void cleanup();

private:
    unsigned int VBO, EBO;
    void setup();
};

#endif