#ifndef CORE_MESH_MESHSHAPES_H_
#define CORE_MESH_MESHSHAPES_H_

namespace ml {

namespace shapes {

TriMeshf box(float xDim, float yDim, float zDim, const vec3f& color);

inline TriMeshf box(float dim = 1, const vec3f& color = vec3f(1, 0, 0)) { return box(dim, dim, dim, color); }

TriMeshf cylinder(float radius, float height, UINT stacks, UINT slices, const vec4f& color);

TriMeshf cylinder(const vec3f &p0, const vec3f &p1, float radius, UINT stacks, UINT slices, const vec3f& color);

TriMeshf torus(const vec3f &center, float majorRadius, float minorRadius, UINT stacks, UINT slices, const std::function<vec3f(unsigned int)> &stackIndexToColor);

inline TriMeshf line(const vec3f& p0, const vec3f& p1, const vec3f& color = vec3f(1, 0, 0), const float thickness = 0.005f) {
  return cylinder(p0, p1, thickness, 2, 4, color);
}

TriMeshf wireframeBox(float dim = 1, const vec3f& color = vec3f(1, 0, 0));

TriMeshf wireframeBox(const mat4f& unitCube2world, const vec3f& color = vec3f(1, 0, 0));

}  // namespace shapes

}  // namespace ml

#endif  // INCLUDE_CORE_MESH_MESHSHAPES_H_
