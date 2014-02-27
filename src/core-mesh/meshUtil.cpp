
TriMesh MeshUtil::createPointCloudTemplate(const TriMesh& templateMesh, const Vector<vec3f>& points)
{
    const UINT64 pointCount = points.size();
    const UINT64 tVertices = templateMesh.vertices().size();
    const UINT64 tIndices = templateMesh.indices().size();

    Vector<MeshVertex> vertices(pointCount * tVertices);
    Vector<UINT> indices(pointCount * tIndices);

    for (UINT pointIndex = 0; pointIndex < points.size(); pointIndex++)
    {
        const vec3f& p = points[pointIndex];
        const UINT64 baseVertexIndex = pointIndex * tVertices;

        for (UINT vertexIndex = 0; vertexIndex < tVertices; vertexIndex++)
        {
            MeshVertex& v = vertices[baseVertexIndex + vertexIndex];
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
