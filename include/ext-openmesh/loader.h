#ifndef INCLUDE_EXT_OPENMESH_LOADER_H_
#define INCLUDE_EXT_OPENMESH_LOADER_H_

#include <string>

namespace OpenMeshLoader {
typedef OpenMesh::TriMesh_ArrayKernelT<> Mesh;

static TriMesh load(const std::string& filename) {
  Mesh mesh;
  if (!OpenMesh::IO::read_mesh(mesh, filename)) {
    MLIB_ERROR("error reading from " + filename);
  }
  mesh.request_vertex_colors();
  mesh.request_vertex_normals();
  const size_t nVertices = mesh.n_vertices();
  const size_t nIndices = mesh.n_faces() * 3;  // Should always be tri meshes
  Vector<MeshVertex> vertices(nVertices);
  Vector<UINT> indices(nIndices);

  MeshVertex mv;
  UINT currVertIdx = 0;
  for (Mesh::VertexIter vIt = mesh.vertices_begin(); vIt != mesh.vertices_end();
       ++vIt, currVertIdx++) {
    const Mesh::Point& p = mesh.point(*vIt);
    const Mesh::Normal& n = mesh.normal(*vIt);
    const Mesh::Color& c = mesh.color(*vIt);
    mv.position = p.data();
    mv.normal = n.data();
    mv.attributeA = vec4f(c[0], c[1], c[2], 255);  // c is of type vec3uc
    vertices[currVertIdx] = mv;
  }
  UINT currIndexIdx = 0;
  for (Mesh::FaceIter fIt = mesh.faces_begin(); fIt != mesh.faces_end();
       ++fIt) {
    for (Mesh::FaceVertexIter fvIt = mesh.fv_iter(*fIt);
         fvIt != mesh.fv_end(*fIt); fvIt++, currIndexIdx++) {
      indices[currIndexIdx] = fvIt->idx();
    }
  }

  MLIB_ASSERT_STR(currVertIdx == nVertices, "nVertices != vertices parsed");
  MLIB_ASSERT_STR(currIndexIdx == nIndices, "nIndices != indices parsed");

  return TriMesh(vertices, indices);
}

}  // namespace OpenMeshLoader

#endif  // INCLUDE_EXT_OPENMESH_LOADER_H_
