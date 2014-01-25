#ifndef _POINT3D_H_
#define _POINT3D_H_


#include <iostream>
#include <cmath>

#include "point2d.h"


//! 3D vector.
template <class T>
class point3d
{
public:
	point3d(T v) {
		array[0] = array[1] = array[2] = v;
	}

	point3d() {
		array[0] = array[1] = array[2] = 0;
	}

	point3d(const T &x, const T &y, const T &z) {
		array[0] = x;
		array[1] = y;
		array[2] = z;
	}

	template <class U>
	point3d(const point3d<U>& other) {
		array[0] = (T)other.array[0];
		array[1] = (T)other.array[1];
		array[2] = (T)other.array[2];
	}

	point3d(const point3d& other) {
		array[0] = other.array[0];
		array[1] = other.array[1];
		array[2] = other.array[2];
	}

	point3d(const T* other) {
		array[0] = other[0];
		array[1] = other[1];
		array[2] = other[2];
	}

	inline const point3d<T>& operator=(const point3d& other) {
		array[0] = other.array[0];
		array[1] = other.array[1];
		array[2] = other.array[2];
		return *this;
	}

	inline const point3d<T>& operator=(T other) {
		array[0] = other;
		array[1] = other;
		array[2] = other;
		return *this;
	}

	inline point3d<T> operator-() const {
		return point3d<T>(-array[0], -array[1], -array[2]);
	}

	inline point3d<T> operator+(const point3d& other) const {
		return point3d<T>(array[0]+other.array[0], array[1]+other.array[1], array[2]+other.array[2]);
	}

	inline void operator+=(const point3d& other) {
		array[0] += other.array[0];
		array[1] += other.array[1];
		array[2] += other.array[2];
	}

	inline void operator-=(const point3d& other) {
		array[0] -= other.array[0];
		array[1] -= other.array[1];
		array[2] -= other.array[2];
	}

	inline void operator*=(T val) {
		array[0] *= val;
		array[1] *= val;
		array[2] *= val;
	}

	inline void operator/=(T val) {
		T inv_val = ((T)1)/(val);

		array[0] *= inv_val;
		array[1] *= inv_val;
		array[2] *= inv_val;
	}

	inline point3d<T> operator*(T val) const {
		return point3d<T>(array[0]*val, array[1]*val, array[2]*val);
	}

	inline point3d<T> operator/(T val) const {
		return point3d<T>(array[0]/val, array[1]/val, array[2]/val);
	}

	//! Cross product
	inline point3d<T> operator^(const point3d& other) const {
		return point3d<T>(array[1]*other.array[2] - array[2]*other.array[1], array[2]*other.array[0] - array[0]*other.array[2], array[0]*other.array[1] - array[1]*other.array[0]);
	}

	//! Dot product
	inline T operator|(const point3d& other) const {
		return (array[0]*other.array[0] + array[1]*other.array[1] + array[2]*other.array[2]);
	}

	inline point3d<T> operator-(const point3d& other) const {
		return point3d<T>(array[0]-other.array[0], array[1]-other.array[1], array[2]-other.array[2]);
	}

	inline bool operator==(const point3d& other) const {
		if ((array[0] == other.array[0]) && (array[1] == other.array[1]) && (array[2] == other.array[2]))
			return true;

		return false;
	}

	inline bool operator!=(const point3d& other) const {
		return !((*this)==other);
	}


	inline T lengthSq() const {
		return (array[0]*array[0] + array[1]*array[1] + array[2]*array[2]);
	}

	inline T length() const {
		return sqrt(squaredLength());
	}

	static T distSq(const point3d& v0, const point3d& v1) {
		return ((v0.array[0]-v1.array[0])*(v0.array[0]-v1.array[0]) + (v0.array[1]-v1.array[1])*(v0.array[1]-v1.array[1]) + (v0.array[2]-v1.array[2])*(v0.array[2]-v1.array[2]));
	}

	static T dist(const point3d& v0, const point3d& v1) {
		return sqrt((v0.array[0]-v1.array[0])*(v0.array[0]-v1.array[0]) + (v0.array[1]-v1.array[1])*(v0.array[1]-v1.array[1]) + (v0.array[2]-v1.array[2])*(v0.array[2]-v1.array[2]));
	}


	inline operator T*() {
		return array;
	}
	
	inline operator const T*() const {
		return array;
	}
	
	~point3d(void) {};


	inline void print() const {
		Console::log() << "(" << array[0] << " / " << array[1] << " / " << array[2] << ")" << std::endl;
	}

	const T& operator[](int i) const {
		assert(i < 3);
		return array[i];
	}

	T& operator[](int i) {
		assert(i < 3);
		return array[i];
	}

	inline point3d getNormalized() const {
		T val = length();
		return point3d<T>(array[0]/val, array[1]/val, array[2]/val);
	}

	inline void normalize() {
		T val = length();
		array[0] /= val;
		array[1] /= val;
		array[2] /= val;
	}


	union {
		struct {
			T x,y,z;    // standard names for components
		};
		T array[3];     // array access
	};
};

//! operator for scalar * vector
template <class T>
inline point3d<T> operator*(T s, const point3d<T>& v) {
	return v * s;
}

//! write a point3d to a stream (should be the inverse of input operator; with " ")
template <class T> 
inline std::ostream& operator<<(std::ostream& s, const point3d<T>& v)
{ return (s << v[0] << " / " << v[1] << " / " << v[2]);}

//! read a point3d from a stream
template <class T> 
inline std::istream& operator>>(std::istream& s, point3d<T>& v)
{ return (s >> v[0] >> v[1] >> v[2]); }


typedef point3d<double> vec3d;
typedef point3d<float> vec3f;
typedef point3d<int> vec3i;
typedef point3d<short> vec3s;
typedef point3d<unsigned int> vec3ui;
typedef point3d<unsigned char> vec3uc;



#endif
