namespace ml {

namespace shapes
{

    float cubeVData[8][3] =
    {
        {1.0f, 1.0f, 1.0f}, { -1.0f, 1.0f, 1.0f}, { -1.0f, -1.0f, 1.0f},
        {1.0f, -1.0f, 1.0f}, {1.0f, 1.0f, -1.0f}, { -1.0f, 1.0f, -1.0f},
        { -1.0f, -1.0f, -1.0f}, {1.0f, -1.0f, -1.0f}
    };

    int cubeIData[12][3] =
    {
        {1, 2, 3}, {1, 3, 0}, {0, 3, 7}, {0, 7, 4}, {3, 2, 6},
        {3, 6, 7}, {1, 6, 2}, {1, 5, 6}, {0, 5, 1}, {0, 4, 5},
        {6, 5, 4}, {6, 4, 7}
    };

    TriMesh box(float xDim, float yDim, float zDim, const Vec4f& color)
    {
        std::vector<TriMesh::TriMeshVertex> V(8);
        std::vector<UINT> I(12*3);

        // Vertices
        for (int i = 0; i < 8; i++)
        {
            V[i].position = Vec3f(cubeVData[i][0], cubeVData[i][1], cubeVData[i][2]);
            V[i].normal = Vec3f(1.0f, 0.0f, 0.0f);  // TODO(ms): write and call generateNormals() function
            V[i].attributeA = color;
        }

        // Triangles
        for (int i = 0; i < 12; i++)
        {
            I[i * 3 + 0] = cubeIData[i][0];
            I[i * 3 + 1] = cubeIData[i][1];
            I[i * 3 + 2] = cubeIData[i][2];
        }

        TriMesh mesh(V, I);
        mesh.stretch(Vec3f(0.5f * xDim, 0.5f * yDim, 0.5f * zDim));

        return mesh;
    }

    TriMesh cylinder(float radius, float height, UINT stacks, UINT slices, const Vec4f& color)
    {
        std::vector<TriMesh::TriMeshVertex> vertices((stacks + 1) * slices);
        std::vector<UINT> indices(stacks * slices * 6);
        
        UINT vIndex = 0;
        for(UINT i = 0; i <= stacks; i++)
            for(UINT i2 = 0; i2 < slices; i2++)
            {
                auto &vtx = vertices[vIndex++];
                float theta = float(i2) * 2.0f * math::PIf / float(slices);
                vtx.position = Vec3f(radius * cosf(theta), radius * sinf(theta), height * float(i) / float(stacks));
                vtx.normal = Vec3f(1.0f, 0.0f, 0.0f);  // TODO(ms): write and call generateNormals() function
                vtx.attributeA = color;
            }

        UINT iIndex = 0;
        for(UINT i = 0; i < stacks; i++)
        {
            for(UINT i2 = 0; i2 < slices; i2++)
            {
                int i2p1 = (i2 + 1) % slices;

                indices[iIndex++] = (i + 1) * slices + i2;
                indices[iIndex++] = i * slices + i2;
                indices[iIndex++] = i * slices + i2p1;


                indices[iIndex++] = (i + 1) * slices + i2;
                indices[iIndex++] = i * slices + i2p1;
                indices[iIndex++] = (i + 1) * slices + i2p1;
            }
        }

        return TriMesh(vertices, indices);
    }

    TriMesh cylinder(const Vec3f &p0, const Vec3f &p1, float radius, UINT stacks, UINT slices, const Vec4f& color)
    {
        float height = (p1 - p0).length();

        TriMesh result = shapes::cylinder(radius, height, stacks, slices, color);
        result.applyTransform(Mat4f::translation(p0) * Mat4f::face(Vec3f::eZ, p1 - p0));
        return result;
    }
}  // namespace shapes

}  // namespace ml