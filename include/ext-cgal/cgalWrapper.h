#ifndef _CGAL_WRAPPER_H_
#define _CGAL_WRAPPER_H_


namespace ml {

template <class FloatType>
class CGALWrapper {
public:
	static std::vector<point3d<FloatType>> convexHull(typename std::vector<point3d<FloatType>>::const_iterator pBegin, typename std::vector<point3d<FloatType>>::const_iterator pEnd) {

		typedef CGAL::Exact_predicates_inexact_constructions_kernel  K;
		typedef CGAL::Polyhedron_3<K>                                Polyhedron_3;
		typedef K::Point_3                                           Point_3;

		const size_t nPoints = pEnd - pBegin;
		std::vector<Point_3> cgalPoints;
		cgalPoints.reserve(nPoints);
		for (auto& it = pBegin; it != pEnd; ++it) {
			const auto& p = *it;
			cgalPoints.push_back(Point_3(p[0], p[1], p[2]));
		}

		Polyhedron_3 poly;
		CGAL::convex_hull_3(std::begin(cgalPoints), std::end(cgalPoints), poly);
		poly.vertices_begin();
		std::vector<point3d<FloatType>> out(poly.size_of_vertices());

		size_t i = 0;
		for (auto it = poly.vertices_begin(); it != poly.vertices_end(); ++it, ++i) {
			point3d<FloatType>& v = out[i];
			const auto& p = it->point();
			v[0] = static_cast<FloatType>(p[0]);
			v[1] = static_cast<FloatType>(p[1]);
			v[2] = static_cast<FloatType>(p[2]);
		}

		return out;
	}

	static std::vector<point3d<FloatType>> convexHull(const std::vector<point3d<FloatType>>& points) {
		return convexHull(points.begin(), points.end());
	}




	static std::vector<point2d<FloatType>> minRectangle2D(typename std::vector<point2d<FloatType>>::const_iterator pBegin, typename std::vector<point2d<FloatType>>::const_iterator pEnd) {
		typedef CGAL::Simple_cartesian<float>        K;
		typedef K::Point_2                           Point_2;
		typedef K::Line_2                            Line_2;
		typedef CGAL::Polygon_2<K>                   Polygon_2;

		const size_t nPoints = pEnd - pBegin;
		std::vector<Point_2> cgalPoints;
		cgalPoints.reserve(nPoints);
		for (auto& it = pBegin; it != pEnd; ++it) {
			const auto& p = *it;
			cgalPoints.push_back(Point_2(p[0], p[1]));
		}

		Polygon_2 p;
		CGAL::convex_hull_2(begin(cgalPoints), end(cgalPoints),
			std::back_inserter(p));

		Polygon_2 p_m;
		CGAL::min_rectangle_2(p.vertices_begin(), p.vertices_end(),
			std::back_inserter(p_m));

		std::vector<point2d<FloatType>> out(p_m.size());
		size_t i = 0;
		for (auto it = p_m.vertices_begin(); it != p_m.vertices_end(); ++it, ++i) {
			point2d<FloatType>& v = out[i];
			v[0] = static_cast<float>(it->x());
			v[1] = static_cast<float>(it->y());
		}
		return out;
	}

	static std::vector<point2d<FloatType> > minRectangle2D(const std::vector < point2d<FloatType> >& points) { 
		return minRectangle2D(points.begin(), points.end());  
	}

	enum FitOpts {
		PCA = 1 << 0,  // OBB axes determined through PCA decomposition of point set
		MIN_PCA = 1 << 1,  // Use minimum PCA axis and determine other axes through 2D project + min rectangle fit
		AABB_FALLBACK = 1 << 2,  // Fall back to AABB if volume of AABB is within 10% of OBB
		CONSTRAIN_Z = 1 << 3,  // Constrain OBB z axis to be canonical z axis
		DEFAULT_OPTS = (MIN_PCA | AABB_FALLBACK)
	};
	typedef FlagSet<FitOpts> FitOptFlags;

	static OrientedBoundingBox3<FloatType> computeOrientedBoundingBox(typename std::vector<point3d<FloatType>>::const_iterator pBegin, typename std::vector<point3d<FloatType>>::const_iterator pEnd, const FitOptFlags opts = DEFAULT_OPTS) {
		const size_t nPoints = pEnd - pBegin;

		if (opts[CONSTRAIN_Z]) {
			// Get centroid, z range and x-y points for 2D rect fitting
			std::vector<point2d<FloatType>> projPs(nPoints);
			size_t i = 0;
			FloatType big = std::numeric_limits<FloatType>::max();
			Vec3f pMin(big, big, big), pMax(-big, -big, -big);
			for (VecVec3fIter it = pBegin; it != pEnd; it++, i++) {
				const FloatType x = (*it)[0], y = (*it)[1], z = (*it)[2];
				if (x < pMin[0]) { pMin[0] = x; }
				else if (x > pMax[0]) { pMax[0] = x; }
				if (y < pMin[1]) { pMin[1] = y; }
				else if (y > pMax[1]) { pMax[1] = y; }
				if (z < pMin[2]) { pMin[2] = z; }
				else if (z > pMax[2]) { pMax[2] = z; }
				projPs[i][0] = x;  projPs[i][1] = y;
			}

			// Find minimum rectangle in x-y plane
			const std::vector<point2d<FloatType>>& rectPts = minRectangle2D(projPs);

			// Set x and y bbox axes from 2D rectangle axes
			const point2d<FloatType>& v0 = rectPts[1] - rectPts[0], v1 = rectPts[2] - rectPts[1];
			const FloatType v0norm2 = v0.squaredNorm(), v1norm2 = v1.squaredNorm();
			size_t v0idx = (v0norm2 > v1norm2) ? 0 : 1;
			size_t v1idx = (v0idx + 1) % 2;
			const point2d<FloatType>& v0n = v0.normalized(), v1n = v1.normalized();
			R_.col(v0idx) = point3d<FloatType>(v0n[0], v0n[1], 0);  r_[v0idx] = sqrtf(v0norm2) * (FloatType)0.5;
			R_.col(v1idx) = point3d<FloatType>(v1n[0], v1n[1], 0);  r_[v1idx] = sqrtf(v1norm2) * (FloatType)0.5;
			R_.col(2) = point3d<FloatType>(0, 0, 1);                r_[2] = (pMax[2] - pMin[2]) * (FloatType)0.5;
			c_ = (pMin + pMax) * (FloatType)0.5;
		}
		else {
			const CoordSystem coords(pBegin, pEnd);
			R_ = coords.R();

			if (opts[MIN_PCA]) {
				// Project points into 2D plane formed by the first two eigenvector
				// in R's columns. The plane normal is the last eigenvector
				std::vector<point2d<FloatType>> projPs(nPoints);
				size_t i = 0;
				const auto Mproj = R_.transpose().topLeftCorner<2, 3>();
				for (const auto it = pBegin; it != pEnd; it++, i++) {
					projPs[i] = Mproj * (*it);
				}

				// Find minimum rectangle in that plane
				const std::vector<point2d<FloatType>>& rectPts = minRectangle2D(projPs);

				// Set new bbox axes v0 and v1 from 2D rectangle's axes by first getting
				// back their 3D world space coordinates and then ordering by length so
				// that v0 remains largest OBB dimension, followed by v1
				const point2d<FloatType> pV0 = rectPts[1] - rectPts[0], pV1 = rectPts[2] - rectPts[1];
				const point3d<FloatType> bv0 = Mproj.transpose() * pV0, bv1 = Mproj.transpose() * pV1;
				const float bv0norm = bv0.squaredNorm(), bv1norm = bv1.squaredNorm();
				R_.col(0) = (bv0norm > bv1norm) ? bv0.normalized() : bv1.normalized();
				R_.col(1) = (bv0norm > bv1norm) ? bv1.normalized() : bv0.normalized();
				R_.col(2) = R_.col(0).cross(R_.col(1));
			}

			// Regularize basis by swapping axes so that Y always points upwards
			R_ = regularizeBasisMatrix(R_, coords.S().diagonal());

			// Find half-extents and centroid by fitting BBox in local space
			Matrix3f Rt = R_.transpose();
			BBox bbox;
			for (VecVec3fIter pIt = pBegin; pIt != pEnd; pIt++) {
				bbox.extend(Rt * (*pIt));
			}

			// Can now decide if AABB is a better or almost as good fit and revert to it
			if (opts[AABB_FALLBACK]) {
				BBox aabb;
				for (const auto p = pBegin; p != pEnd; p++) { aabb.extend(*p); }
				const FloatType AABBvolume = aabb.volume();
				const FloatType OBBvolume = bbox.volume();
				// Threshold OBB volume multiplier under which to use AABB instead
				const FloatType eps = (FloatType)1.1;
				if (AABBvolume < eps * OBBvolume) {
					MLIB_WARNING("Reverting to AABB");
					//sg::util::println("Reverting to AABB");

					// Adjust AABB axes for longest-to-shortest ordering
					FloatType dims[3];
					Eigen::Map<Vec3f> m(dims);
					m = aabb.diagonal();
					std::vector<size_t> ind;
					sg::util::sortIndices(std::begin(dims), std::end(dims), std::less<float>(), ind);
					const Matrix3f& I = Matrix3f::Identity();
					const point3d<FloatType> x = I.col(ind[2]);
					const point3d<FloatType> y = I.col(ind[1]);
					const point3d<FloatType> z = I.col(ind[0]);
					R_.col(0) = x;
					R_.col(1) = y;
					R_.col(2) = z;
					const point3d<FloatType> sortedDims(m[ind[2]], m[ind[1]], m[ind[0]]);
					R_ = regularizeBasisMatrix(R_, sortedDims);

					// Recompute dimensions with new AABB basis
					bbox.setEmpty();
					Matrix3f Rt2 = R_.transpose();
					for (VecVec3fIter pIt = pBegin; pIt != pEnd; pIt++) {
						bbox.extend(Rt2 * (*pIt));
					}
				}
			}

			// Save half-widths and centroid of BBox
			r_ = (bbox.max() - bbox.min()) * (FloatType)0.5;
			c_ = R_ * (bbox.max() + bbox.min()) * (FloatType)0.5;
		}  // !opts[CONSTRAIN_Z]

		// Avoid degenerate boxes by enforcing non-zero width along all dimensions
		const FloatType minWidth = (FloatType)1e-10f;
		for (size_t i = 0; i < 3; i++) {
			if (r_[i] < minWidth) { r_[i] = minWidth; }
		}

		// Local-to-world transform
		for (size_t i = 0; i < 3; i++) {
			localToWorld_.linear().col(i) = R_.col(i) * r_[i];
		}
		localToWorld_.translation() = c_;

		// World-to-local transform. Points within OBB are in [0,1]^3
		for (size_t i = 0; i < 3; i++) {
			worldToLocal_.linear().row(i) = R_.col(i) * ((FloatType)1.0 / r_[i]);
		}
		worldToLocal_.translation() = -worldToLocal_.linear() * c_;
	}

	static OrientedBoundingBox3<FloatType> computeOrientedBoundingBox(const std::vector<point3d<FloatType>>& points, const FitOptFlags opts = DEFAULT_OPTS) {
		return OrientedBoundingBox3(points.begin(), points.end(), opts);
	}

private:

};

typedef CGALWrapper<float>	CGALWrapperf;
typedef CGALWrapper<double>	CGALWrapperd;

} // end namespace

#endif
