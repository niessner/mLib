
namespace ml {

TriMeshOld meshutil::createUnifiedMesh(const std::vector< std::pair<TriMeshOld, mat4f> >& meshes) {
  auto lambdaVertices = [ = ](size_t total, const std::pair<TriMeshOld, mat4f>& t) { return t.first.vertices().size() + total; };
  const size_t totalPoints = std::accumulate(meshes.begin(), meshes.end(), static_cast<size_t>(0), lambdaVertices);
  auto lambdaIndices = [ = ](size_t total, const std::pair<TriMeshOld, mat4f>& t) { return t.first.indices().size() + total; };
  const size_t totalIndices = std::accumulate(meshes.begin(), meshes.end(), static_cast<size_t>(0), lambdaIndices);

  std::vector<TriMeshOld::TriMeshOldVertex> vertices(totalPoints);
  std::vector<UINT> indices(totalIndices);

  UINT vIndex = 0, iIndex = 0;
  for (const auto& m : meshes) {
    const UINT baseVertexIndex = vIndex;

    for (UINT vertexIndex = 0; vertexIndex < m.first.vertices().size(); vertexIndex++) {
      TriMeshOld::TriMeshOldVertex& v = vertices[vIndex++];
      v = m.first.vertices()[vertexIndex];
      v.position =  m.second * v.position;
    }
    for (UINT indexIndex = 0; indexIndex < m.first.indices().size(); indexIndex++) {
      indices[iIndex++] = m.first.indices()[indexIndex] + baseVertexIndex;
    }
  }

  return TriMeshOld(vertices, indices);
}

TriMeshOld meshutil::createUnifiedMesh(const std::vector<TriMeshOld>& meshes) {
  auto lambdaVertices = [=](size_t total, const TriMeshOld& t) { return t.vertices().size() + total; };
  const size_t totalPoints = std::accumulate(meshes.begin(), meshes.end(), static_cast<size_t>(0), lambdaVertices);
  auto lambdaIndices = [=](size_t total, const TriMeshOld& t) { return t.indices().size() + total; };
  const size_t totalIndices = std::accumulate(meshes.begin(), meshes.end(), static_cast<size_t>(0), lambdaIndices);

  std::vector<TriMeshOld::TriMeshOldVertex> vertices(totalPoints);
  std::vector<UINT> indices(totalIndices);

  UINT vIndex = 0, iIndex = 0;
  for (const auto& m : meshes) {
    const UINT baseVertexIndex = vIndex;

    for (UINT vertexIndex = 0; vertexIndex < m.vertices().size(); vertexIndex++) {
      TriMeshOld::TriMeshOldVertex& v = vertices[vIndex++];
      v = m.vertices()[vertexIndex];
    }
    for (UINT indexIndex = 0; indexIndex < m.indices().size(); indexIndex++) {
      indices[iIndex++] = m.indices()[indexIndex] + baseVertexIndex;
    }
  }

  return TriMeshOld(vertices, indices);
}

TriMeshOld meshutil::createPointCloudTemplate(const TriMeshOld& templateMesh,
                                           const std::vector<vec3f>& points,
                                           const std::vector<vec4f>& colors) {
  const UINT64 pointCount = points.size();
  const UINT64 tVertices = templateMesh.vertices().size();
  const UINT64 tIndices = templateMesh.indices().size();
  const vec4f defaultColor(1.f, 0.f, 0.f, 1.f);

  std::vector<TriMeshOld::TriMeshOldVertex> vertices(pointCount * tVertices);
  std::vector<UINT> indices(pointCount * tIndices);

  for (UINT pointIndex = 0; pointIndex < points.size(); pointIndex++) {
    const vec3f& p = points[pointIndex];
    const vec4f& c = colors.empty() ? defaultColor : colors[pointIndex];
    const UINT64 baseVertexIndex = pointIndex * tVertices;

    for (UINT vertexIndex = 0; vertexIndex < tVertices; vertexIndex++) {
      TriMeshOld::TriMeshOldVertex& v = vertices[baseVertexIndex + vertexIndex];
      v = templateMesh.vertices()[vertexIndex];
      v.position += p;
      v.attributeA = c;
    }
    for (UINT indexIndex = 0; indexIndex < tIndices; indexIndex++) {
      indices[pointIndex * tIndices + indexIndex] = templateMesh.indices()[indexIndex] + (pointIndex * (UINT)tVertices);
    }
  }

  return TriMeshOld(vertices, indices);
}

}  // namespace ml
