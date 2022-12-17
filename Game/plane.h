#pragma once
#ifndef GAME_PLANE_H
#define GAME_PLANE_H


#include "hittable.h"
#include "glm/detail/type_vec4.hpp"
#include "../res/includes/glm/glm.hpp"



class plane : public hittable {
    public:
        plane() {}
        plane(float transperancy,float reflectiveness, float shininess, glm::vec3 color, glm::vec4 equation) : equation(equation){
            this->color = color;
            this-> shininess = shininess;
            this-> reflectiveness = reflectiveness;
            this->normal = glm::normalize(glm::vec3(equation[0], equation[1], equation[2]));
            this->point = getPointOnPlane();
            this->transparency = transperancy;
        };

        virtual bool hit(const ray& r, double t_min, double t_max, hit_record& rec) const override;
        virtual glm::vec3 getPointOnPlane()const;
        glm::vec4 equation;
        glm::vec3 point;
        glm::vec3 normal;
};


#endif //GAME_PLANE_H
