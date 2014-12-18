#pragma once

#ifndef _TRIANGLE_H_
#define _TRIANGLE_H_

namespace ml {

template<class T>
struct Triangle
{
    point3d<T> getNormal() const
    {
        return ml::math::triangleNormal(vertices[0], vertices[1], vertices[2]);
    }

    //wouldn't this be prettier if it were vec3<FloatType>? :)
    point3d<T> vertices[3];

};

typedef Triangle<float> Trianglef;
typedef Triangle<double> Triangled;

} //namespace ml


#endif
