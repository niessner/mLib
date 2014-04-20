#ifndef CORE_MESH_MESHUTIL_H_
#define CORE_MESH_MESHUTIL_H_

namespace ml {
namespace meshutil {

TriMeshOld createPointCloudTemplate(const TriMeshOld& templateMesh, const std::vector<vec3f>& points, const std::vector<vec4f>& colors = std::vector<vec4f>());
TriMeshOld createUnifiedMesh(const std::vector<std::pair<TriMeshOld, mat4f>>& meshes);
TriMeshOld createUnifiedMesh(const std::vector<TriMeshOld>& meshes);

} // namespace meshutil
} // namespace ml

#endif  // CORE_MESH_MESHUTIL_H_
