#ifndef LAMP_HPP
#define LAMP_HPP

#include "cube.hpp"
#include "../light.h"

class Lamp : public Cube
{
public:
    glm::vec3 lightColor;

    // light strength values
    PointLight pointLight;

    Lamp() {}

    Lamp(glm::vec3 lightColor,
         glm::vec3 ambient,
         glm::vec3 diffuse,
         glm::vec3 specular,
         float k0,
         float k1,
         float k2,
         glm::vec3 pos,
         glm::vec3 size)
        : lightColor(lightColor),
          pointLight({pos, k0, k1, k2, ambient, diffuse, specular}),
          Cube(pos, size) {}

    void render(Shader shader)
    {
        // set the light color
        shader.set3Float("lightColor", lightColor);
        Cube::render(shader);
    }
};

#endif