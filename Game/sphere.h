#ifndef SPHERE_H
#define SPHERE_H

#include "hittable.h"
#include "../res/includes/glm/vec3.hpp"
#include <cmath>


double length_squared(glm::vec3 e)
{
    return e[0]*e[0] + e[1]*e[1] + e[2]*e[2];
}

class sphere : public hittable {
    public:
        sphere() {}
        sphere(glm::vec3 cen, double r, float transperancy, float reflectiveness, float shininess, glm::vec3 color) : center(cen), radius(r) {
            this->color = color;
            this-> shininess = shininess;
            this-> reflectiveness = reflectiveness;
            this->transparency = transperancy;
        };

        virtual bool hit(const ray& r, double t_min, double t_max, hit_record& rec) const override;

    public:
        glm::vec3 center;
        double radius;
};

bool sphere::hit(const ray& r, double t_min, double t_max, hit_record& rec) const {
    if (r.direction() == glm::vec3(0,0,0)) {
        return false;
    }
    glm::vec3 oc = r.origin() - center;
    auto a = length_squared(r.direction());
    auto half_b = dot(oc, r.direction());
    auto c = length_squared(oc) - radius*radius;

    auto discriminant = half_b*half_b - a*c;
    if (discriminant < 0) return false;
    auto sqrtd = sqrt(discriminant);

    // Find the nearest root that lies in the acceptable range.
    if (a == 0) return false;
    auto root = (-half_b - sqrtd) / a;
    if (root < t_min || t_max < root) {
        root = (-half_b + sqrtd) / a;
        if (root < t_min || t_max < root)
            return false;
    }

    rec.t = root;
    rec.p = r.at(rec.t);
    rec.kd = 0.7;
    rec.color = this->color;
    rec.reflectiveness = this->reflectiveness;
    rec.shininess = this->shininess;
    rec.transparency = this->transparency;
    glm::vec3 outward_normal = glm::normalize((rec.p - center) / radius);
    rec.set_face_normal(r, outward_normal);

    return true;
}
#endif