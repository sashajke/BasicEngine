

#include "directionalLight.h"
#include "constantsClass.h"

directionalLight::directionalLight(glm::vec3 direction, glm::vec3 color) {
    this->direction = direction;
    this->color = color;
}

ray directionalLight::getRayFromPointToLight(glm::vec3 point)const {
    return  ray(point,glm::normalize(this->direction) * -1.0f);
}
