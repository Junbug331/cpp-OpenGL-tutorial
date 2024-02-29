#include "model.h"

Model::Model()
{}

void Model::init()
{}

void Model::render(Shader shader)
{
    for (auto &mesh: meshes)
    {
        mesh.render(shader);
    }
}

void Model::cleanup()
{
    for (auto &mesh: meshes)
    {
        mesh.cleanup();
    }
}
