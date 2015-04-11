
#ifndef CORE_MATH_POINT6D_H_
#define CORE_MATH_POINT6D_H_

#include "vec4.h"

#include <iostream>
#include <cmath>
#include <cstdlib>
#include <cassert>

namespace ml {

//! 6D vector.
template <class T>
class vec6 : public BinaryDataSerialize< vec6<T> >
{
public:
	explicit vec6(T v) {
		array[0] = array[1] = array[2] = array[3] = array[4] = array[5] = v;
	}

	vec6() {
		array[0] = array[1] = array[2] = array[3] = array[4] = array[5] = 0;
	}

	vec6(T x, T y, T z, T xx, T yy, T zz) {
		array[0] = x;
		array[1] = y;
		array[2] = z;
		array[3] = xx;
		array[4] = yy;
		array[5] = zz;
	}

	template <class U>
	vec6(const vec6<U>& other) {
		array[0] = (T)other.array[0];
		array[1] = (T)other.array[1];
		array[2] = (T)other.array[2];
		array[3] = (T)other.array[3];
		array[4] = (T)other.array[4];
		array[5] = (T)other.array[5];
	}

	vec6(const vec6& other) {
		array[0] = other.array[0];
		array[1] = other.array[1];
		array[2] = other.array[2];
		array[3] = other.array[3];
		array[4] = other.array[4];
		array[5] = other.array[5];
	}

	vec6(const T* other) {
		array[0] = other[0];
		array[1] = other[1];
		array[2] = other[2];
		array[3] = other[3];
		array[4] = other[4];
		array[5] = other[5];
	}

	inline const vec6<T>& operator=(const vec6& other) {
		array[0] = other.array[0];
		array[1] = other.array[1];
		array[2] = other.array[2];
		array[3] = other.array[3];
		array[4] = other.array[4];
		array[5] = other.array[5];
		return *this;
	}

	inline const vec6<T>& operator=(T other) {
		array[0] = other;
		array[1] = other;
		array[2] = other;
		array[3] = other;
		array[4] = other;
		array[5] = other;
		return *this;
	}

	inline bool operator!=(const vec6& other) const {
		return !(*this == other);
	}

	inline vec6<T> operator-() const {
		return vec6<T>(-array[0], -array[1], -array[2], -array[3], -array[4], -array[5]);
	}

	inline vec6<T> operator+(const vec6& other) const {
		return vec6<T>(array[0]+other.array[0], array[1]+other.array[1], array[2]+other.array[2],
						  array[3]+other.array[3], array[4]+other.array[4], array[5]+other.array[5]);
	}

	inline vec6<T> operator+(T val) const {
		return vec6<T>(array[0]+val, array[1]+val, array[2]+val, array[3]+val, array[4]+val, array[5]+val);
	}

	inline void operator+=(const vec6& other) {
		array[0] += other.array[0];
		array[1] += other.array[1];
		array[2] += other.array[2];
		array[3] += other.array[3];
		array[4] += other.array[4];
		array[5] += other.array[5];
	}

	inline void operator-=(const vec6& other) {
		array[0] -= other.array[0];
		array[1] -= other.array[1];
		array[2] -= other.array[2];
		array[3] -= other.array[3];
		array[4] -= other.array[4];
		array[5] -= other.array[5];
	}

	inline void operator+=(T val) {
		array[0] += val;
		array[1] += val;
		array[2] += val;
		array[3] += val;
		array[4] += val;
		array[5] += val;
	}

	inline void operator-=(T val) {
		array[0] -= val;
		array[1] -= val;
		array[2] -= val;
		array[3] -= val;
		array[4] += val;
		array[5] += val;
	}

	inline void operator*=(T val) {
		array[0] *= val;
		array[1] *= val;
		array[2] *= val;
		array[3] *= val;
		array[4] *= val;
		array[5] *= val;
	}

	inline void operator/=(T val) {

		T inv_val = ((T)1)/(val);

		array[0] *= inv_val;
		array[1] *= inv_val;
		array[2] *= inv_val;
		array[3] *= inv_val;
		array[4] *= inv_val;
		array[5] *= inv_val;
	}

	inline vec6<T> operator*(T val) const {
		return vec6<T>(array[0]*val, array[1]*val, array[2]*val, array[3]*val, array[4]*val, array[5]*val);
	}

	inline vec6<T> operator/(T val) const {
		return vec6<T>(array[0]/val, array[1]/val, array[2]/val, array[3]/val, array[4]/val, array[5]/val);
	}

	//! dot product
	inline T operator|(const vec6& other) const {
		return (array[0]*other.array[0] + array[1]*other.array[1] + array[2]*other.array[2] + array[3]*other.array[3] + array[4]*other.array[4] + array[5]*other.array[5]);
	}

	inline vec6<T> operator-(const vec6& other) const {
		return vec6<T>(array[0]-other.array[0], array[1]-other.array[1], array[2]-other.array[2], array[3]-other.array[3], array[4]-other.array[4], array[5]-other.array[5]);
	}

	inline vec6<T> operator-(T val) const {
		return vec6<T>(array[0]-val, array[1]-val, array[2]-val, array[3]-val, array[4]-val, array[5]-val);
	}

	inline bool operator==(const vec6& other) const {
		if ((array[0] == other.array[0]) && (array[1] == other.array[1]) && (array[2] == other.array[2])
			&& (array[3] == other.array[3]) && (array[4] == other.array[4]) && (array[5] == other.array[5]))
			return true;

		return false;
	}

	inline T lengthSq() const {
		return (array[0]*array[0] + array[1]*array[1] + array[2]*array[2] + array[3]*array[3] + array[4]*array[4] + array[5]*array[5]);
	}

	inline T length() const {
		return sqrt(lengthSq());
	}

	static T distSq(const vec6& v0, const vec6& v1) {
		return (v0-v1).lengthSq();
	}

	static T dist(const vec6& v0, const vec6& v1) {
		return (v0-v1).length();
	}

	inline void print() const {
		Console::log() << "(" << array[0] << " / " << array[1] << " / " << array[2] << " / " << array[3] << " / " << array[4] << " / " << array[5] << ")" << std::endl;
	}


	const T& operator[](unsigned int i) const {
		assert(i < 6);
		return array[i];
	}

	T& operator[](unsigned int i) {
		assert(i < 6);
		return array[i];

	}

	inline void normalize() {
		T inv = (T)1/length();
		array[0] *= inv;
		array[1] *= inv;
		array[2] *= inv;
		array[3] *= inv;
		array[4] *= inv;
		array[5] *= inv;
	}

	inline vec6<T> getNormalized() const {
		T val = (T)1.0 / length();
		return vec6<T>(array[0] * val, array[1] * val, array[2] * val, array[3] * val, array[4] * val, array[5] * val);
	}

    static inline void normalize(const vec6 &v) {
        return v.getNormalized();
    }

	inline T* ptr() {
		return &array[0];
	}

	inline std::vector<T> toStdVector() const {
		std::vector<T> result(6);
		result[0] = x;
		result[1] = y;
		result[2] = z;
		result[3] = xx;
		result[4] = yy;
		result[5] = zz;
		return result;
	}

	inline vec1<T> getVec1() const {
		return vec1<T>(x);
	}
	inline vec2<T> getVec2() const {
		return vec2<T>(x,y);
	}
	inline vec3<T> getVec3() const {
		return vec3<T>(x,y,z);
	}
	inline vec4<T> getVec4() const {
		return vec4<T>(x,y,z,xx);
	}

	static const vec6<T> origin;

	union {
		struct {
			T x,y,z, xx, yy, zz;	// standard names for components
		};
		T array[6];					// array access
	};
};

//! operator for scalar * vector
template <class T>
inline vec6<T> operator*(T s, const vec6<T>& v)
{
	return v * s;
}
template <class T>
inline vec6<T> operator/(T s, const vec6<T>& v)
{
	return vec6<T>(s/v.x, s/v.y, s/v.z, s/v.xx, s/v.yy, s/v.zz);
}
template <class T>
inline vec6<T> operator+(T s, const vec6<T>& v)
{
	return v + s;
}
template <class T>
inline vec6<T> operator-(T s, const vec6<T>& v)
{
	return vec6<T>(s-v.x, s-v.y, s-v.z, s-v.xx, s-v.yy, s-v.zz);
}

namespace math {
	template<class T>
	inline vec6<int> sign(const vec6<T>& v) {
		return vec6<int>(sign(v.x), sign(v.y), sign(v.z), sign(v.xx), sign(v.yy), sign(v.zz));
	}
}

//! write a vec6 to a stream
template <class T> 
inline std::ostream& operator<<(std::ostream& s, const vec6<T>& v) {
  return (s << v[0] << " " << v[1] << " " << v[2] << " " << v[3] << " " << v[4] << " " << v[5]);
}

//! read a vec6 from a stream
template <class T> 
inline std::istream& operator>>(std::istream& s, vec6<T>& v) {
  return (s >> v[0] >> v[1] >> v[2] >> v[3] >> v[4] >> v[5]);
}


typedef vec6<double> vec6d;
typedef vec6<float> vec6f;
typedef vec6<int> vec6i;
typedef vec6<unsigned int> vec6ui;
typedef vec6<unsigned char> vec6uc;
typedef vec6<UINT64> vec6ul;
typedef vec6<INT64> vec6l;



}  // namespace ml

#endif  // CORE_MATH_POINT6D_H_
