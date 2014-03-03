#ifndef INCLUDE_CORE_MESH_MESHUTIL_H_
#define INCLUDE_CORE_MESH_MESHUTIL_H_

#include <vector>

namespace MeshUtil
{
    TriMesh createPointCloudTemplate(const TriMesh& templateMesh, const Vector<vec3f>& points);
    TriMesh createPointCloudTemplate(const TriMesh& templateMesh, const std::vector<vec3f>& points,
                                     const std::vector<vec4f>& colors);
}

#endif  // INCLUDE_CORE_MESH_MESHUTIL_H_
