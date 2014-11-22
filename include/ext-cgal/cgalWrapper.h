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

private:

};

typedef CGALWrapper<float>	CGALWrapperf;
typedef CGALWrapper<double>	CGALWrapperd;

} // end namespace

#endif
