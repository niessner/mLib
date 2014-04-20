#ifndef CORE_MESH_MESHSHAPES_H_
#define CORE_MESH_MESHSHAPES_H_

namespace ml {

namespace shapes {

TriMeshOld box(float xDim, float yDim, float zDim, const vec4f& color);

inline TriMeshOld box(float dim = 1, const vec4f& color = vec4f(1, 0, 0, 1)) { return box(dim, dim, dim, color); }

TriMeshOld cylinder(float radius, float height, UINT stacks, UINT slices, const vec4f& color);

TriMeshOld cylinder(const vec3f &p0, const vec3f &p1, float radius, UINT stacks, UINT slices, const vec4f& color);

inline TriMeshOld line(const vec3f& p0, const vec3f& p1, const vec4f& color = vec4f(1, 0, 0, 1), const float thickness = 0.005f) {
  return cylinder(p0, p1, thickness, 2, 4, color);
}

TriMeshOld wireframeBox(float dim = 1, const vec4f& color = vec4f(1, 0, 0, 1));

TriMeshOld wireframeBox(const mat4f& unitCube2world, const vec4f& color = vec4f(1, 0, 0, 1));

}  // namespace shapes

}  // namespace ml

#endif  // INCLUDE_CORE_MESH_MESHSHAPES_H_
