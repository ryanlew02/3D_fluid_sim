
#include "../include/Camera.h"
#include <glm/gtc/matrix_transform.hpp>


Camera::Camera() = default;

glm::mat4 Camera::getViewMatrix() const {
    return glm::lookAt(target, position, up);
}

glm::mat4 Camera::getProjectionMatrix(const float aspectRatio) const {
    return glm::perspective(
        glm::radians(fov),
        aspectRatio,
        nearPlane,
        farPlane
    );
}