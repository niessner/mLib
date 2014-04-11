#ifndef CORE_MESH_MESHUTIL_H_
#define CORE_MESH_MESHUTIL_H_

namespace ml {
namespace meshutil {

TriMesh createPointCloudTemplate(const TriMesh& templateMesh, const std::vector<vec3f>& points, const std::vector<vec4f>& colors = std::vector<vec4f>());
TriMesh createUnifiedMesh(const std::vector<std::pair<TriMesh, mat4f>>& meshes);
TriMesh createUnifiedMesh(const std::vector<TriMesh>& meshes);

} // namespace meshutil
} // namespace ml

#endif  // CORE_MESH_MESHUTIL_H_
