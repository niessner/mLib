
namespace MeshShapes
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

    TriMesh box(float xDim, float yDim, float zDim, const vec4f& color)
    {
        Vector<TriMesh::TriMeshVertex> V(8);
        Vector<UINT> I(12*3);

        // Vertices
        for (int i = 0; i < 8; i++)
        {
            V[i].position = vec3f(cubeVData[i][0], cubeVData[i][1], cubeVData[i][2]);
            V[i].normal = vec3f(1.0f, 0.0f, 0.0f);  // TODO(ms): write and call generateNormals() function
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
        mesh.stretch(vec3f(0.5f * xDim, 0.5f * yDim, 0.5f * zDim));

        return mesh;
    }

    TriMesh cylinder(float radius, float height, UINT stacks, UINT slices, const vec4f& color)
    {
        Vector<TriMesh::TriMeshVertex> V(8);
        Vector<UINT> I(12*3);
        
        UINT vIndex = 0;
        for(UINT i = 0; i <= stacks; i++)
            for(UINT i2 = 0; i2 < slices; i2++)
            {
                auto &vtx = V[vIndex++];
                float theta = float(i2) * 2.0f * Math::PIf / float(slices);
                vtx.position = vec3f(radius * cosf(theta), radius * sinf(theta), height * float(i) / float(stacks));
                vtx.normal = vec3f(1.0f, 0.0f, 0.0f);  // TODO(ms): write and call generateNormals() function
                vtx.attributeA = color;
            }

        UINT iIndex = 0;
        for(UINT i = 0; i < stacks; i++)
        {
            for(UINT i2 = 0; i2 < slices; i2++)
            {
                int i2p1 = (i2 + 1) % slices;

                I[iIndex++] = (i + 1) * slices + i2;
                I[iIndex++] = i * slices + i2;
                I[iIndex++] = i * slices + i2p1;


                I[iIndex++] = (i + 1) * slices + i2;
                I[iIndex++] = i * slices + i2p1;
                I[iIndex++] = (i + 1) * slices + i2p1;
            }
        }

        return TriMesh(V, I);
    }

}  // namespace MeshShapes
