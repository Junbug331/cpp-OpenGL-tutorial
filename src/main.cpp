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

    // Shader for object
    Shader shader(path_vs.c_str(), path_fs.c_str());
    Shader lampShader(path_vs.c_str(), path_LampFs.c_str());

    Cube cube(Material::copper, glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.75f));
    cube.init();

    Lamp lamp(glm::vec3(1.0f), glm::vec3(1.0f), glm::vec3(1.0f), glm::vec3(1.0f), glm::vec3(-3.0f, -0.0f, -1.0f), glm::vec3(0.25f));
    lamp.init();

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
        shader.set3Float("light.position", lamp.pos);
        shader.set3Float("viewPos", cameras[activeCam].cameraPos);

        shader.set3Float("light.ambient", lamp.ambient);
        shader.set3Float("light.diffuse", lamp.diffuse);
        shader.set3Float("light.specular", lamp.specular);

        // create transformation
        glm::mat4 model      = glm::mat4(1.0f);// local -> world
        glm::mat4 view       = glm::mat4(1.0f);// world -> camera
        glm::mat4 projection = glm::mat4(1.0f);

        view       = cameras[activeCam].getViewMatrix();
        projection = glm::perspective(glm::radians(cameras[activeCam].getZoom()), (float) SCR_WIDTH / (float) SCR_HEIGHT, near, 100.0f);

        shader.setMat4("view", view);
        shader.setMat4("projection", projection);

        cube.render(shader);

        lampShader.activate();
        lampShader.setMat4("view", view);
        lampShader.setMat4("projection", projection);
        lamp.render(lampShader);


        glBindVertexArray(0);

        // send new frame to window
        screen.newFrame();
    }

    cube.cleanup();
    lamp.cleanup();

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