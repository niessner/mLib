
#ifndef CORE_MESH_TRIMESH_INL_H_
#define CORE_MESH_TRIMESH_INL_H_

namespace ml {
	
	template<class FloatType>
	TriMesh<FloatType>::TriMesh( const MeshData<FloatType>& meshData )
	{

		m_Vertices.resize(meshData.m_Vertices.size());

		m_bHasNormals = meshData.m_Normals.size() > 0;
		m_bHasColors = meshData.m_Colors.size() > 0;
		m_bHasTexCoords = meshData.m_TextureCoords.size() > 0;

		for (size_t i = 0; i < m_Vertices.size(); i++) {
			m_Vertices[i].position = meshData.m_Vertices[i];
		}

		for (size_t i = 0; i < meshData.m_FaceIndicesVertices.size(); i++) {
			if (meshData.m_FaceIndicesVertices[i].size() == 3) {
				//we need to split vertices if the same vertex has different texcoords and/or normals
				if (m_bHasNormals || m_bHasTexCoords || m_bHasColors) {
					vec3ui coords = vec3ui(0,0,0);
					for (unsigned int j = 0; j < 3; j++) {
						bool vertexSplit = false;
						if (m_bHasNormals) { //split if normal is different than the one found before
							const point3d<FloatType>& n = meshData.m_Normals[meshData.getFaceIndicesNormals()[i][j]];
							if (m_Vertices[meshData.getFaceIndicesVertices()[i][j]].normal != point3d<FloatType>::origin && 
								m_Vertices[meshData.getFaceIndicesVertices()[i][j]].normal != n)	vertexSplit = true;
						}
						if (m_bHasTexCoords) { //split if texcoord is different than the one found before
							const point2d<FloatType>& t = meshData.m_TextureCoords[meshData.getFaceIndicesTexCoords()[i][j]];
							if (m_Vertices[meshData.getFaceIndicesVertices()[i][j]].texCoord != point2d<FloatType>::origin && 
								m_Vertices[meshData.getFaceIndicesVertices()[i][j]].texCoord != t) vertexSplit = true;
						}
						if (m_bHasColors) { //split if texcoord is different than the one found before
							const point4d<FloatType>& c = meshData.m_Colors[meshData.getFaceIndicesColors()[i][j]];
							if (m_Vertices[meshData.getFaceIndicesVertices()[i][j]].color != point4d<FloatType>::origin && 
								m_Vertices[meshData.getFaceIndicesVertices()[i][j]].color != c) vertexSplit = true;
						}

						if (vertexSplit) {
							MLIB_WARNING("vertex split untested");
							Vertex<FloatType> v = m_Vertices[meshData.getFaceIndicesVertices()[i][j]];
							if (m_bHasNormals)		v.normal = meshData.m_Normals[meshData.getFaceIndicesNormals()[i][j]];
							if (m_bHasTexCoords)	v.texCoord = meshData.m_TextureCoords[meshData.getFaceIndicesTexCoords()[i][j]];
							if (m_bHasColors)		v.color = meshData.m_Colors[meshData.getFaceIndicesColors()[i][j]];
							m_Vertices.push_back(v);
							coords[j] = (unsigned int)m_Vertices.size() - 1;
						} else {
							if (m_bHasNormals)		m_Vertices[meshData.getFaceIndicesVertices()[i][j]].normal = meshData.m_Normals[meshData.getFaceIndicesNormals()[i][j]];
							if (m_bHasTexCoords)	m_Vertices[meshData.getFaceIndicesVertices()[i][j]].texCoord = meshData.m_TextureCoords[meshData.getFaceIndicesTexCoords()[i][j]];
							if (m_bHasColors)		m_Vertices[meshData.getFaceIndicesVertices()[i][j]].color = meshData.m_Colors[meshData.getFaceIndicesColors()[i][j]];
							coords[j] = meshData.getFaceIndicesVertices()[i][j];
						}
					}
					m_Indices.push_back(coords);

					//m_Indices.push_back(vec3ui(meshData.m_FaceIndicesVertices[i][0], meshData.m_FaceIndicesVertices[i][1], meshData.m_FaceIndicesVertices[i][2]));
					//if (hasNormals) {
					//	//we are ignoring the fact that sometimes there might be vertex split required (if a vertex has two different normals)
					//	m_Vertices[m_Indices[i][0]].normal = meshData.m_Normals[meshData.m_FaceIndicesNormals[i][0]];
					//	m_Vertices[m_Indices[i][1]].normal = meshData.m_Normals[meshData.m_FaceIndicesNormals[i][1]];
					//	m_Vertices[m_Indices[i][2]].normal = meshData.m_Normals[meshData.m_FaceIndicesNormals[i][2]];
					//}
					//if (hasTexCoords) {
					//	//we are ignoring the fact that sometimes there might be vertex split required (if a vertex has two different normals)
					//	m_Vertices[m_Indices[i][0]].texCoord = meshData.m_TextureCoords[meshData.m_FaceIndicesTextureCoords[i][0]];
					//	m_Vertices[m_Indices[i][1]].texCoord = meshData.m_TextureCoords[meshData.m_FaceIndicesTextureCoords[i][1]];
					//	m_Vertices[m_Indices[i][2]].texCoord = meshData.m_TextureCoords[meshData.m_FaceIndicesTextureCoords[i][2]];
					//}
				} else {
					m_Indices.push_back(vec3ui(meshData.m_FaceIndicesVertices[i][0], meshData.m_FaceIndicesVertices[i][1], meshData.m_FaceIndicesVertices[i][2]));
				}
			} else {
				MLIB_WARNING("non triangle face found - ignoring it");
			}
		}
	}


	template<class FloatType>
	void TriMesh<FloatType>::computeNormals()
	{
		for (int i = 0; i < (int)m_Vertices.size(); i++) {
			m_Vertices[i].normal = point3d<FloatType>::origin;
		}

		for (int i = 0; i < (int)m_Indices.size(); i++) {
			point3d<FloatType> faceNormal = 
				(m_Vertices[m_Indices[i].y].position - m_Vertices[m_Indices[i].x].position)^(m_Vertices[m_Indices[i].z].position - m_Vertices[m_Indices[i].x].position);

			m_Vertices[m_Indices[i].x].normal += faceNormal;
			m_Vertices[m_Indices[i].y].normal += faceNormal;
			m_Vertices[m_Indices[i].z].normal += faceNormal;
		}
		for (int i = 0; i < (int)m_Vertices.size(); i++) {
			m_Vertices[i].normal.normalize();
		}

		m_bHasNormals = true;
	}


}

#endif // CORE_MESH_TRIMESH_INL_H_