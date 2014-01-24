
#ifndef _POINT1D_H_
#define _POINT1D_H_

// Disables the 'nameless struct/union' warning in VC.
#pragma warning( disable : 4201 )

#include <iostream>
#include <cmath>
#include <iostream>
#include <cassert>

//! 1D vector (I know it's a joke, but we need it for compatibility reasons)
template <class T>
class point1d
{
public:
	point1d(T v) {
		array[0] = v;
	}

	point1d() {
		array[0] = 0;
	}

	template <class U>
	point1d(const point1d<U>& other) {
		array[0] = (T)other.array[0];
	}

	point1d(const point1d& other) {
		array[0] = other.array[0];
	}

	inline const point1d<T>& operator=(const point1d& other) {
		array[0] = other.array[0];
		return *this;
	}

	inline point1d<T> operator-() const {
		return point1d<T>(-array[0]);
	}

	inline point1d<T> operator+(const point1d& other) const {
		return point1d<T>(array[0]+other.array[0]);
	}

	inline void operator+=(const point1d& other) {
		array[0] += other.array[0];
	}

	inline void operator-=(const point1d& other) {
		array[0] -= other.array[0];
	}

	inline void operator*=(T val) {
		array[0] *= val;
	}

	inline void operator/=(T val) {
		array[0] /= val;
	}

	inline point1d<T> operator*(T val) const {
		return point1d<T>(array[0]*val);
	}

	inline point1d<T> operator/(T val) const {
		return point1d<T>(array[0]/val);
	}

	inline point1d<T> operator-(const point1d& other) const {
		return point1d<T>(array[0]-other.array[0]);
	}

	inline bool operator==(const point1d& other) const {
		if ((array[0] == other.array[0]))
			return true;

		return false;
	}


	//! dot product
	inline T operator|(const point1d& other) const {
		return (array[0]*other.array[0]);
	}

	inline T& operator[](unsigned int i) {
		assert(i < 1);
		return array[i];
	}

	inline const T& operator[](unsigned int i) const {
		assert(i < 1);
		return array[i];
	}

	~point1d(void) {};

	inline T lengthSq() const {
		return (array[0]*array[0]);
	}

	inline T length() const {
		return array[0];
	}

	static T distSq(const point1d& v0, const point1d& v1) {
		return (v0.array[0] - v1.array[1])*(v0.array[0] - v1.array[1]);
	}

	static T dist(const point1d& v0, const point1d& v1) {
		return std::abs(v0.array[0] - v1.array[1]);
	}

	inline point1d getNormalized() const {
		return point1d<T>();
	}

	inline void normalize() const {
		array[0] /= length();
	}

	inline void print() const {
		std::cout << "(" << array[0] << ")" << std::endl;
	}

	union {
		struct {
			T x;        // standard names for components
		};
		T array[1];     // array access
	};
};


//! write a point1d to a stream
template <class T> __forceinline std::ostream& operator<<(std::ostream& s, const point1d<T>& v)
{ return (s << v[0]);}

//! read a point1d from a stream
template <class T> __forceinline std::istream& operator>>(std::istream& s, point1d<T>& v)
{ return (s >> v[0]); }


typedef point1d<double> vec1d;
typedef point1d<float> vec1f;
typedef point1d<int> vec1i;

#endif
