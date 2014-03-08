#ifndef INCLUDE_CORE_MESH_MESHUTIL_H_
#define INCLUDE_CORE_MESH_MESHUTIL_H_

namespace MeshUtil
{
    TriMesh createPointCloudTemplate(const TriMesh& templateMesh, const std::vector<vec3f>& points);
    TriMesh createPointCloudTemplate(const TriMesh& templateMesh, const std::vector<vec3f>& points, const std::vector<vec4f>& colors);
    TriMesh createUnifiedMesh(const std::vector< std::pair<TriMesh, mat4f> > &meshes);
}

#endif  // INCLUDE_CORE_MESH_MESHUTIL_H_
