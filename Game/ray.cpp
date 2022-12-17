#ifndef RAY_H
#define RAY_H

#include "../res/includes/glm/vec3.hpp"

class ray {
    public:
        ray() {}
        ray(const glm::vec3& origin, const glm::vec3& direction)
            : orig(origin), dir(direction)
        {}

        glm::vec3 origin() const  { return orig; } //A
        glm::vec3 direction() const { return dir; } // B

        glm::vec3 at(double t) const {
            return orig + t*dir;
        }
        glm::vec3 orig;
        glm::vec3 dir;
};

#endif