#ifndef INCLUDE_CORE_MESH_MESHSHAPES_H_
#define INCLUDE_CORE_MESH_MESHSHAPES_H_

namespace MeshShapes {

TriMesh box(float xDim, float yDim, float zDim);

inline TriMesh box(float dim) { return box(dim, dim, dim); }

}  // namespace MeshShapes

#endif  // INCLUDE_CORE_MESH_MESHSHAPES_H_
