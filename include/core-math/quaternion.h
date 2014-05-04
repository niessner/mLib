#pragma once

#ifndef _QUATERNION_H_
#define _QUATERNION_H_


namespace ml {

template <class T> class Matrix4x4;
template <class T> class Matrix3x3;

//! Quaternions are used to describe rotations
template <class T> class Quaternion {
	public:
		//! construct a quaternion that does no rotation
		Quaternion();

		Quaternion(T r, T i, T j, T k) {
			re = r;
			im = point3d<T>(i,j,k);
		}
		//! construct a quaternion explicitly
		Quaternion( T real, const point3d<T>& imag );
		//! construct a quaternion given a rotation-axis and an angle
		/*! \param angle is specified in degrees */
		Quaternion( const point3d<T>& axis, T angle );
		//! Copy-Constructor
		Quaternion( const Quaternion& b );

		//! Constructs a quaternion between a start and end point
		Quaternion(const point3d<T>& from, const point3d<T>& to) {
			point3d<T> vecHalf = (from + to).getNormalized();
			re = vecHalf | to;
			im = vecHalf ^ to;
		}

		inline float sgn(float x) {return (x >= 0.0f) ? +1.0f : -1.0f;}

		//! construct a quaternion based on a matrix: todo check that!
		Quaternion(const Matrix3x3<T>& m) {
			T m00 = m(0,0);	T m01 = m(0,1);	T m02 = m(0,2);
			T m10 = m(1,0);	T m11 = m(1,1);	T m12 = m(1,2);
			T m20 = m(2,0);	T m21 = m(2,1);	T m22 = m(2,2);

			T tr = m00 + m11 + m22;

			T qw, qx, qy, qz;
				if (tr > 0) { 
					T S = sqrt(tr+(T)1.0) * 2; // S=4*qw 
					qw = (T)0.25 * S;
					qx = (m21 - m12) / S;
					qy = (m02 - m20) / S; 
					qz = (m10 - m01) / S; 
				} else if ((m00 > m11)&(m00 > m22)) { 
					T S = sqrt((T)1.0 + m00 - m11 - m22) * (T)2; // S=4*qx 
					qw = (m21 - m12) / S;
					qx = (T)0.25 * S;
					qy = (m01 + m10) / S; 
					qz = (m02 + m20) / S; 
				} else if (m11 > m22) { 
					T S = sqrt((T)1.0 + m11 - m00 - m22) * (T)2; // S=4*qy
					qw = (m02 - m20) / S;
					qx = (m01 + m10) / S; 
					qy = (T)0.25 * S;
					qz = (m12 + m21) / S; 
				} else { 
					T S = sqrt((T)1.0 + m22 - m00 - m11) * (T)2; // S=4*qz
					qw = (m10 - m01) / S;
					qx = (m02 + m20) / S;
					qy = (m12 + m21) / S;
					qz = (T)0.25 * S;
				}
			re = qw;
			im = point3d<T>(qx,qy,qz);
			this->invert();

			/*
			T r11 = m(0,0);	T r12 = m(0,1);	T r13 = m(0,2);
			T r21 = m(1,0);	T r22 = m(1,1);	T r23 = m(1,2);
			T r31 = m(2,0);	T r32 = m(2,1);	T r33 = m(2,2);

			T q0 = ( r11 + r22 + r33 + 1.0f) / 4.0f;
			T q1 = ( r11 - r22 - r33 + 1.0f) / 4.0f;
			T q2 = (-r11 + r22 - r33 + 1.0f) / 4.0f;
			T q3 = (-r11 - r22 + r33 + 1.0f) / 4.0f;
			if(q0 < 0.0f) q0 = 0.0f;
			if(q1 < 0.0f) q1 = 0.0f;
			if(q2 < 0.0f) q2 = 0.0f;
			if(q3 < 0.0f) q3 = 0.0f;
			q0 = sqrt(q0);
			q1 = sqrt(q1);
			q2 = sqrt(q2);
			q3 = sqrt(q3);
			if(q0 >= q1 && q0 >= q2 && q0 >= q3) {
				q0 *= +1.0f;
				q1 *= sgn(r32 - r23);
				q2 *= sgn(r13 - r31);
				q3 *= sgn(r21 - r12);
			} else if(q1 >= q0 && q1 >= q2 && q1 >= q3) {
				q0 *= sgn(r32 - r23);
				q1 *= +1.0f;
				q2 *= sgn(r21 + r12);
				q3 *= sgn(r13 + r31);
			} else if(q2 >= q0 && q2 >= q1 && q2 >= q3) {
				q0 *= sgn(r13 - r31);
				q1 *= sgn(r21 + r12);
				q2 *= +1.0f;
				q3 *= sgn(r32 + r23);
			} else if(q3 >= q0 && q3 >= q1 && q3 >= q2) {
				q0 *= sgn(r21 - r12);
				q1 *= sgn(r31 + r13);
				q2 *= sgn(r32 + r23);
				q3 *= +1.0f;
			} else {
				printf("coding error\n");
			}
			T r = point4d<T>(q0, q1, q2, q3).length();
			q0 /= r;
			q1 /= r;
			q2 /= r;
			q3 /= r;

			re = q3;
			im = point3d<T>(q0,q1,q2);
			*/

		}

		//! sets the quaternion to 1,0,0,0 (i.e., no rotation)
		inline void reset() {
			re = 1;
			im = point3d<T>(0,0,0);
		}

		//! query the real part of the quaternion
		inline T real() const;
		//! query the imaginary part of the quaternion
		inline point3d<T> imag() const;

		//! return quaternion as LinAl vector
		//inline doubleVec LinAlVec() const;

		//! set real part of the quaternion
		inline void setReal( const T r );

		//! set imaginary part of the quaternion
		inline void setImag( const point3d<T>& imag );

		//! query the axis of the rotation described by the quaternion
		point3d<T> axis() const;
		//! query the angle of the rotation described by the quaternion in radians
		inline T angleRad() const;
		//! query the angle of the rotation described by the quaternion in degrees
		inline T angleDeg() const;
		//! query the transformation-matrix of the rotation described by the quaternion
		inline Matrix4x4<T> matrix4x4() const;
		inline Matrix3x3<T> matrix3x3() const;

		//! returns the squared length of the quaternion
		inline T sqrLength() const;
		//! returns the length of the quaternion
		inline T length() const;

		//! return the normalized quaternion as a copy
		inline Quaternion normalized();
		//! sets the length of the quaternion to one
		inline void normalize();

		//! the absolute-value of a quaternion is its length
		//inline T abs(const Quaternion& q);
		//! the multiplication operator that allows the scalar value to preceed the quaternion
		//inline Quaternion operator* (T r, const Quaternion& q);

		//! add two quaternions
		inline Quaternion operator+ ( const Quaternion& b ) const;
		//! add quaternion b to operand
		inline void operator+=( const Quaternion& b );
		//! multiply two quaternions
		inline Quaternion operator* ( const Quaternion& b ) const;
		//! multiply quaternions b to operand
		inline Quaternion operator*=( const Quaternion& b );
		//! scale quaternion with a scalar
		inline Quaternion operator* ( T r ) const;
		//! multiply quaternion with a scalar
		inline void operator*=( T r );
		//! divide quaternion by scalar
		inline Quaternion operator/ ( T r ) const;
		//! divide quaternion by scalar
		inline void operator/=( T r );

		//! returns the scalar-product of the quaternion with the argument
		inline T scalarProd( const Quaternion& b ) const;

		//! return the conjugated quaternion
		inline Quaternion conjugated() const;

		//! return the quaternion that performs the inverse rotation
		inline Quaternion inverted() const;
		//! set the quaternion to the one that performs the inverse rotation
		inline void invert();

		//! calculates a spherical linear interpolation between the quaternions
		/*! If t==0 the result is the quaternion from which the method was called.
		    If t==1 the result is the quaternion q2.
		    Inbetween the quaternions are weighted
		*/
		Quaternion slerp( const Quaternion& q2, T t ) const;

	private:
		//! the real part of the quaternion
		T re;
		//! the imaginary prt of the quaternion as a 3-dimensional vector
		point3d<T> im;
		//! read a quaternion from a stream
		template <class t> friend std::istream& operator>> ( std::istream& s, Quaternion<T>& q );

		static const T EPSILON;
	};

//};	// namespace Math

#include "matrix4x4.h"
#include "matrix3x3.h"

//namespace Math {

template <class T> const T Quaternion<T>::EPSILON = ( T ) 0.00001;

// ********************************
// INLINE-functions for general use
// ********************************

//! the absolute-value of a quaternion is its length
template <class T> inline T abs( const Quaternion<T>& q ) { return q.length(); }

//! The multiplication operator that allows the scalar value to preceed the quaternion
template <class T> inline Quaternion<T> operator* ( T r, const Quaternion<T>& q ) { return q * r; }

//! write a quaternion to a stream
template <class T> inline std::ostream& operator<<( std::ostream& s, const Quaternion<T>& q ) { return ( s << q.real() << " " << q.imag() ); }

//! read a quaternion from a stream
template <class T> inline std::istream& operator>>( std::istream& s, Quaternion<T>& q ) { return ( s >> q.re >> q.im ); }


// ********************************
// INLINE-functions for Quaternion
// ********************************

template <class T> inline T Quaternion<T>::real() const { return re; }

template <class T> inline point3d<T> Quaternion<T>::imag() const { return im; }

// inline doubleVec Quaternion<T>::LinAlVec() const
// {
//   doubleVec vec(4);
//   vec(0) = re;
//   vec(1) = im[0];
//   vec(2) = im[1];
//   vec(3) = im[2];
//
//   return vec;
// }

template <class T> inline void Quaternion<T>::setReal( const T r ) { re = r; }

template <class T> inline void Quaternion<T>::setImag( const point3d<T>& imag ) { im = imag; }

template <class T> inline T Quaternion<T>::angleRad() const { return acos( re ); }

template <class T> inline T Quaternion<T>::angleDeg() const { return ( angleRad() * ( 360.0 / M_PI ) ); }

template <class T> inline Matrix4x4<T> Quaternion<T>::matrix4x4() const {
	Matrix4x4<T> m;

	float q[ 4 ];
	q[ 0 ] = im[ 0 ];
	q[ 1 ] = im[ 1 ];
	q[ 2 ] = im[ 2 ];
	q[ 3 ] = re;

	T s, xs, ys, zs, wx, wy, wz, xx, xy, xz, yy, yz, zz;
	s = ( T ) 2.0 / ( q[ 0 ] * q[ 0 ] + q[ 1 ] * q[ 1 ] + q[ 2 ] * q[ 2 ] + q[ 3 ] * q[ 3 ] );

	xs = q[ 0 ] * s; ys = q[ 1 ] * s; zs = q[ 2 ] * s;
	wx = q[ 3 ] * xs; wy = q[ 3 ] * ys; wz = q[ 3 ] * zs;
	xx = q[ 0 ] * xs; xy = q[ 0 ] * ys; xz = q[ 0 ] * zs;
	yy = q[ 1 ] * ys; yz = q[ 1 ] * zs; zz = q[ 2 ] * zs;

	m(0,0) = ( T ) 1.0 - ( yy + zz );	m(0,1) = xy + wz;					m(0,2) = xz - wy;					m(0,3) = 0;
	m(1,0) = xy - wz;					m(1,1) = ( T ) 1.0 - ( xx + zz );	m(1,2) = yz + wx;					m(1,3) = 0;
	m(2,0) = xz + wy;					m(2,1) = yz - wx;					m(2,2) = ( T ) 1.0 - ( xx + yy );	m(2,3) = 0;
	m(3,0) = 0;							m(3,1) = 0;							m(3,2) = 0;							m(3,3) = 1;

	return m;
}

template <class T> inline Matrix3x3<T> Quaternion<T>::matrix3x3() const {
	Matrix3x3<T> m;

	float q[ 4 ];
	q[ 0 ] = im[ 0 ];
	q[ 1 ] = im[ 1 ];
	q[ 2 ] = im[ 2 ];
	q[ 3 ] = re;

	T s, xs, ys, zs, wx, wy, wz, xx, xy, xz, yy, yz, zz;
	s = ( T ) 2.0 / ( q[ 0 ] * q[ 0 ] + q[ 1 ] * q[ 1 ] + q[ 2 ] * q[ 2 ] + q[ 3 ] * q[ 3 ] );

	xs = q[ 0 ] * s;	ys = q[ 1 ] * s;	zs = q[ 2 ] * s;
	wx = q[ 3 ] * xs;	wy = q[ 3 ] * ys;	wz = q[ 3 ] * zs;
	xx = q[ 0 ] * xs;	xy = q[ 0 ] * ys;	xz = q[ 0 ] * zs;
	yy = q[ 1 ] * ys;	yz = q[ 1 ] * zs;	zz = q[ 2 ] * zs;

	m(0,0) = ( T ) 1.0 - ( yy + zz );	m(0,1) = xy + wz;					m(0,2) = xz - wy;					
	m(1,0) = xy - wz;					m(1,1) = ( T ) 1.0 - ( xx + zz );	m(1,2) = yz + wx;				
	m(2,0) = xz + wy;					m(2,1) = yz - wx;					m(2,2) = ( T ) 1.0 - ( xx + yy );

	return m;
}

template <class T> inline T Quaternion<T>::sqrLength() const { return re * re + im[ 0 ] * im[ 0 ] + im[ 1 ] * im[ 1 ] + im[ 2 ] * im[ 2 ]; }

template <class T> inline T Quaternion<T>::length() const { return sqrt( sqrLength() ); }

template <class T> inline Quaternion<T> Quaternion<T>::normalized() {
	T thisLength = length();
	//if(!isZero(thisLength))
	if ( thisLength > Quaternion<T>::EPSILON )
		return Quaternion( re / thisLength, im / thisLength );
	else
		return Quaternion( 1, point3d<T>( 0, 0, 0 ) );
	}

template <class T> inline void Quaternion<T>::normalize() {
	T thisLength = length();
	//if(!isZero(thisLength)) {
	if ( thisLength > Quaternion<T>::EPSILON ) {
		re /= thisLength;
		im /= thisLength;
		} else {
		re = 1;
		im = point3d<T>( 0, 0, 0 );
		}
	}

template <class T> inline Quaternion<T> Quaternion<T>::operator+ ( const Quaternion& b ) const { return Quaternion( b.re + re, b.im + im ); }

template <class T> inline void Quaternion<T>::operator+=( const Quaternion& b ) { re += b.re; im += b.im; }

template <class T> inline Quaternion<T> Quaternion<T>::operator* ( const Quaternion& b ) const {
	T re2 = ( re * b.re ) - ( im | b.im );	// | = dot product
	point3d<T> im2 = ( b.im * re ) + ( im * b.re ) + ( im ^ b.im ); // ^ = cross product
	return Quaternion( re2, im2 );
	}

template <class T> inline Quaternion<T> Quaternion<T>::operator*=( const Quaternion& b ) {
	( *this ) = ( *this ) * b;
	return *this;
	}

template <class T> inline Quaternion<T> Quaternion<T>::operator* ( T r ) const { return Quaternion( r * re, r * im ); }

template <class T> inline void Quaternion<T>::operator*=( T r ) { re *= r; im *= r; }

template <class T> inline Quaternion<T> Quaternion<T>::operator/ ( T r ) const {
	assert( !isZero( r ) );
	return Quaternion( re / r, im / r );
	}

template <class T> inline void Quaternion<T>::operator/=( T r ) {
	assert( !isZero( r ) );
	re /= r; im /= r;
	}

template <class T> inline T Quaternion<T>::scalarProd( const Quaternion& b ) const { 
	return ( re * b.re + (im | b.im ) ); 
}

template <class T> inline Quaternion<T> Quaternion<T>::conjugated() const { return Quaternion( re, point3d<T>( -im[ 0 ], -im[ 1 ], -im[ 2 ] ) ); }

template <class T> inline Quaternion<T> Quaternion<T>::inverted() const {
		T l = length();
		assert(l != (T)0.0);
		//assert( !isZero( l ) );
		return Quaternion( re / l, point3d<T>( -im[ 0 ], -im[ 1 ], -im[ 2 ] ) / l );
	}

template <class T> inline void Quaternion<T>::invert() { *this = inverted(); }

// ****************************
// IMPLEMENTATION of Quaternion
// ****************************

template <class T> Quaternion<T>::Quaternion() : re( 1 ), im(0, 0, 0) {}


template <class T> Quaternion<T>::Quaternion( T real, const point3d<T>& imag ) : re( real ), im( imag ) {}

template <class T> Quaternion<T>::Quaternion( const point3d<T>& axis, T angle ) {
	T halfAngleRad = ( T ) M_PI * angle / ( T ) 360.0;
	T axisLength = axis.length();
	if ( axisLength > Quaternion<T>::EPSILON ) {
		re = cos( halfAngleRad );
		im = axis * ( sin( halfAngleRad ) / axisLength );
		} else {
		re = 1;
		im = point3d<T>( 0, 0, 0 );
		}
	}

template <class T> Quaternion<T>::Quaternion( const Quaternion<T>& b ) : re( b.re ), im( b.im ) { }

template <class T> point3d<T> Quaternion<T>::axis() const {
	T halfAngle = acos( re );
	T s = sin( halfAngle );
	point3d<T> a;
	if ( isZero( s ) ) {
		a[ 0 ] = 1.0;
		a[ 1 ] = 0.0;
		a[ 2 ] = 0.0;
		} else {
		T c = 1.0 / s;
		a = c * im;
		}
	return a;
	}

template <class T> Quaternion<T> Quaternion<T>::slerp( const Quaternion<T>& q2, T t ) const {
	const T delta = (T)0.0001;
	Quaternion result;
	T Omega, CosOmega, SinOmega, scale0, scale1;

	// scalar product of the two quaternions is the cosine of the angle between them
	CosOmega = scalarProd( q2 );

	// test if they are exacltly opposite
	if ( ( 1.0 + CosOmega ) > delta ) {
		// if they are too close together calculate only lerp
		if ( ( 1.0 - CosOmega ) > delta ) {
			// slerp
			Omega = acos( CosOmega );
			SinOmega = sin( Omega );
			scale0 = sin( ( (T)1.0 - t ) * Omega ) / SinOmega;
			scale1 = sin( t * Omega ) / SinOmega;
			} else {
			// lerp
			scale0 = (T)1.0 - t;
			scale1 = t;
			}
		result.im[ 0 ] = scale0 * im[ 0 ] + scale1 * q2.im[ 0 ];
		result.im[ 1 ] = scale0 * im[ 1 ] + scale1 * q2.im[ 1 ];
		result.im[ 2 ] = scale0 * im[ 2 ] + scale1 * q2.im[ 2 ];
		result.re = scale0 * re + scale1 * q2.re;
		} else {
		// quaternions are opposite
		// calculate orthogonal quaternion and use it to calculate slerp
		result.im[ 0 ] = -q2.im[ 1 ];
		result.im[ 1 ] = q2.im[ 0 ];
		result.im[ 2 ] = -q2.re;
		result.re = q2.im[ 2 ];

		// slerp
		scale0 = sin( ( (T)1.0 - t ) * (T)M_PI / (T)2.0 );
		scale1 = sin( t * (T)M_PI / (T)2.0 );
		result.im[ 0 ] = scale0 * im[ 0 ] + scale1 * result.im[ 0 ];
		result.im[ 1 ] = scale0 * im[ 1 ] + scale1 * result.im[ 1 ];
		result.im[ 2 ] = scale0 * im[ 2 ] + scale1 * result.im[ 2 ];
		result.re = scale0 * re + scale1 * result.re;
		}

	return result;
	}

typedef Quaternion<double> quatd;
typedef Quaternion<float> quatf;

}	// namespace ml

#endif
