#ifndef INCLUDE_CORE_MESH_MESHSHAPES_H_
#define INCLUDE_CORE_MESH_MESHSHAPES_H_

namespace MeshShapes {

TriMesh box(float xDim, float yDim, float zDim, const Vec4f& color);

inline TriMesh box(float dim, const Vec4f& color = Vec4f(1.0f, 0.0f, 0.0f, 1.0f)) { return box(dim, dim, dim, color); }

TriMesh cylinder(float radius, float height, UINT stacks, UINT slices, const Vec4f& color);

TriMesh cylinder(const Vec3f &p0, const Vec3f &p1, float radius, UINT stacks, UINT slices, const Vec4f& color);

}  // namespace MeshShapes

#endif  // INCLUDE_CORE_MESH_MESHSHAPES_H_
