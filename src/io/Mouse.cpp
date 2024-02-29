#include "Mouse.h"

double Mouse::x        = 0.0;
double Mouse::y        = 0.0;
double Mouse::lastX    = 0.0;
double Mouse::lastY    = 0.0;
double Mouse::dx       = 0.0;
double Mouse::dy       = 0.0;
double Mouse::scrollDX = 0.0;
double Mouse::scrollDY = 0.0;

bool Mouse::firstMouse                             = true;
bool Mouse::buttons[GLFW_MOUSE_BUTTON_LAST]        = {0};
bool Mouse::buttonsChanged[GLFW_MOUSE_BUTTON_LAST] = {0};

void Mouse::cursorPosCallback(GLFWwindow *window, double _x, double _y)
{
    x = _x;
    y = _y;

    if (firstMouse)
    {
        lastX      = x;
        lastY      = y;
        firstMouse = false;
    }

    dx    = x - lastX;
    dy    = lastY - y;// inverted top left is (0, 0)
    lastX = x;
    lastY = y;
}

void Mouse::mouseButtonCallback(GLFWwindow *window, int button, int action, int mods)
{
    if (action != GLFW_RELEASE)
    {
        if (!buttons[button])
        {
            buttons[button] = true;
        }
    }
    else
    {
        buttons[button] = false;
    }
    buttonsChanged[button] = (action != GLFW_REPEAT);
}

void Mouse::mouseWheelCallback(GLFWwindow *window, double dx, double dy)
{
    scrollDX = dx;
    scrollDY = dy;
}

double Mouse::getMouseX()
{
    return x;
}

double Mouse::getMouseY()
{
    return y;
}

double Mouse::getDX()
{
    double _dx = dx;
    dx         = 0;
    return _dx;
}

double Mouse::getDY()
{
    double _dy = dy;
    dy         = 0;
    return _dy;
}

double Mouse::getScrollDX()
{
    double dx = scrollDX;
    scrollDX  = 0;
    return dx;
}

double Mouse::getScrollDY()
{
    double dy = scrollDY;
    scrollDY  = 0;
    return dy;
}

double Mouse::button(int button)
{
    return buttons[button];
}

double Mouse::buttonChanged(int button)
{
    bool ret               = buttonsChanged[button];
    buttonsChanged[button] = false;
    return ret;
}

double Mouse::buttonWentUp(int button)
{
    return !buttons[button] && buttonChanged(button);
}

double Mouse::buttonWentDown(int button)
{
    return buttons[button] && buttonChanged(button);
}