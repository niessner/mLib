
#ifndef CORE_GRAPHICS_DIST_H_
#define CORE_GRAPHICS_DIST_H_

namespace ml {

//
// normally this would be a namespace (hence the lower-case letters),
// but I wanted to overload () extensively which requires a class.
// Except that operator() can't be static, so they are just overloaded
// functions in ml.
//
template <class T>
T distSq(const point3d<T> &ptA, const point3d<T> &ptB)
{
    return point3d<T>::distSq(ptA, ptB);
}

template <class T>
T distSq(const ObjectOrientedBoundingBox<T> &box, const point3d<T> &pt)
{
    //
    // This is wrong, this file is just meant as an example of the dist interface
    //
    return point3d<T>::distSq(box.getCenter(), pt);
}

template <class T>
T distSq(const point3d<T> &pt, const ObjectOrientedBoundingBox<T> &box)
{
    return (*this)(box, pt);
}

template <class A, class B>
double dist(const A &a, const B &b)
{
    return sqrt((double)distSq(a, b));
}

}  // namespace ml

#endif  // CORE_GRAPHICS_DIST_H_
