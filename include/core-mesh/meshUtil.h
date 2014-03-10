#ifndef INCLUDE_CORE_MESH_MESHUTIL_H_
#define INCLUDE_CORE_MESH_MESHUTIL_H_

namespace MeshUtil
{
    TriMesh createPointCloudTemplate(const TriMesh& templateMesh, const std::vector<Vec3f>& points);
    TriMesh createPointCloudTemplate(const TriMesh& templateMesh, const std::vector<Vec3f>& points, const std::vector<Vec4f>& colors);
    TriMesh createUnifiedMesh(const std::vector< std::pair<TriMesh, Mat4f> > &meshes);
}

#endif  // INCLUDE_CORE_MESH_MESHUTIL_H_
