#pragma once
#include "glm/vec3.hpp"


class Particle {

public:

    Particle(const glm::vec3& position, float mass, float radius);

    void applyForce(const glm::vec3& force);
    void integrate(float dt);
    void resetForce();

    glm::vec3 position{0.0f};
    glm::vec3 velocity{0.0f};
    glm::vec3 force{0.0f};

    float radius{0.0f};
    float mass{1.0f};
    float density{0.0f};
    float pressure{0.0f};


private:


};


