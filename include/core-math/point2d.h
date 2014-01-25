
#ifndef _POINT2D_H_
#define _POINT2D_H_

// Disables the 'nameless struct/union' warning in VC.
#pragma warning( disable : 4201 )

#include "point1d.h"

#include <iostream>
#include <cmath>
#include <iostream>
#include <cassert>


//! 2D vector
template <class T>
class point2d	
{
public:
	point2d(T v) {
		array[0] = array[1] = v;
	}

	point2d(const T* other) {
		array[0] = other[0];
		array[1] = other[1];
	}

	point2d() {
		array[0] = array[1] = 0;
	}

	point2d(T x, T y) {
		array[0] = x;
		array[1] = y;
	}

	template <class U>
	point2d(const point2d<U>& other) {
		array[0] = (T)other.array[0];
		array[1] = (T)other.array[1];
	}

	point2d(const point2d& other) {
		array[0] = other.array[0];
		array[1] = other.array[1];
	}

	inline const point2d<T>& operator=(const point2d& other) {
		array[0] = other.array[0];
		array[1] = other.array[1];
		return *this;
	}

	inline const point2d<T>& operator=(T other) {
		array[0] = other;
		array[1] = other;
		return *this;
	}

	inline point2d<T> operator-() const {
		return point2d<T>(-array[0], -array[1]);
	}

	inline point2d<T> operator+(const point2d& other) const {
		return point2d<T>(array[0]+other.array[0], array[1]+other.array[1]);
	}

	inline void operator+=(const point2d& other) {
		array[0] += other.array[0];
		array[1] += other.array[1];
	}

	inline void operator-=(const point2d& other) {
		array[0] -= other.array[0];
		array[1] -= other.array[1];
	}

	inline void operator*=(T val) {
		array[0] *= val;
		array[1] *= val;
	}

	inline void operator/=(T val) {
		T inv = (T)1/val;
		array[0] *= inv;
		array[1] *= inv;
	}

	inline point2d<T> operator*(T val) const {
		return point2d<T>(array[0]*val, array[1]*val);
	}

	inline point2d<T> operator/(T val) const {
		T inv = (T)1/val;
		return point2d<T>(array[0]*inv, array[1]*inv);
	}

	inline point2d<T> operator-(const point2d& other) const {
		return point2d<T>(array[0]-other.array[0], array[1]-other.array[1]);
	}

	inline bool operator==(const point2d& other) {
		if ((array[0] == other.array[0]) && (array[1] == other.array[1]))
			return true;

		return false;
	}

	//! dot product
	inline T operator|(const point2d& other) const {
		return (array[0]*other.array[0] + array[1]*other.array[1]);
	}

	inline T& operator[](unsigned int i) {
		assert(i < 2);
		return array[i];
	}

	inline const T& operator[](unsigned int i) const {
		assert(i < 2);
		return array[i];
	}

	~point2d(void) {};

	inline bool operator<(const point2d& other) const {
		if ((x < other.x) && (y < other.y))
			return true;

		return false;
	}

	inline T lengthSq() const {
		return (array[0]*array[0] + array[1]*array[1]);
	}

	inline T length() const {
		return sqrt(array[0]*array[0] + array[1]*array[1]);
	}

	static T distSq(const point2d& v0, const point2d& v1) {
		return ((v0.array[0]-v1.array[0])*(v0.array[0]-v1.array[0]) + (v0.array[1]-v1.array[1])*(v0.array[1]-v1.array[1]));
	}

	static T dist(const point2d& other) {
		return sqrt((v0.array[0]-v1.array[0])*(v0.array[0]-v1.array[0]) + (v0.array[1]-v1.array[1])*(v0.array[1]-v1.array[1]));
	}

	inline point2d getNormalized() const {
		T inv = (T)1/length();
		return point2d<T>(array[0]*inv, array[1]*inv);
	}

	inline void normalize() {
		T inv = (T)1/length();
		array[0] *= inv;
		array[1] *= inv;
	}

	inline void print() const {
		Console::log() << "(" << array[0] << " / " << array[1] << ")" << std::endl;
	}


	union {
		struct {
			T x,y;      // standard names for components
		};
		T array[2];     // array access
	};
};


//! write a point2d to a stream
template <class T> __forceinline std::ostream& operator<<(std::ostream& s, const point2d<T>& v)
{ return (s << v[0] << " / " << v[1]);}

//! read a point2d from a stream
template <class T> __forceinline std::istream& operator>>(std::istream& s, point2d<T>& v)
{ return (s >> v[0] >> v[1]); }


typedef point2d<double> vec2d;
typedef point2d<float> vec2f;
typedef point2d<int> vec2i;
typedef point2d<unsigned int> vec2ui;
typedef point2d<unsigned char> vec2uc;


#endif

