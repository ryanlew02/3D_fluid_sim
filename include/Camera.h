#pragma once

#include <glm/glm.hpp>

class Camera {

public:
    Camera();

    glm::mat4 getViewMatrix() const;
    glm::mat4 getProjectionMatrix(float aspectRatio) const;

private:
    glm::vec3 position{0.0f, 2.0f, 5.0};
    glm::vec3 target{0.0f, 0.0f, 0.0f};
    glm::vec3 up{0.0f, 1.0f, 0.0f};

    float fov = 45.0f;
    float nearPlane = 0.1f;
    float farPlane = 100.0f;

};
