#ifndef INCLUDE_EXT_OPENMESH_LOADER_H_
#define INCLUDE_EXT_OPENMESH_LOADER_H_

#include <string>

namespace OpenMeshLoader
{

typedef OpenMesh::TriMesh_ArrayKernelT<OpenMesh::DefaultTraits> Mesh;

static TriMesh load(const std::string& filename)
{
    MLIB_ASSERT_STR(Utility::fileExists(filename), "File not found: " + filename + "\nWorking directory: " + Utility::workingDirectory());

    namespace io = OpenMesh::IO;
    io::Options opts = io::Options::VertexColor | io::Options::VertexNormal;

    Mesh mesh;
    mesh.request_vertex_colors();
    mesh.request_vertex_normals();

    if (!io::read_mesh(mesh, filename, opts))
    {
        MLIB_ERROR("error reading from " + filename);
    }

    const size_t nVertices = mesh.n_vertices();
    const size_t nIndices = mesh.n_faces() * 3;  // Should always be tri meshes
    std::vector<TriMesh::TriMeshVertex> vertices(nVertices);
    std::vector<UINT> indices(nIndices);

    TriMesh::TriMeshVertex mv;
    UINT currVertIdx = 0;
    for (Mesh::VertexIter vIt = mesh.vertices_begin(); vIt != mesh.vertices_end(); ++vIt, currVertIdx++)
    {
        const Mesh::Point& p = mesh.point(*vIt);    // p is vec3f
        const Mesh::Normal& n = mesh.normal(*vIt);  // n is vec3f
        const Mesh::Color& c = mesh.color(*vIt);    // c is vec3uc
        mv.position = p.data();
        mv.normal = n.data();
        mv.attributeA = vec4f(c[0] / 255.0f, c[1] / 255.0f, c[2] / 255.0f, 1.0f);
        vertices[currVertIdx] = mv;
    }
    UINT currIndexIdx = 0;
    for (Mesh::FaceIter fIt = mesh.faces_begin(); fIt != mesh.faces_end(); ++fIt)
    {
        for (Mesh::FaceVertexIter fvIt = mesh.fv_iter(*fIt);
                fvIt != mesh.fv_end(*fIt); fvIt++, currIndexIdx++)
        {
            indices[currIndexIdx] = fvIt->idx();
        }
    }

    MLIB_ASSERT_STR(currVertIdx == nVertices, "nVertices != vertices parsed");
    MLIB_ASSERT_STR(currIndexIdx == nIndices, "nIndices != indices parsed");

    return TriMesh(vertices, indices);
}

}  // namespace OpenMeshLoader

#endif  // INCLUDE_EXT_OPENMESH_LOADER_H_
