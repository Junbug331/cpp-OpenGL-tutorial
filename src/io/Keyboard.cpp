#include "Keyboard.h"

bool Keyboard::keys[GLFW_KEY_LAST]        = {0};
bool Keyboard::keysChanged[GLFW_KEY_LAST] = {0};

// Key state callback
void Keyboard::keyCallback(GLFWwindow *window, int key, int scancode, int action, int modes)
{
    // action: GLFW_RELEASE(0), GLFW_PRESS(1), GLFW_REPEAT(2)
    if (action != GLFW_RELEASE)
    {
        if (!keys[key])
        {
            keys[key] = true;
        }
    }
    else
    {
        keys[key] = false;
    }

    keysChanged[key] = (action != GLFW_REPEAT);
}

bool Keyboard::key(int key)
{
    return keys[key];
}

bool Keyboard::keyChanged(int key)
{
    bool ret = keysChanged[key];

    // Since change was happend in at a certain single frame
    keysChanged[key] = false;

    return ret;
}


bool Keyboard::keyWentUp(int key)
{
    // keysChanged[key] will true because GLFW_RELEASE != GLFW_REPEAT
    return !keys[key] && keyChanged(key);
}

bool Keyboard::keyWentDown(int key)
{
    // keysChanged[key] will true because GLFW_PRESS != GLFW_REPEAT
    return keys[key] && keyChanged(key);
}