
namespace MeshShapes
{
    TriMesh box(float xDim, float yDim, float zDim);

    inline TriMesh box(float dim)
    {
        return box(dim, dim, dim);
    }
}