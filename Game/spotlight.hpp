#include "../res/includes/glm/glm.hpp"

#define ZERO_VECTOR glm::vec3(0.0,0.0,0.0)

class spotlight : public light {
  public:
    glm::vec3 position;  // position of the spotlight
    float cutoffAngle;
    spotlight(glm::vec3 origin, glm::vec3 direction, float angle, glm::vec3 color) {
        this->direction = direction;
        this->color = color;
        this->cutoffAngle = angle;
        this->position = origin;
    };
    virtual ray getRayFromPointToLight(glm::vec3 point) override;
};

ray spotlight::getRayFromPointToLight(glm::vec3 point) {
    glm::vec3 directionFromLightToPoint = point - this->position;
    directionFromLightToPoint = glm::normalize(directionFromLightToPoint);

    float angle = acos(glm::dot(directionFromLightToPoint, glm::normalize(this->direction)));
    if (angle > this->cutoffAngle) {
        return ray(point, ZERO_VECTOR);
    }
    return ray(point, directionFromLightToPoint * -1.0);
}