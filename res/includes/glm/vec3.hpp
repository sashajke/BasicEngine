///////////////////////////////////////////////////////////////////////////////////
/// OpenGL Mathematics (glm.g-truc.net)
///
/// Copyright (c) 2005 - 2013 G-Truc Creation (www.g-truc.net)
/// Permission is hereby granted, free of charge, to any person obtaining a copy
/// of this software and associated documentation files (the "Software"), to deal
/// in the Software without restriction, including without limitation the rights
/// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
/// copies of the Software, and to permit persons to whom the Software is
/// furnished to do so, subject to the following conditions:
/// 
/// The above copyright notice and this permission notice shall be included in
/// all copies or substantial portions of the Software.
/// 
/// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
/// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
/// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
/// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
/// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
/// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
/// THE SOFTWARE.
///
/// @ref core
/// @file glm/vec3.hpp
/// @date 2013-12-24 / 2013-12-24
/// @author Christophe Riccio
///////////////////////////////////////////////////////////////////////////////////

#ifndef GLM_VEC3_INCLUDED
#define GLM_VEC3_INCLUDED

#include "detail/type_vec3.hpp"


inline glm::vec3 operator*(const glm::vec3 &u, const glm::vec3 &v) {
    return glm::vec3(u[0] * v[0], u[1] * v[1], u[2] * v[2]);
}

inline glm::vec3 operator*(double t, const glm::vec3 &v) {
    return glm::vec3(t*v[0], t*v[1], t*v[2]);
}

inline glm::vec3 operator*(const glm::vec3 &v, double t) {
    return t * v;
}

inline double dot(const glm::vec3 &u, const glm::vec3 &v) {
    return u[0] * v[0]
         + u[1] * v[1]
         + u[2] * v[2];
}

inline glm::vec3 cross(const glm::vec3 &u, const glm::vec3 &v) {
    return glm::vec3(u[1] * v[2] - u[2] * v[1],
                u[2] * v[0] - u[0] * v[2],
                u[0] * v[1] - u[1] * v[0]);
}

inline glm::vec3 operator/(glm::vec3 v, double t) {
    return (1/t) * v;
}

inline glm::vec3 unit_vector(glm::vec3 v) {
    return v / v.length();
}

inline bool isZeroVector(glm::vec3 v) {
    return v[0] == 0 && v[1] == 0 && v[2] == 0;
}


// inline glm::vec3 operator-(const glm::vec3 &v, double t)
// {
//     return glm::vec3(v[0]-t, v[1]-t, v[2]-t);
// }

#endif//GLM_VEC3_INCLUDED
