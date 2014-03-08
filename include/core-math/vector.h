#pragma once

#ifndef _VECTOR_H_
#define _VECTOR_H_


/************************************************************************/
/* Math vector class (used for instance with sparse matrices)           */
/************************************************************************/


template<class T>
class Vector : public std::vector<T>
{
public:
	Vector() {}
	Vector(size_t size) {
		resize(size);
	}
	Vector(size_t size, const T& val) {
		resize(size, val);
	}
	Vector(Vector&& v) : std::vector<T>(v) {
	}
	void operator=(Vector&& v) {
		std::vector<T>::operator =(v);
	}

	void operator+=(const Vector& other) {
		for (size_t i = 0; i < res.size(); i++) {
			(*this)[i] += other[i];
		}
	}
	void operator-=(const Vector& other) {
		for (size_t i = 0; i < res.size(); i++) {
			(*this)[i] -= other[i];
		}
	}
	void operator*=(const Vector& other) {
		for (size_t i = 0; i < res.size(); i++) {
			(*this)[i] *= other[i];
		}
	}
	void operator/=(const Vector& other) {
		for (size_t i = 0; i < res.size(); i++) {
			(*this)[i] /= other[i];
		}
	}
	void operator*=(const T& other) {
		for (size_t i = 0; i < res.size(); i++) {
			(*this)[i] *= other;
		}
	}
	void operator/=(const T& other) {
		for (size_t i = 0; i < res.size(); i++) {
			(*this)[i] /= other;
		}
	}


	Vector operator+(const Vector& other) const {
		Vector res(this->size());
		for (size_t i = 0; i < res.size(); i++) {
			res[i] = (*this)[i] + other[i];
		}
		return res;
	}
	Vector operator-(const Vector& other) const {
		Vector res(this->size());
		for (size_t i = 0; i < res.size(); i++) {
			res[i] = (*this)[i] - other[i];
		}
		return res;
	}
	Vector operator*(const Vector& other) const {
		Vector res(this->size());
		for (size_t i = 0; i < res.size(); i++) {
			res[i] = (*this)[i] * other[i];
		}
		return res;
	}
	Vector operator/(const Vector& other) const {
		Vector res(this->size());
		for (size_t i = 0; i < res.size(); i++) {
			res[i] = (*this)[i] / other[i];
		}
		return res;
	}
	Vector operator*(const T& other) const {
		Vector res(this->size());
		for (size_t i = 0; i < res.size(); i++) {
			res[i] = (*this)[i] * other;
		}
		return res;
	}
	Vector operator/(const T& other) const {
		Vector res(this->size());
		for (size_t i = 0; i < res.size(); i++) {
			res[i] = (*this)[i] / other;
		}
		return res;
	}



	//! dot product
	T operator|(const Vector& other) const {
		return dot(*this, other);
	} 

	static T dot(const Vector& v0, const Vector& v1) {
		T res = (T)0;
		for (size_t i = 0; i < v0.size(); i++) {
			res += v0[i] * v1[i];
		}
		return res;
	}
};


template<class T>
inline Vector<T> operator*(const T& v, const Vector<T>& other)  {
	Vector<T> res(other.size());
	for (size_t i = 0; i < res.size(); i++) {
		res[i] = other[i] * v;
	}
	return res;
}

template<class T>
inline Vector<T> operator/(const T& v, const Vector<T>& other)  {
	Vector<T> res(other.size());
	for (size_t i = 0; i < res.size(); i++) {
		res[i] = other[i] * v;
	}
	return res;
}

#endif
