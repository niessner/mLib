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
private:

};

typedef CGALWrapper<float>	CGALWrapperf;
typedef CGALWrapper<double>	CGALWrapperd;

} // end namespace

#endif
