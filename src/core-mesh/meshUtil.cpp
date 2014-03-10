
namespace ml
{

TriMesh MeshUtil::createUnifiedMesh(const std::vector< std::pair<TriMesh, Mat4f> > &meshes)
{
    //const UINT totalPoints = meshes.sum([](const std::pair<TriMesh, Mat4f> &t) { return (UINT)t.first.vertices().size(); });
    //const UINT totalIndices = meshes.sum([](const std::pair<TriMesh, Mat4f> &t) { return (UINT)t.first.indices().size(); });
	auto lambdaVertices = [=](size_t total, const std::pair<TriMesh, Mat4f> &t) { return t.first.vertices().size() + total; };
	const size_t totalPoints = std::accumulate(meshes.begin(), meshes.end(), static_cast<size_t>(0), lambdaVertices);
	auto lambdaIndices = [=](size_t total, const std::pair<TriMesh, Mat4f> &t) { return t.first.indices().size() + total; };
	const size_t totalIndices = std::accumulate(meshes.begin(), meshes.end(), static_cast<size_t>(0), lambdaIndices);
    
    std::vector<TriMesh::TriMeshVertex> vertices(totalPoints);
    std::vector<UINT> indices(totalIndices);

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

TriMesh MeshUtil::createPointCloudTemplate(const TriMesh& templateMesh, const std::vector<Vec3f>& points)
{
    const UINT64 pointCount = points.size();
    const UINT64 tVertices = templateMesh.vertices().size();
    const UINT64 tIndices = templateMesh.indices().size();

    std::vector<TriMesh::TriMeshVertex> vertices(pointCount * tVertices);
    std::vector<UINT> indices(pointCount * tIndices);

    for (UINT pointIndex = 0; pointIndex < points.size(); pointIndex++)
    {
        const Vec3f& p = points[pointIndex];
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
                                           const std::vector<Vec3f>& points,
                                           const std::vector<Vec4f>& colors)
{
    const UINT64 pointCount = points.size();
    const UINT64 tVertices = templateMesh.vertices().size();
    const UINT64 tIndices = templateMesh.indices().size();

    std::vector<TriMesh::TriMeshVertex> vertices(pointCount * tVertices);
    std::vector<UINT> indices(pointCount * tIndices);

    for (UINT pointIndex = 0; pointIndex < points.size(); pointIndex++)
    {
        const Vec3f& p = points[pointIndex];
        const Vec4f& c = colors[pointIndex];
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

}
