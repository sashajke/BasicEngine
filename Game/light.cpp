#include <iostream>
#include "light.h"

static double ka = 0.7;
static double kd = 0.7;
static double ks = 0.7;

glm::vec3 light::getColor(ray camera, hit_record hit) { 
    auto rayToLight = getRayFromPointToLight(hit.p);
    // no direction
    if (isZeroVector(rayToLight.direction())) {
        return glm::vec3(0,0,0);
    }
    auto normalizedDirectionToLight = glm::normalize(rayToLight.direction());
    auto normalizedDirectionToCamera = glm::normalize(camera.direction() * -1.0f);
    auto normalizedNormalToHitPoint = glm::normalize(hit.normal);
    auto normalizedReflectionVector = glm::normalize(2.0f * glm::dot(normalizedDirectionToLight,normalizedNormalToHitPoint) * normalizedNormalToHitPoint - normalizedDirectionToLight);
    float maxValue = std::fmax(0.0, (float)std::pow((float)glm::dot(normalizedReflectionVector, normalizedDirectionToCamera), hit.shininess));
    float dotValue = glm::dot(normalizedDirectionToLight, normalizedNormalToHitPoint);
    return   (hit.kd * dotValue + ks *  maxValue) * this->color;
}