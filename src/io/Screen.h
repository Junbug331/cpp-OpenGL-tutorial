#ifndef SCREEN_H
#define SCREEN_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <string>

// Abstracting window function
class Screen
{
public:
    static unsigned int SCR_WIDTH;
    static unsigned int SCR_HEIGHT;

    static void framebufferSizeCallback(GLFWwindow *window, int width, int height);

    Screen();

    bool init();
    bool init(unsigned int width, unsigned int height);

    void setParameters();

    // main loop
    void update();
    void newFrame();

    // window closing accessor and modifier
    bool shouldClose();
    void setShouldClose(bool shouldClose);

private:
    GLFWwindow *window;
};

#endif