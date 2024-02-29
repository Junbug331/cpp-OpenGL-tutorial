#include "mesh.h"

std::vector<Vertex> Vertex::genList(float *verticies, int noVerticies)
{
    std::vector<Vertex> ret(noVerticies);

    int stride = sizeof(Vertex) / sizeof(float);// 5

    for (int i = 0; i < noVerticies; ++i)
    {
        ret[i].pos = glm::vec3(
                verticies[i * stride + 0],
                verticies[i * stride + 1],
                verticies[i * stride + 2]);
        ret[i].texCoord = glm::vec2(
                verticies[i * stride + 3],
                verticies[i * stride + 4]);
    }

    return ret;
}


Mesh::Mesh()
{
}

Mesh::Mesh(const std::vector<Vertex> &verticies,
           const std::vector<unsigned int> &indicies,
           const std::vector<Texture> &textures)
    : verticies(verticies), indicies(indicies), textures(textures)
{
    setup();
}

void Mesh::setup()
{
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, verticies.size() * sizeof(Vertex), &verticies[0], GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indicies.size() * sizeof(unsigned int), &indicies[0], GL_STATIC_DRAW);


    // set vertex attribute pointers
    // vertex.position
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *) 0);

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *) offsetof(Vertex, texCoord));

    glBindVertexArray(0);
}

void Mesh::render(Shader shader)
{
    // textures
    for (unsigned int i = 0; i < textures.size(); ++i)
    {
        shader.setInt(textures[i].name, textures[i].id);
        glActiveTexture(GL_TEXTURE0 + i);
        textures[i].bind();
    }

    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, indicies.size(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);

    glActiveTexture(GL_TEXTURE0);
}

void Mesh::cleanup()
{
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
}