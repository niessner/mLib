namespace MeshShapes
{

float CubeVData[8][3] =
{
    {1.0f, 1.0f, 1.0f}, { -1.0f, 1.0f, 1.0f}, { -1.0f, -1.0f, 1.0f},
    {1.0f, -1.0f, 1.0f}, {1.0f, 1.0f, -1.0f}, { -1.0f, 1.0f, -1.0f},
    { -1.0f, -1.0f, -1.0f}, {1.0f, -1.0f, -1.0f}
};

int CubeIData[12][3] =
{
    {1, 2, 3}, {1, 3, 0}, {0, 3, 7}, {0, 7, 4}, {3, 2, 6},
    {3, 6, 7}, {1, 6, 2}, {1, 5, 6}, {0, 5, 1}, {0, 4, 5},
    {6, 5, 4}, {6, 4, 7}
};

TriMesh box(float xDim, float yDim, float zDim)
{
    Vector<MeshVertex> V(8);
    Vector<UINT> I(12*3);

    // Vertices
    for (int i = 0; i < 8; i++)
    {
        V[i].position = vec3f(CubeVData[i][0], CubeVData[i][1], CubeVData[i][2]);
        V[i].normal = vec3f(1.0f, 0.0f, 0.0f);  // TODO(ms): Set correct cube normal
        V[i].attributeA = vec4f(1.0f, 0.0f, 0.0f, 1.0f);
    }

    // Triangles
    for (int i = 0; i < 12; i++)
    {
        I[i * 3 + 0] = CubeIData[i][0];
        I[i * 3 + 1] = CubeIData[i][1];
        I[i * 3 + 2] = CubeIData[i][2];
    }

    TriMesh mesh(V, I);
    mesh.stretch(vec3f(0.5f * xDim, 0.5f * yDim, 0.5f * zDim));

    return mesh;
}

}  // namespace MeshShapes
