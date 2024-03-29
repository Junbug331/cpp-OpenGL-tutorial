#ifndef MOUSE_H
#define MOUSE_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>

class Mouse
{
public:
    static void cursorPosCallback(GLFWwindow *window, double _x, double _y);
    static void mouseButtonCallback(GLFWwindow *window, int button, int action, int mods);
    static void mouseWheelCallback(GLFWwindow *window, double dx, double dy);

    // accessors

    static double getMouseX();
    static double getMouseY();

    static double getDX();
    static double getDY();

    static double getScrollDX();
    static double getScrollDY();

    static double button(int button);
    static double buttonChanged(int button);
    static double buttonWentUp(int button);
    static double buttonWentDown(int button);

private:
    static double x;
    static double y;

    static double lastX;
    static double lastY;

    static double dx;
    static double dy;

    static double scrollDX;
    static double scrollDY;

    static bool firstMouse;
    static bool buttons[];
    static bool buttonsChanged[];
};

#endif