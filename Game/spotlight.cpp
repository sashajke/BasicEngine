// #include "spotlight.h"
// #define ZERO_VECTOR glm::vec3(0.0,0.0,0.0)

// spotlight::spotlight(glm::vec3 origin, glm::vec3 direction, float angle, glm::vec3 color) {
//     this->direction = direction;
//     this->color = color;
//     this->cutoffAngle = angle;
//     this->position = origin;
// }

// ray spotlight::getRayFromPointToLight(glm::vec3 point) { 
//     glm::vec3 directionFromLightToPoint = point - this->position;
//     directionFromLightToPoint = glm::normalize(directionFromLightToPoint);

//     float angle = acos(dot(directionFromLightToPoint, glm::normalize(this->direction)));
//     if (angle > this->cutoffAngle) {
//         return ray(point, ZERO_VECTOR);
//     }
//     return ray(point, directionFromLightToPoint * -1.0);
// }



//    return ray(point, glm::normalize(this->direction) * -1.0f)glm::normalize(this->direction) * -1.0f);