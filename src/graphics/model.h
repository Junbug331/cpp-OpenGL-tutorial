#ifndef MODEL_H_
#define MODEL_H_

#include <vector>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "mesh.h"

class Model
{
public:
    std::vector<Mesh> meshes;

    Model();

    virtual void init();

    virtual void render(Shader shader);

    void cleanup();
};

#endif
