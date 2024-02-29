#include "Screen.h"
#include "Keyboard.h"
#include "Mouse.h"

unsigned int Screen::SCR_WIDTH  = 800;
unsigned int Screen::SCR_HEIGHT = 600;

void Screen::framebufferSizeCallback(GLFWwindow *window, int width, int height)
{
    glViewport(0, 0, width, height);

    SCR_WIDTH  = width;
    SCR_HEIGHT = height;
}

Screen::Screen()
    : window(nullptr)
{
}

bool Screen::init(unsigned int width, unsigned int height)
{
    SCR_WIDTH  = width;
    SCR_HEIGHT = height;

    return init();
}

bool Screen::init()
{
    window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "OpenGL Tutorial", nullptr, nullptr);

    if (!window)
    {
        return false;
    }
    glfwMakeContextCurrent(window);

    return true;
}

void Screen::setParameters()
{
    glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);

    glfwSetFramebufferSizeCallback(window, Screen::framebufferSizeCallback);

    // Keyboard and mouse input callback
    glfwSetKeyCallback(window, Keyboard::keyCallback);
    glfwSetCursorPosCallback(window, Mouse::cursorPosCallback);
    glfwSetMouseButtonCallback(window, Mouse::mouseButtonCallback);
    glfwSetScrollCallback(window, Mouse::mouseWheelCallback);
}

// main loop
void Screen::update()
{
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Screen::newFrame()
{
    glfwSwapBuffers(window);
    glfwPollEvents();
}

// window closing accessor and modifier
bool Screen::shouldClose()
{
    return glfwWindowShouldClose(window);
}

void Screen::setShouldClose(bool shouldClose)
{
    glfwSetWindowShouldClose(window, shouldClose);
}