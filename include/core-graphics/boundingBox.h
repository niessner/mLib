#pragma once

#ifndef _BOUNDINGBOX_H_
#define _BOUNDINGBOX_H_

#include "Ray.h"

#include <cfloat>
#include <vector>
#include <cmath>
#include <iostream>

template<class FloatType>
class BoundingBox3d
{
public:

	BoundingBox3d(void)
	{
		reset();
	}

	BoundingBox3d(const std::vector<point3d<FloatType>>& verts) 
	{
		reset();
		for (unsigned int i = 0; i < verts.size(); i++) {
			include(verts[i]);
		}
	}

	BoundingBox3d(const point3d<FloatType>& minBound, const point3d<FloatType>& maxBound) 
	{
		reset();
		minB = minBound;
		maxB = maxBound;
	}


	~BoundingBox3d(void)
	{

	}
	

	void reset()
	{
		minX = minY = minZ = std::numeric_limits<FloatType>::max();
		maxX = maxY = maxZ = -std::numeric_limits<FloatType>::max();
	}

	void include(const BoundingBox3d &other)
	{
		if (other.minX < minX)	minX = other.minX;
		if (other.minY < minY)	minY = other.minY;
		if (other.minZ < minZ)	minZ = other.minZ;

		if (other.maxX > maxX)	maxX = other.maxX;
		if (other.maxY > maxY)	maxY = other.maxY;
		if (other.maxZ > maxZ)	maxZ = other.maxZ;
	}

	void include(const point3d<FloatType> &v)
	{
		if (v.x < minX)	minX = v.x;
		if (v.y < minY)	minY = v.y;
		if (v.z < minZ)	minZ = v.z;

		if (v.x > maxX)	maxX = v.x;
		if (v.y > maxY)	maxY = v.y;
		if (v.z > maxZ)	maxZ = v.z;
	}

	bool intersect(const Ray<FloatType> &r) const
	{

		const FloatType t0 = 0.0;
		const FloatType t1 = r.t;

		FloatType txmin, txmax, tymin, tymax, tzmin, tzmax;


		txmin = (parameters[r.sign().x*3] - r.origin().x) * r.inverseDirection().x;
		txmax = (parameters[3-r.sign().x*3] - r.origin().x) * r.inverseDirection().x;
		tymin = (parameters[r.sign().y*3 + 1] - r.origin().y) * r.inverseDirection().y;
		tymax = (parameters[3-r.sign().y*3 + 1] - r.origin().y) * r.inverseDirection().y;

		if ( (txmin > tymax) || (tymin > txmax) ) return false;

		if (tymin > txmin)	txmin = tymin;
		if (tymax < txmax)	txmax = tymax;

		tzmin = (parameters[r.sign().z*3 + 2] - r.origin().z) * r.inverseDirection().z;
		tzmax = (parameters[3-r.sign().z*3 + 2] - r.origin().z) * r.inverseDirection().z;

		if ( (txmin > tzmax) || (tzmin > txmax) ) 
			return false;
		if (tzmin > txmin)
			txmin = tzmin;
		if (tzmax < txmax)
			txmax = tzmax;
		return ( (txmin <= t1) && (txmax >= t0) );

	}

	FloatType getMaxExtend() const {
		FloatType d0 = maxX - minX;
		FloatType d1 = maxY - minY;
		FloatType d2 = maxZ - maxZ;
		return std::max(d0, std::max(d1,d2));
	}

	FloatType getExtentX() const {
		return maxX - minX;
	}

	FloatType getExtentY() const {
		return maxY - minY;
	}

	FloatType getExtentZ() const {
		return maxZ - minZ;
	}

	point3d<FloatType> getExtent() const {
		return float3(maxX - minX, maxY - minY, maxZ - minZ);
	}

	point3d<FloatType> getMin() const {
		return point3d<FloatType>(minX, minY, minZ);
	}

	point3d<FloatType> getMax() const {
		return point3d<FloatType>(maxX, maxY, maxZ);
	}

	point3d<FloatType> getCenter() const {
		point3d<FloatType> center = getMin() + getMax();
		center *= (FloatType)0.5;
		return center;
	}

	void setMin(const point3d<FloatType>& minValue) {
		minX = minValue.x;
		minY = minValue.y;
		minZ = minValue.x;
	}

	void setMax(const point3d<FloatType>& maxValue) {
		maxX = maxValue.x;
		maxY = maxValue.y;
		maxZ = maxValue.z;
	}

	//! scales the bounding box by the factor t (for t=1 the bb remains unchanged)
	void scale(FloatType x, FloatType y, FloatType z) {

		float scale[] = {x, y, z};
		for (unsigned int i = 0; i < 3; i++) {
			FloatType center = (FloatType)0.5 * (parameters[i] + parameters[i+3]);
			FloatType diff = parameters[i+3] - parameters[i];
			diff *= scale[i];
			diff *= (FloatType)0.5;
			parameters[i] = center - diff;
			parameters[i+3] = center + diff;
		}
	}

	//! scales the bounding box by the factor t (for t=1 the bb remains unchanged)
	void scale(FloatType t) {
		for (unsigned int i = 0; i < 3; i++) {
			FloatType center = (FloatType)0.5 * (parameters[i] + parameters[i+3]);
			FloatType diff = parameters[i+3] - parameters[i];
			diff *= t;
			diff *= (FloatType)0.5;
			parameters[i] = center - diff;
			parameters[i+3] = center + diff;
		}
	}

	//! scales the bounding box (see scale)
	BoundingBox3d<FloatType> operator*(FloatType t) const {
		BoundingBox3d<FloatType> res = *this;
		res.scale(t);
		return res;
	}

protected:
	union {
		struct {
			point3d<FloatType> minB;
			point3d<FloatType> maxB;
		};
		struct {
			FloatType minX, minY, minZ;
			FloatType maxX, maxY, maxZ;
		};
		FloatType parameters[6];
	};
};

template<class FloatType>
std::ostream& operator<< (std::ostream& s, const BoundingBox3d<FloatType>& bb) {
	s << bb.getMin() << std::endl << bb.getMax() << std::endl;
	return s;
}


typedef BoundingBox3d<float> bbox3f;
typedef BoundingBox3d<double> bbox3d;

#endif
