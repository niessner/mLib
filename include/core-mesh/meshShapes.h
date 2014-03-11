#ifndef CORE_MESH_MESHSHAPES_H_
#define CORE_MESH_MESHSHAPES_H_

namespace ml {

namespace shapes {

TriMesh box(float xDim, float yDim, float zDim, const vec4f& color);

inline TriMesh box(float dim, const vec4f& color = vec4f(1.0f, 0.0f, 0.0f, 1.0f)) { return box(dim, dim, dim, color); }

TriMesh cylinder(float radius, float height, UINT stacks, UINT slices, const vec4f& color);

TriMesh cylinder(const vec3f &p0, const vec3f &p1, float radius, UINT stacks, UINT slices, const vec4f& color);

}  // namespace shapes

}  // namespace ml

#endif  // INCLUDE_CORE_MESH_MESHSHAPES_H_
