#include "Camera.h"

Camera::Camera()
    : Camera::Camera(glm::vec3(0.f))
{}

Camera::Camera(const glm::vec3 &position)
    : cameraPos(position),
      worldUp(glm::vec3(0.f, 1.f, 0.f)),
      yaw(-90.0f),
      pitch(0.0f),
      speed(2.5f),
      zoom(45.0f),
      cameraFront(glm::vec3(0.f, 0.f, -1.f))
{
    cameraStartPos = position;
    updateCameraVectors();
}

void Camera::updateCameraDirection(double dx, double dy)
{
    yaw += dx;
    pitch += dy;

    // limit vertical view
    if (pitch > 89.0f)
    {
        pitch = 89.f;
    }
    else if (pitch < -89.0f)
    {
        pitch = -89.f;
    }

    updateCameraVectors();
}

void Camera::updateCameraPos(CameraDirection dir, double dt)
{
    float velocity = (float) dt * speed;

    switch (dir)
    {
        case CameraDirection::FORWARD:
            cameraPos += cameraFront * velocity;
            break;
        case CameraDirection::BACKWARD:
            cameraPos -= cameraFront * velocity;
            break;
        case CameraDirection::RIGHT:
            cameraPos += cameraRight * velocity;
            break;
        case CameraDirection::LEFT:
            cameraPos -= cameraRight * velocity;
            break;
        case CameraDirection::UP:
            cameraPos += cameraUp * velocity;
            break;
        case CameraDirection::DOWN:
            cameraPos -= cameraUp * velocity;
            break;
    }
}

void Camera::updateCameraZoom(double dy)
{
    if (zoom >= 1.0f && zoom <= 45.f)
    {
        zoom -= dy;
    }
    else if (zoom < 1.0f)
    {
        zoom = 1.0f;
    }
    else
    {
        zoom = 45.0f;
    }
}

void Camera::reset()
{
    cameraPos = cameraStartPos;
    yaw       = -90.f;
    pitch     = 0.f;
    zoom      = 45.f;

    updateCameraVectors();
}

glm::mat4 Camera::getViewMatrix()
{
    return glm::lookAt(cameraPos,
                       cameraPos + cameraFront,
                       cameraUp);
}

void Camera::updateCameraVectors()
{
    // new direction
    glm::vec3 direction;
    direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    direction.y = sin(glm::radians(pitch));
    direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));

    cameraFront = glm::normalize(direction);
    cameraRight = glm::normalize(glm::cross(cameraFront, worldUp));
    cameraUp    = glm::normalize(glm::cross(cameraRight, cameraFront));
}
