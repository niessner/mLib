
TriMesh MeshUtil::createPointCloudTemplate(const TriMesh& templateMesh, const Vector<vec3f>& points)
{
    const UINT64 pointCount = points.size();
    const UINT64 tVertices = templateMesh.vertices().size();
    const UINT64 tIndices = templateMesh.indices().size();

    Vector<TriMesh::TriMeshVertex> vertices(pointCount * tVertices);
    Vector<UINT> indices(pointCount * tIndices);

    for (UINT pointIndex = 0; pointIndex < points.size(); pointIndex++)
    {
        const vec3f& p = points[pointIndex];
        const UINT64 baseVertexIndex = pointIndex * tVertices;

        for (UINT vertexIndex = 0; vertexIndex < tVertices; vertexIndex++)
        {
            TriMesh::TriMeshVertex& v = vertices[baseVertexIndex + vertexIndex];
            v = templateMesh.vertices()[vertexIndex];
            v.position += p;
        }
        for (UINT indexIndex = 0; indexIndex < tIndices; indexIndex++)
        {
            indices[pointIndex * tIndices + indexIndex] = templateMesh.indices()[indexIndex] + (pointIndex * (UINT)tVertices);
        }
    }

    return TriMesh(vertices, indices);
}

TriMesh MeshUtil::createPointCloudTemplate(const TriMesh& templateMesh,
                                           const std::vector<vec3f>& points,
                                           const std::vector<vec4f>& colors)
{
    MLIB_ASSERT_STR(points.size() == colors.size(), "Mismatch in size of point and color vectors");
    
    const UINT64 pointCount = points.size();
    const UINT64 tVertices = templateMesh.vertices().size();
    const UINT64 tIndices = templateMesh.indices().size();

    Vector<TriMesh::TriMeshVertex> vertices(pointCount * tVertices);
    Vector<UINT> indices(pointCount * tIndices);

    for (UINT pointIndex = 0; pointIndex < points.size(); pointIndex++)
    {
        const vec3f& p = points[pointIndex];
        const vec4f& c = colors[pointIndex];
        const UINT64 baseVertexIndex = pointIndex * tVertices;

        for (UINT vertexIndex = 0; vertexIndex < tVertices; vertexIndex++)
        {
            TriMesh::TriMeshVertex& v = vertices[baseVertexIndex + vertexIndex];
            v = templateMesh.vertices()[vertexIndex];
            v.position += p;
            v.attributeA = c;
        }
        for (UINT indexIndex = 0; indexIndex < tIndices; indexIndex++)
        {
            indices[pointIndex * tIndices + indexIndex] = templateMesh.indices()[indexIndex] + (pointIndex * (UINT)tVertices);
        }
    }

    return TriMesh(vertices, indices);
}