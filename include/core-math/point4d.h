
#ifndef _POINT4D_H_
#define _POINT4D_H_

#include "point3d.h"

#include <iostream>
#include <cmath>
#include <cstdlib>
#include <cassert>

//! 4D vector.
template <class T>
class point4d
{
public:
	point4d(T v) {
		array[0] = array[1] = array[2] = array[3] = v;
	}

	point4d() {
		array[0] = array[1] = array[2] = array[3] = 0;
	}

	point4d(T x, T y, T z, T w) {
		array[0] = x;
		array[1] = y;
		array[2] = z;
		array[3] = w;
	}

	point4d(const point3d<T>& other, T w = (T)1) {
		array[0] = other.array[0];
		array[1] = other.array[1];
		array[2] = other.array[2];
		array[3] = w;
	}

	point4d(const point4d& other) {
		array[0] = other.array[0];
		array[1] = other.array[1];
		array[2] = other.array[2];
		array[3] = other.array[3];
	}

	inline const point4d<T>& operator=(const point4d& other) {
		array[0] = other.array[0];
		array[1] = other.array[1];
		array[2] = other.array[2];
		array[3] = other.array[3];
		return *this;
	}

	inline const point4d<T>& operator=(T other) {
		array[0] = other;
		array[1] = other;
		array[2] = other;
		array[3] = other;
		return *this;
	}


	inline point4d<T> operator-() const {
		return point4d<T>(-array[0], -array[1], -array[2], -array[3]);
	}

	inline point4d<T> operator+(const point4d& other) const {
		return point4d<T>(array[0]+other.array[0], array[1]+other.array[1], array[2]+other.array[2], array[3]+other.array[3]);
	}

	inline void operator+=(const point4d& other) {
		array[0] += other.array[0];
		array[1] += other.array[1];
		array[2] += other.array[2];
		array[3] += other.array[3];
	}

	inline void operator-=(const point4d& other) {
		array[0] -= other.array[0];
		array[1] -= other.array[1];
		array[2] -= other.array[2];
		array[3] -= other.array[3];
	}

	inline void operator*=(T val) {
		array[0] *= val;
		array[1] *= val;
		array[2] *= val;
		array[3] *= val;
	}

	inline void operator/=(T val) {
		T inv = (T)1/inv;
		array[0] *= inv;
		array[1] *= inv;
		array[2] *= inv;
		array[3] *= inv;
	}

	inline point4d<T> operator*(T val) const {
		return point4d<T>(array[0]*val, array[1]*val, array[2]*val, array[3]*val);
	}

	inline point4d<T> operator/(T val) const {
		T inv = (T)1/val;
		return point4d<T>(array[0]*inv, array[1]*inv, array[2]*inv, array[3]*inv);
	}

	//! cross product (of .xyz)
	inline point4d<T> operator^(const point4d& other) const {
		return point4d<T>(array[1]*other.array[2] - array[2]*other.array[1], array[2]*other.array[0] - array[0]*other.array[2], array[0]*other.array[1] - array[1]*other.array[0], T(1));
	}

	//! dot product
	inline T operator|(const point4d& other) const {
		return (array[0]*other.array[0] + array[1]*other.array[1] + array[2]*other.array[2] + array[3]*other.array[3]);
	}

	inline point4d<T> operator-(const point4d& other) const {
		return point4d<T>(array[0]-other.array[0], array[1]-other.array[1], array[2]-other.array[2], array[3]-other.array[3]);
	}

	inline bool operator==(const point4d& other) const {
		if ((array[0] == other.array[0]) && (array[1] == other.array[1]) && (array[2] == other.array[2]) && (array[3] == other.array[3]))
			return true;

		return false;
	}

	//! opposite of ==
	inline bool operator!=(const point4d& other) const {
		return !(*this == other);
	}


	inline T lengthSq() const {
		return (array[0]*array[0] + array[1]*array[1] + array[2]*array[2] + array[3]*array[3]);
	}

	inline T length() const {
		return sqrt(lengthSq());
	}

	static T distSq(const point4d& v0, const point4d& v1) {
		return (
			(v0.array[0]-v1.array[0])*(v0.array[0]-v1.array[0]) +
			(v0.array[1]-v1.array[1])*(v0.array[1]-v1.array[1]) +
			(v0.array[2]-v1.array[2])*(v0.array[2]-v1.array[2]) +
			(v0.array[3]-v1.array[3])*(v0.array[3]-v1.array[3])
			);
	}

	static T dist(const point4d& v0, const point4d& v1) {
		return (*this - other).length();
	}

	~point4d(void) {};

	void print() const {
		std::cout << "(" << array[0] << " / " << array[1] << " / " << array[2] << " / " << array[3] << " ) " << std::endl;
	}

	inline const T& operator[](int i) const {
		assert(i < 4);
		return array[i];
	}

	inline T& operator[](int i) {
		assert(i < 4);
		return array[i];
	}

	inline void normalize() {
		T inv = (T)1/length();
		array[0] *= inv;
		array[1] *= inv;
		array[2] *= inv;
		array[3] *= inv;
	}

	inline point4d getNormalized() const {
		T inv = (T)1/length();
		return point4d<T>(array[0]/val, array[1]/val, array[2]/val, array[3]/val);
	}

	inline void dehomogenize() {
		array[0] /= array[3];
		array[1] /= array[3];
		array[2] /= array[3];
		array[3] /= array[3];
	}


	inline bool isLinearDependent(const point4d& other) const {
		T factor = x/other.x;

		if ((std::fabs(x/factor - other.x) + std::fabs(y/factor - other.y) + std::fabs(z/factor - other.z) + std::fabs(w/factor - other.w)) < 0.00001)
			return true;
		else
			return false;
	}

	union {
		struct {
			T x,y,z,w;          // standard names for components
		};
		T array[4];     // array access
	};
};

//! operator for scalar * vector
template <class T>
__forceinline point4d<T> operator*(T s, const point4d<T>& v) {
	return v * s;
}

//! write a point4d to a stream (should be the inverse of read operator; with " ")
template <class T> 
__forceinline std::ostream& operator<<(std::ostream& s, const point4d<T>& v)
{ return (s << v[0] << " / " << v[1] << " / " << v[2] << " / " << v[3]);}

//! read a point4d from a stream
template <class T>
__forceinline std::istream& operator>>(std::istream& s, point4d<T>& v)
{ return (s >> v[0] >> v[1] >> v[2] >> v[3]); }


typedef point4d<double> vec4d;
typedef point4d<float> vec4f;
typedef point4d<int> vec4i;
typedef point4d<unsigned int> vec4ui;
typedef point4d<unsigned char> vec4uc;


#endif
