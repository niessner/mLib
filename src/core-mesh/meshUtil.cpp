
TriMesh MeshUtil::createUnifiedMesh(const Vector< std::pair<TriMesh, mat4f> > &meshes)
{
    const UINT totalPoints = meshes.sum([](const std::pair<TriMesh, mat4f> &t) { return (UINT)t.first.vertices().size(); });
    const UINT totalIndices = meshes.sum([](const std::pair<TriMesh, mat4f> &t) { return (UINT)t.first.indices().size(); });
    
    Vector<TriMesh::TriMeshVertex> vertices(totalPoints);
    Vector<UINT> indices(totalIndices);

    UINT vIndex = 0, iIndex = 0;
    for (const auto &m : meshes)
    {
        const UINT baseVertexIndex = vIndex;

        for (UINT vertexIndex = 0; vertexIndex < m.first.vertices().size(); vertexIndex++)
        {
            TriMesh::TriMeshVertex& v = vertices[vIndex++];
            v = m.first.vertices()[vertexIndex];
            v.position =  m.second * v.position;
        }
        for (UINT indexIndex = 0; indexIndex < m.first.indices().size(); indexIndex++)
        {
            indices[iIndex++] = m.first.indices()[indexIndex] + baseVertexIndex;
        }
    }

    return TriMesh(vertices, indices);
}

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
        const UINT baseVertexIndex = pointIndex * (UINT)tVertices;

        for (UINT vertexIndex = 0; vertexIndex < tVertices; vertexIndex++)
        {
            TriMesh::TriMeshVertex& v = vertices[baseVertexIndex + vertexIndex];
            v = templateMesh.vertices()[vertexIndex];
            v.position += p;
        }
        for (UINT indexIndex = 0; indexIndex < tIndices; indexIndex++)
        {
            indices[pointIndex * tIndices + indexIndex] = templateMesh.indices()[indexIndex] + baseVertexIndex;
        }
    }

    return TriMesh(vertices, indices);
}

TriMesh MeshUtil::createPointCloudTemplate(const TriMesh& templateMesh,
                                           const Vector<vec3f>& points,
                                           const Vector<vec4f>& colors)
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

TriMesh MeshUtil::createPointCloudTemplate(const TriMesh& templateMesh,
                                           const std::vector<vec3f>& points,
                                           const std::vector<vec4f>& colors)
{
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