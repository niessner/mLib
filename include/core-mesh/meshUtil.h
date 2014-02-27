#ifndef INCLUDE_CORE_MESH_MESHUTIL_H_
#define INCLUDE_CORE_MESH_MESHUTIL_H_

namespace MeshUtil
{
    TriMesh createPointCloudTemplate(const TriMesh& templateMesh, const Vector<vec3f>& points);
}

#endif  // INCLUDE_CORE_MESH_MESHUTIL_H_
