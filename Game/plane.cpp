

#include "plane.h"


#define  EPSILON 1e-6


bool plane::hit(const ray& r, double t_min, double t_max, hit_record& rec) const {
    if(r.direction()==glm::vec3(0,0,0))
    {
        return false;
    }
    // Compute the dot product of the ray direction and the plane normal
    float d = dot(r.direction(), this->normal);

    // Check if the ray is parallel to the plane (d == 0) or if the ray is facing away from the plane (d < 0)
    if (abs(d) < EPSILON)
    {
        return false;
    }

    // Compute the distance from the ray origin to the plane
    float t = dot(this->point - r.origin(), this->normal) / d;
    rec.p = r.origin() + t * r.direction();
    rec.set_face_normal(r, normal);
    // Check if the intersection is behind the ray origin
    if (t > 0 && t >= t_min && t<=t_max)
    {
        glm::vec3 projected_point = rec.p - (glm::dot(rec.normal, rec.p) * rec.normal);
        if (((int)(std::floor(projected_point.x * 1.5) + std::floor(projected_point.y * 1.5)) % 2) == 0)
            rec.kd = 0.5;
        else
            rec.kd = 0.7;

        // Update the hit information
        rec.t = t;
        rec.p = r.origin() + t * r.direction();
        rec.color = this->color;
        rec.reflectiveness = this->reflectiveness;
        rec.shininess = this->shininess;
        rec.set_face_normal(r, normal);
        rec.transparency = this->transparency;
        return true;
    }
    return false;

}

glm::vec3 plane::getPointOnPlane() const {
    glm::vec3 result;
    float a = equation[0];
    float b = equation[1];
    float c = equation[2];
    float d = equation[3];

    if (a != 0) {
        result.y =0;
        result.z = 0;
        if (d!= 0) {
            result.x = -d/a;
        }
        else {
            result.x = 0;
        }
    } else if (b != 0) {
        result.x =0;
        result.z = 0;
        if (d!= 0) {
            result.y = -d/b;
        }
        else {
            result.y = 0;
        }
    } else if (c != 0) {
        result.x =0;
        result.y = 0;
        if (d!= 0) {
            result.z = -d/c;
        }
        else {
            result.z = 0;
        }
    }
    return result;
}