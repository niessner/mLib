#ifndef CORE_MESH_MESHSHAPES_H_
#define CORE_MESH_MESHSHAPES_H_

namespace ml {

namespace shapes {

TriMeshf box(float xDim, float yDim, float zDim, const vec4f& color);

inline TriMeshf box(float dim = 1, const vec4f& color = vec4f(1, 0, 0, 1)) { return box(dim, dim, dim, color); }

TriMeshf cylinder(float radius, float height, UINT stacks, UINT slices, const vec4f& color);

TriMeshf cylinder(const vec3f &p0, const vec3f &p1, float radius, UINT stacks, UINT slices, const vec4f& color);

TriMeshf torus(const vec3f &center, float majorRadius, float minorRadius, UINT stacks, UINT slices, const std::function<vec4f(unsigned int)> &stackIndexToColor);

inline TriMeshf line(const vec3f& p0, const vec3f& p1, const vec4f& color = vec4f(1, 0, 0, 1), const float thickness = 0.005f) {
  return cylinder(p0, p1, thickness, 2, 4, color);
}

TriMeshf wireframeBox(float dim = 1, const vec4f& color = vec4f(1, 0, 0, 1));

TriMeshf wireframeBox(const mat4f& unitCube2world, const vec4f& color = vec4f(1, 0, 0, 1));

TriMeshf sphere(const float radius, const ml::vec3f& pos, const size_t stacks = 10, const size_t slices = 10, const ml::vec4f& color = ml::vec4f(1,1,1,1));

}  // namespace shapes

}  // namespace ml

#endif  // INCLUDE_CORE_MESH_MESHSHAPES_H_
