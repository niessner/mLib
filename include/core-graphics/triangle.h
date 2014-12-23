#pragma once

#ifndef _TRIANGLE_H_
#define _TRIANGLE_H_

namespace ml {

template<class T>
struct Triangle
{
    Triangle() {}
    Triangle(const vec3<T> &v0, const vec3<T> &v1, const vec3<T> &v2)
    {
        vertices[0] = v0;
        vertices[1] = v1;
        vertices[2] = v2;
    }
    vec3<T> getNormal() const
    {
        return ml::math::triangleNormal(vertices[0], vertices[1], vertices[2]);
    }

    vec3<T> vertices[3];

};

typedef Triangle<float> Trianglef;
typedef Triangle<double> Triangled;

} //namespace ml


#endif
