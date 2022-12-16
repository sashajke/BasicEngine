#include <iostream>
#include "light.h"

static double ka = 0.7;
static double kd = 0.7;
static double ks = 0.7;

glm::vec3 light::getColor(ray camera, hit_record hit) { 
//    auto rayToLight = getRayFromPointToLight(hit.p);
//    // no direction
//    if (isZeroVector(rayToLight.direction())) {
//        return glm::vec3(0,0,0);
//    }
//    auto normalizedDirectionToLight = glm::normalize(rayToLight.direction());
//    auto normalizedDirectionToCamera = glm::normalize(camera.direction() * -1.0f);
//    auto normalizedNormalToHitPoint = glm::normalize(hit.normal);
//    auto normalizedReflectionVector = glm::normalize(2.0f * glm::dot(normalizedDirectionToLight,normalizedNormalToHitPoint) * normalizedNormalToHitPoint - normalizedDirectionToLight);
//    float maxValue = std::fmax(0.0, (float)std::pow((float)glm::dot(normalizedReflectionVector, normalizedDirectionToCamera), 3));
//    float dotValue = glm::dot(normalizedDirectionToLight, normalizedNormalToHitPoint);
//    return   (kd * dotValue + ks *  maxValue) * this->color;
    glm::vec3 temp = getRayFromPointToLight(hit.p).direction();
    if (glm::length(temp) == 0)
        return glm::vec3(.0f,.0f,.0f);
    glm::vec3 L = glm::normalize(temp);
    glm::vec3 V = glm::normalize(camera.dir * (-1.0f));
    glm::vec3 N = glm::normalize(hit.normal);
    glm::vec3 R = glm::normalize(2.0f * glm::dot(N, L) * N - L); //https://www.fabrizioduroni.it/2017/08/25/how-to-calculate-reflection-vector/
    return (0.7 * glm::dot(L, N) + 0.7 * std::fmax(0.0f, ((float)std::pow((float)glm::dot(R, V),10)))) * this->color;
}