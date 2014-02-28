#ifndef _MESH_DATA_H_
#define _MESH_DATA_H_


template <class FloatType>
struct MeshData {
	MeshData() {}
	MeshData(MeshData&& d) {
		m_Vertices = std::move(d.m_Vertices);
		m_Normals = std::move(d.m_Normals);
		m_TextureCoords = std::move(d.m_TextureCoords);
		m_Colors = std::move(d.m_Colors);
		m_FaceIndicesVertices = std::move(d.m_FaceIndicesVertices);
		m_FaceIndicesNormals = std::move(d.m_FaceIndicesNormals);
		m_FaceIndicesTextureCoords = std::move(d.m_FaceIndicesTextureCoords);
	}
	void operator=(MeshData&& d) {
		m_Vertices = std::move(d.m_Vertices);
		m_Normals = std::move(d.m_Normals);
		m_TextureCoords = std::move(d.m_TextureCoords);
		m_Colors = std::move(d.m_Colors);
		m_FaceIndicesVertices = std::move(d.m_FaceIndicesVertices);
		m_FaceIndicesNormals = std::move(d.m_FaceIndicesNormals);
		m_FaceIndicesTextureCoords = std::move(d.m_FaceIndicesTextureCoords);
	}
	void clear() {
		m_Vertices.clear();
		m_Normals.clear();
		m_Colors.clear();
		m_FaceIndicesVertices.clear();
		m_FaceIndicesNormals.clear();
		m_FaceIndicesTextureCoords.clear();
	}

	bool isConsistent() {
		bool consistent = true;
		if (m_FaceIndicesNormals.size() > 0			&& m_FaceIndicesVertices.size() != m_FaceIndicesNormals.size())			consistent = false;
		if (m_FaceIndicesTextureCoords.size() > 0	&& m_FaceIndicesVertices.size() != m_FaceIndicesTextureCoords.size())	consistent = false;
		return consistent;
	}

	void applyTransform(const mat4f& t) {
		for (size_t i = 0; i < m_Vertices.size(); i++) {
			m_Vertices[i] *= t;
		}
		mat4f invTrans = m.getInverse().getTranspose();
		for (size_t i = 0; i < m_Normals.size(); i++) {
			m_Normals[i] *= invTrans;
		}
	}

	BoundingBox3d<FloatType> getBoundingBox() {
		BoundingBox3d<FloatType> bb;
		for (size_t i = 0; i < m_Vertices.size(); i++) {
			bb.include(m_Vertices[i]);
		}
		return bb;
	}

	Vector<point3d<FloatType>>	m_Vertices;			//vertices are indexed (see below)
	Vector<point3d<FloatType>>	m_Normals;			//normals are indexed (see below)
	Vector<point2d<FloatType>>	m_TextureCoords;	//tex coords are indexed (see below)
	Vector<point3d<FloatType>>	m_Colors;			//colors are not indexed (exclusive per vertex) 
	Vector<Vector<unsigned int>>	m_FaceIndicesVertices;		//indices in face array
	Vector<Vector<unsigned int>>	m_FaceIndicesNormals;		//indices in normal array
	Vector<Vector<unsigned int>>	m_FaceIndicesTextureCoords;	//indices in texture array
};

typedef MeshData<float>		MeshDataf;
typedef MeshData<double>	MeshDatad;

#endif
