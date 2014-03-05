#ifndef INCLUDE_CORE_MESH_MESHUTIL_H_
#define INCLUDE_CORE_MESH_MESHUTIL_H_

#include <vector>

namespace MeshUtil
{
    TriMesh createPointCloudTemplate(const TriMesh& templateMesh, const Vector<vec3f>& points);
    TriMesh createPointCloudTemplate(const TriMesh& templateMesh, const Vector<vec3f>& points,
                                     const Vector<vec4f>& colors);
    TriMesh createPointCloudTemplate(const TriMesh& templateMesh, const std::vector<vec3f>& points,
                                     const vec4f& color);  // Same color for all points
}

#endif  // INCLUDE_CORE_MESH_MESHUTIL_H_
