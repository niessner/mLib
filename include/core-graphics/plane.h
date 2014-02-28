#pragma once

#ifndef _PLANE_H_
#define _PLANE_H_

#include "point3d.h"

template<class FloatType>
class Plane {
public:
	Plane() {

	}

	Plane(const point3d<FloatType> *points) {
		createFromPoints(points);
	}

	Plane(const point3d<FloatType>& normal, FloatType dist) {
		m_Normal = normal;
		m_Distance = dist;
	}

	Plane(const point3d<FloatType> &normal, const point3d<FloatType>& p) {
		m_Normal = normal;
		m_Distance = m_Normal | p;
	}

	inline point3d<FloatType> getNormal() const {
		return m_Normal;
	}

	inline FloatType getDistance() const {
		return m_Distance;
	}

	inline FloatType distanceToPoint(const point3d<FloatType>& point) const {
		return (m_Normal | point) - m_Distance;
	}

	inline FloatType distanceToPointAbs(const point3d<FloatType>& point) const {
		return std::abs(distanceToPoint(point));
	}

	inline point3d<FloatType> projectPointToPlane(const point3d<FloatType>& point) const {
		return point - distanceToPoint(point) * getNormal();
	}
private:
	void createFromPoints( const point3d<FloatType> * points ) 
	{
		m_Normal = ((points[1] - points[0])^(points[2] - points[0])).getNormalized();
		m_Distance = (m_Normal | points[0]);
		//make sure normal points away from origin (i.e., distance is positive)
		if (m_Distance < (FloatType)0) {
			m_Distance = -m_Distance;
			m_Normal = -m_Normal;
		}	
	}

	point3d<FloatType> m_Normal;
	FloatType m_Distance;
};


#endif
