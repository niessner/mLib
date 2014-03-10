#ifndef CORE_MESH_MESHUTIL_H_
#define CORE_MESH_MESHUTIL_H_

namespace ml {

namespace meshutil {
    TriMesh createPointCloudTemplate(const TriMesh& templateMesh, const std::vector<Vec3f>& points);
    TriMesh createPointCloudTemplate(const TriMesh& templateMesh, const std::vector<Vec3f>& points, const std::vector<Vec4f>& colors);
    TriMesh createUnifiedMesh(const std::vector< std::pair<TriMesh, Mat4f> > &meshes);
} // namespace meshutil

} // namespace ml

#endif  // CORE_MESH_MESHUTIL_H_
