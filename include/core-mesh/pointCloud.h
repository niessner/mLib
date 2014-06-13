#ifndef CORE_MESH_POINTCLOUD_H_
#define CORE_MESH_POINTCLOUD_H_

namespace ml {

template <class FloatType>
class PointCloud {
public:
	PointCloud() {}
	PointCloud(PointCloud&& pc) {
		m_points = std::move(pc.m_points);
		m_normals = std::move(pc.m_normals);
		m_colors = std::move(pc.m_colors);
	}
	void operator=(PointCloud&& pc) {
		m_points = std::move(pc.m_points);
		m_normals = std::move(pc.m_normals);
		m_colors = std::move(pc.m_colors);
	}

	bool hasNormals() const {
		return m_normals.size() > 0;
	}
	bool hasColors() const {
		return m_colors.size() > 0;
	}

	void clear() {
		m_points.clear();
		m_normals.clear();
		m_colors.clear();
	}

	std::vector<point3d<FloatType>> m_points;
	std::vector<point3d<FloatType>> m_normals;
	std::vector<point3d<FloatType>> m_colors;
private:
};

typedef PointCloud<float>	PointCloudf;
typedef PointCloud<double>	PointCloudd;

} // namespace ml


#endif