#include <iomanip>
#include <iostream>
#include <spdlog/spdlog.h>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <fstream>
#include <strstream>
#include <streambuf>
#include <string>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/component_wise.hpp>
#include <glm/gtx/string_cast.hpp>

#include <stb_image.h>

#include <filesystem>
namespace fs                   = std::filesystem;
fs::path path_assets           = fs::path(ASSETS_DIR);
fs::path path_vertex_shader    = path_assets / fs::path("vertex_core.vs");
fs::path path_fragment_shader  = path_assets / fs::path("fragment_core.fs");
fs::path path_fragment2_shader = path_assets / fs::path("fragment_core2.fs");

#include "graphics/shader.h"
#include "graphics/texture.h"
#include "graphics/material.h"
#include "graphics/models/cube.hpp"
#include "graphics/models/lamp.hpp"
#include "graphics/light.h"
#include "graphics/model.h"

#include "io/Keyboard.h"
#include "io/Mouse.h"
#include "io/Joystick.h"
#include "io/Camera.h"
#include "io/Screen.h"

void processInput(double dt);
void printGlmMat4(const glm::mat4 &glm_mat4);

Joystick mainJ(0);

float mixVal = 0.5f;

unsigned int SCR_WIDTH  = 1080;
unsigned int SCR_HEIGHT = 720;

glm::mat4 mouseTransform = glm::mat4(1.0f);

Camera cameras[2] = {
        Camera(glm::vec3(0.f, 0.f, 3.f)),
        Camera(glm::vec3(10.f, 10.f, 10.f))};

int activeCam = 0;

double deltaTime = 0.f;
double lastFrame = 0.f;

Screen screen;

using namespace std;
int main()
{
    // Set the precision to 2 decimal places
    std::cout << std::fixed << std::setprecision(3);
    int success;
    char infoLog[512];

    glfwInit();

    // openGL version 3.3
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COPMPAT, GL_TRUE);
#endif

    if (!screen.init(1080, 720))
    {
        std::cout << "Could not create window." << std::endl;
        glfwTerminate();
        return -1;
    }

    if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        glfwTerminate();
        return -1;
    }

    screen.setParameters();

    // Create depth buffer
    glEnable(GL_DEPTH_TEST);
    fs::path path_assets_dir(ASSETS_DIR);
    fs::path path_vs     = path_assets_dir / "object.vs";
    fs::path path_fs     = path_assets_dir / "object.fs";
    fs::path path_LampVs = path_assets_dir / "object.vs";
    fs::path path_LampFs = path_assets_dir / "lamp.fs";
    fs::path path_model  = path_assets_dir / fs::path("models/lotr_troll/scene.gltf");

    // Shader for object
    Shader shader(path_vs.c_str(), path_fs.c_str());
    Shader lampShader(path_vs.c_str(), path_LampFs.c_str());

    Model m(glm::vec3(0.f, 0.f, -5.0f), glm::vec3(0.05));
    m.loadModel(path_model.string());

    glm::vec3 pointLightPositions[] = {
            glm::vec3(0.7f, 0.2f, 2.0f),
            glm::vec3(2.3f, -3.3f, -4.0f),
            glm::vec3(-4.0f, 2.0f, -12.0f),
            glm::vec3(0.0f, 0.0f, -3.0f)};
    Lamp lamps[4];
    for (unsigned int i = 0; i < 4; i++)
    {
        lamps[i] = Lamp(glm::vec3(1.0f),
                        glm::vec3(0.05f), glm::vec3(0.8f), glm::vec3(1.0f),
                        1.0f, 0.07f, 0.032f,
                        pointLightPositions[i], glm::vec3(0.25f));
        lamps[i].init();
    }

    DirLight dirLight = {glm::vec3(-0.2f, -1.0f, -0.3f), glm::vec3(0.1f), glm::vec3(0.4f), glm::vec3(0.5f)};

    SpotLight s = {
            cameras[activeCam].cameraPos,                   // pos
            cameras[activeCam].cameraFront,                 // direction
            glm::cos(glm::radians(20.f * 0.6f)),            // cutOff
            glm::cos(glm::radians(20.f)),                   // outer cutOff
            1.0f, 0.07f, 0.032f,                            // attenuation
            glm::vec3(0.f), glm::vec3(1.f), glm::vec3(1.f)};// ambient, diffuse, specular


    mainJ.update();
    if (mainJ.isPresent())
    {
        std::cout << mainJ.getName() << " is present." << std::endl;
    }

    float fov  = 90.f;
    float near = 1.f / tan(glm::radians(fov * 0.5));

    while (!screen.shouldClose())
    {
        double currentTime = glfwGetTime();
        deltaTime          = currentTime - lastFrame;
        lastFrame          = currentTime;

        // process input
        processInput(deltaTime);

        // render
        screen.update();

        shader.activate();
        shader.set3Float("viewPos", cameras[activeCam].cameraPos);

        dirLight.direction = glm::vec3(
                glm::rotate(glm::mat4(1.f), glm::radians(0.5f), glm::vec3(1.0f, 0.0f, 0.0f)) *
                glm::vec4(dirLight.direction, 1.0f));
        dirLight.render(shader);

        for (unsigned int i = 0; i < 4; ++i)
        {
            lamps[i].pointLight.render(shader, i);
        }
        shader.setInt("noPointLights", 4);

        s.position  = cameras[activeCam].cameraPos;
        s.direction = cameras[activeCam].cameraFront;
        s.render(shader, 0);
        shader.setInt("noSpotLights", 1);


        // create transformation
        glm::mat4 view       = glm::mat4(1.0f);// world -> camera
        glm::mat4 projection = glm::mat4(1.0f);
        view                 = cameras[activeCam].getViewMatrix();
        projection           = glm::perspective(glm::radians(cameras[activeCam].getZoom()), (float) SCR_WIDTH / (float) SCR_HEIGHT, near, 100.0f);

        shader.setMat4("view", view);
        shader.setMat4("projection", projection);

        m.render(shader);

        lampShader.activate();
        lampShader.setMat4("view", view);
        lampShader.setMat4("projection", projection);
        for (int i = 0; i < 4; ++i)
        {
            lamps[i].render(lampShader);
        }

        // send new frame to window
        screen.newFrame();
    }

    m.cleanup();

    for (int i = 0; i < 4; ++i)
    {
        lamps[i].cleanup();
    }

    glfwTerminate();
    return 0;
}


void processInput(double dt)
{
    if (Keyboard::key(GLFW_KEY_ESCAPE))
    {
        screen.setShouldClose(true);
    }

    // change mix value
    if (Keyboard::key(GLFW_KEY_UP))
    {
        mixVal += .05f;
        if (mixVal > 1)
        {
            mixVal = 1.0f;
        }
    }
    if (Keyboard::key(GLFW_KEY_DOWN))
    {
        mixVal -= .05f;
        if (mixVal < 0)
        {
            mixVal = 0.0f;
        }
    }

    if (Keyboard::keyWentDown(GLFW_KEY_TAB))
    {
        activeCam += (activeCam == 0) ? 1 : -1;
    }


    if (Keyboard::key(GLFW_KEY_W))
    {
        cameras[activeCam].updateCameraPos(CameraDirection::FORWARD, dt);
    }
    if (Keyboard::key(GLFW_KEY_S))
    {
        cameras[activeCam].updateCameraPos(CameraDirection::BACKWARD, dt);
    }
    if (Keyboard::key(GLFW_KEY_A))
    {
        cameras[activeCam].updateCameraPos(CameraDirection::LEFT, dt);
    }
    if (Keyboard::key(GLFW_KEY_D))
    {
        cameras[activeCam].updateCameraPos(CameraDirection::RIGHT, dt);
    }
    if (Keyboard::key(GLFW_KEY_Q))
    {
        cameras[activeCam].updateCameraPos(CameraDirection::UP, dt);
    }
    if (Keyboard::key(GLFW_KEY_E))
    {
        cameras[activeCam].updateCameraPos(CameraDirection::DOWN, dt);
    }
    if (Keyboard::keyWentDown(GLFW_KEY_SPACE))
    {
        cameras[activeCam].reset();
    }

    double dx = Mouse::getDX(), dy = Mouse::getDY();
    if (dx != 0 || dy != 0)
    {
        cameras[activeCam].updateCameraDirection(dx, dy);
    }

    double scrollDy = Mouse::getScrollDY();
    {
        if (scrollDy != 0)
        {
            cameras[activeCam].updateCameraZoom(scrollDy);
        }
    }
}

void printGlmMat4(const glm::mat4 &glm_mat4)
{
    glm::mat4 row_major = glm::transpose(glm_mat4);
    float *data         = glm::value_ptr(row_major);

    for (int r = 0; r < 4; ++r)
    {
        for (int c = 0; c < 4; ++c)
        {
            std::cout << data[r * 4 + c] << '\t';
        }
        std::cout << '\n';
    }
}