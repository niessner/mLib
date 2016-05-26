
#ifndef CORE_MESH_POINTCLOUD_INL_H_
#define CORE_MESH_POINTCLOUD_INL_H_

namespace ml {
template<class FloatType>
std::ostream& operator<<(std::ostream& os, const PointCloud<FloatType>& pointCloud) {
	os << "MeshData:\n"
		<< "\tVertices:  " << pointCloud.m_points.size() << "\n"
		<< "\tColors:    " << pointCloud.m_colors.size() << "\n"
		<< "\tNormals:   " << pointCloud.m_normals.size() << "\n"
		<< std::endl;

	return os;
}
}

#endif