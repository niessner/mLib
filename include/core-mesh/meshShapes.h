#ifndef INCLUDE_CORE_MESH_MESHSHAPES_H_
#define INCLUDE_CORE_MESH_MESHSHAPES_H_

namespace MeshShapes {

TriMesh box(float xDim, float yDim, float zDim, const vec4f& color = vec4f(1.0f, 0.0f, 0.0f, 1.0f));

inline TriMesh box(float dim, const vec4f& color = vec4f(1.0f, 0.0f, 0.0f, 1.0f)) { return box(dim, dim, dim, color); }

}  // namespace MeshShapes

#endif  // INCLUDE_CORE_MESH_MESHSHAPES_H_
