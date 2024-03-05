#ifndef TEXTURE_H
#define TEXTURE_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stb/stb_image.h>

#include <assimp/scene.h>

class Texture
{
public:
    Texture();
    Texture(const std::string &dir, const std::string &path, aiTextureType type);

    void generate();
    void load(bool flip = true);

    void activate();

    unsigned int id;
    aiTextureType type;
    std::string dir;
    std::string path;
};

#endif