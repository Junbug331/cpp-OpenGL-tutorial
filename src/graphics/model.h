#ifndef MODEL_H_
#define MODEL_H_

#include <vector>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "mesh.h"

class Model
{
public:
    glm::vec3 pos;
    glm::vec3 size;

    Model(glm::vec3 pos = glm::vec3(0.f), glm::vec3 size = glm::vec3(1.f));

    virtual void init();

    void loadModel(const std::string &path);

    virtual void render(Shader shader);

    void cleanup();

protected:
    std::vector<Mesh> meshes;
    std::string directory;

    std::vector<Texture> textures_loaded;

    void processNode(aiNode *node, const aiScene *scene);

    Mesh processMesh(aiMesh *mesh, const aiScene *scene);

    std::vector<Texture> loadTextures(aiMaterial *mat, aiTextureType type);
};

#endif
