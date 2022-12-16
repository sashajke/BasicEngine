#include "../res/includes/glm/glm.hpp"
#include "ray.cpp"
#include "hittable.h"

class light {
    public:
        light(){}
        glm::vec3 direction;
        glm::vec3 color; // base illumination (rgb)

            
        virtual glm::vec3 getColor(ray camera, hit_record hit);
        virtual ray getRayFromPointToLight(glm::vec3 point) =0;
};
