#ifndef CORE_MESH_MESHDATA_H_
#define CORE_MESH_MESHDATA_H_

namespace ml {

//! raw mesh data could be also a point cloud
template <class FloatType>
class MeshData {
public:
	MeshData() {}
	MeshData(MeshData&& d) {
		m_Vertices = std::move(d.m_Vertices);
		m_Normals = std::move(d.m_Normals);
		m_TextureCoords = std::move(d.m_TextureCoords);
		m_Colors = std::move(d.m_Colors);
		m_FaceIndicesVertices = std::move(d.m_FaceIndicesVertices);
		m_FaceIndicesNormals = std::move(d.m_FaceIndicesNormals);
		m_FaceIndicesTextureCoords = std::move(d.m_FaceIndicesTextureCoords);
		m_FaceIndicesColors = std::move(d.m_FaceIndicesColors);
	}
	void operator=(MeshData&& d) {
		m_Vertices = std::move(d.m_Vertices);
		m_Normals = std::move(d.m_Normals);
		m_TextureCoords = std::move(d.m_TextureCoords);
		m_Colors = std::move(d.m_Colors);
		m_FaceIndicesVertices = std::move(d.m_FaceIndicesVertices);
		m_FaceIndicesNormals = std::move(d.m_FaceIndicesNormals);
		m_FaceIndicesTextureCoords = std::move(d.m_FaceIndicesTextureCoords);
		m_FaceIndicesColors = std::move(d.m_FaceIndicesColors);
	}
	void clear() {
		m_Vertices.clear();
		m_Normals.clear();
		m_Colors.clear();
		m_FaceIndicesVertices.clear();
		m_FaceIndicesNormals.clear();
		m_FaceIndicesTextureCoords.clear();
		m_FaceIndicesColors.clear();
	}

	bool isConsistent() const {
		//todo more elaborate, but cheap checks
		bool consistent = true;
		if (m_FaceIndicesNormals.size() > 0			&& m_FaceIndicesVertices.size() != m_FaceIndicesNormals.size())			consistent = false;
		if (m_FaceIndicesTextureCoords.size() > 0	&& m_FaceIndicesVertices.size() != m_FaceIndicesTextureCoords.size())	consistent = false;
		if (m_FaceIndicesColors.size() > 0			&& m_FaceIndicesColors.size() != m_FaceIndicesColors.size())			consistent = false;
		return consistent;
	}

	void applyTransform(const mat4f& t) {
		for (size_t i = 0; i < m_Vertices.size(); i++) {
			m_Vertices[i] = t*m_Vertices[i];
		}
		mat4f invTrans = t.getInverse().getTranspose();
		for (size_t i = 0; i < m_Normals.size(); i++) {
			m_Normals[i] = invTrans*m_Normals[i];
		}
	}

	BoundingBox3d<FloatType> getBoundingBox() const {
		BoundingBox3d<FloatType> bb;
		for (size_t i = 0; i < m_Vertices.size(); i++) {
			bb.include(m_Vertices[i]);
		}
		return bb;
	}

	const std::vector<std::vector<unsigned int>>& getFaceIndicesVertices() const {
		return m_FaceIndicesVertices;
	}
	const std::vector<std::vector<unsigned int>>& getFaceIndicesNormals() const {
		if (!hasNormals())	throw MLIB_EXCEPTION("mesh does not have normals");
		else if (m_FaceIndicesNormals.size() > 0)			return m_FaceIndicesNormals;
		else if (m_Vertices.size() == m_Normals.size())		return m_FaceIndicesVertices;
		else throw MLIB_EXCEPTION("vertex/normal mismatch");
	}
	const std::vector<std::vector<unsigned int>>& getFaceIndicesTexCoords() const {
		if (!hasTexCoords())	throw MLIB_EXCEPTION("mesh does not have texcoords");
		else if (m_FaceIndicesTextureCoords.size() > 0)			return m_FaceIndicesTextureCoords;
		else if (m_Vertices.size() == m_TextureCoords.size())	return m_FaceIndicesVertices;
		else throw MLIB_EXCEPTION("vertex/texcoord mismatch");
	}
	const std::vector<std::vector<unsigned int>>& getFaceIndicesColors() const {
		if (!hasColors())	throw MLIB_EXCEPTION("mesh does not have colors");
		else if (m_FaceIndicesColors.size() > 0)		return m_FaceIndicesColors;
		else if (m_Vertices.size() == m_Colors.size())	return m_FaceIndicesVertices;
		else throw MLIB_EXCEPTION("vertex/color mismatch");
	}

	bool hasNormals() const { return m_Normals.size() > 0; }
	bool hasColors() const { return m_Colors.size() > 0; }
	bool hasTexCoords() const { return m_TextureCoords.size() > 0; }

	bool hasPerVertexNormals()	const	{ return hasNormals() && m_FaceIndicesNormals.size() == 0; }
	bool hasPerVertexColors()	const	{ return hasColors() && m_FaceIndicesColors.size() == 0; }
	bool hasPerVertexTexCoords() const	{ return hasTexCoords() && m_FaceIndicesTextureCoords.size() == 0; }

	unsigned int removeDuplicateVertices();
	unsigned int removeDuplicateFaces();
	unsigned int mergeCloseVertices(FloatType thresh, bool approx = false);
	unsigned int removeDegeneratedFaces() {
		std::vector<std::vector<unsigned int>> newFacesIndicesVertices;

		for (size_t i = 0; i < m_FaceIndicesVertices.size(); i++) {
			std::unordered_set<unsigned int> _set(m_FaceIndicesVertices[i].size());
			bool foundDuplicate = false;
			for (unsigned int idx : m_FaceIndicesVertices[i]) {
				if (_set.find(idx) != _set.end()) {
					foundDuplicate = true;
					break;
				} else {
					_set.insert(idx);
				}
			}
			if (!foundDuplicate) {
				newFacesIndicesVertices.push_back(m_FaceIndicesVertices[i]);
			}
		}
		m_FaceIndicesVertices = newFacesIndicesVertices;
		return (unsigned int)m_FaceIndicesVertices.size();
	}

	std::vector<point3d<FloatType>>	m_Vertices;			//vertices are indexed (see below)
	std::vector<point3d<FloatType>>	m_Normals;			//normals are indexed (see below/or per vertex)
	std::vector<point2d<FloatType>>	m_TextureCoords;	//tex coords are indexed (see below/or per vertex)
	std::vector<point4d<FloatType>>	m_Colors;			//colors are not indexed (see below/or per vertex) 
	std::vector<std::vector<unsigned int>>	m_FaceIndicesVertices;		//indices in face array
	std::vector<std::vector<unsigned int>>	m_FaceIndicesNormals;		//indices in normal array (if size==0, indicesVertices is used)
	std::vector<std::vector<unsigned int>>	m_FaceIndicesTextureCoords;	//indices in texture array (if size==0, indicesVertices is used)
	std::vector<std::vector<unsigned int>>	m_FaceIndicesColors;		//indices in color array (if size==0, indicesVertices is used)

private:
	inline vec3i toVirtualVoxelPos(const point3d<FloatType>& v, FloatType voxelSize) {
		return vec3i(v/voxelSize+(FloatType)0.5*point3d<FloatType>(math::sign(v)));
	} 
	//! returns -1 if there is no vertex closer to 'v' than thresh; otherwise the vertex id of the closer vertex is returned
	unsigned int hasNearestNeighbor(const vec3i& coord, SparseGrid3D<std::list<std::pair<point3d<FloatType>,unsigned int>>> &neighborQuery, const point3d<FloatType>& v, FloatType thresh );

	//! returns -1 if there is no vertex closer to 'v' than thresh; otherwise the vertex id of the closer vertex is returned (manhattan distance)
	unsigned int hasNearestNeighborApprox(const vec3i& coord, SparseGrid3D<unsigned int> &neighborQuery, FloatType thresh );
};

typedef MeshData<float>		MeshDataf;
typedef MeshData<double>	MeshDatad;




static inline bool FaceLess(const std::vector<unsigned int>& t0_, const std::vector<unsigned int>& t1_)
{
	if (t0_.size() != t1_.size())	return t0_.size() < t1_.size();
	else {
		std::vector<unsigned int> t0 = t0_;
		std::vector<unsigned int> t1 = t1_;

		std::sort(t0.begin(), t0.end());
		std::sort(t1.begin(), t1.end());
		for (size_t i = 0; i < t0.size(); i++) {
			if (t0[i] == t1[i]) continue;
			return t0[i] < t1[i];
		}
	}
	return false;
}

template <class FloatType>
unsigned int MeshData<FloatType>::removeDuplicateFaces()
{
	
	unsigned int numT = (unsigned int)m_FaceIndicesVertices.size();
 
	std::map<std::vector<unsigned int>, unsigned int, bool(*)(const std::vector<unsigned int>&, const std::vector<unsigned int>&)> pts(FaceLess);
	
	std::vector<std::vector<unsigned int>> faces_new; faces_new.reserve(numT);

	unsigned int cnt = 0;
	for (unsigned int i1 = 0; i1 < numT; i1++)	{

		const std::vector<unsigned int>& face = m_FaceIndicesVertices[i1];
		std::map<std::vector<unsigned int>, unsigned int, bool(*)(const std::vector<unsigned int>&, const std::vector<unsigned int>&)>::iterator it = pts.find(face);

		if (it == pts.end()) {
			pts.insert(std::make_pair(face, cnt));
			faces_new.push_back(face);
			cnt++;
		}
	}

	m_FaceIndicesVertices = std::vector<std::vector<unsigned int>>(faces_new.begin(), faces_new.end());

	std::cout << "Removed " << numT-cnt << " duplicate faces of " << numT << " faces" << std::endl;

	return cnt;
}




template <class FloatType>
static inline bool VertexLess(const point3d<FloatType>& v0, const point3d<FloatType>& v1)
{
	if (v0[0] < v1[0]) return true;
	if (v0[0] > v1[0]) return false;
	if (v0[1] < v1[1]) return true;
	if (v0[1] > v1[1]) return false;
	if (v0[2] < v1[2]) return true;

	return false;
}


template <class FloatType>
unsigned int MeshData<FloatType>::removeDuplicateVertices() {
	unsigned int numV = (unsigned int)m_Vertices.size();
	//int numT = (int)tris.size();

	std::map<point3d<FloatType>, unsigned int, bool(*)(const point3d<FloatType>&, const point3d<FloatType>&)> pts(VertexLess);

	std::vector<unsigned int> vertexLookUp;	vertexLookUp.resize(numV);
	std::vector<point3d<FloatType>> new_verts; new_verts.reserve(numV);
	std::vector<point4d<FloatType>> new_color;		if (hasPerVertexColors())		new_color.reserve(m_Colors.size());
	std::vector<point3d<FloatType>> new_normals;	if (hasPerVertexNormals())		new_normals.reserve(m_Normals.size());
	std::vector<point2d<FloatType>> new_tex;		if (hasPerVertexTexCoords())	new_tex.reserve(m_TextureCoords.size());

	unsigned int cnt = 0;
	for (size_t i1 = 0; i1 < numV; i1++) {
		const point3d<FloatType>& pt = m_Vertices[i1];

		std::map<point3d<FloatType>, unsigned int, bool(*)(const point3d<FloatType>&, const point3d<FloatType>&)>::iterator it = pts.find(pt);

		if (it != pts.end()) {
			vertexLookUp[i1] = it->second;
		} else {
			pts.insert(std::make_pair(pt, cnt));
			new_verts.push_back(pt);
			vertexLookUp[i1] = cnt;
			cnt++;
			if (hasPerVertexColors())		new_color.push_back(m_Colors[i1]);
			if (hasPerVertexNormals())		new_normals.push_back(m_Normals[i1]);
			if (hasPerVertexTexCoords())	new_tex.push_back(m_TextureCoords[i1]);
		}
	}

	// Update faces
	for (std::vector<std::vector<unsigned int>>::iterator it = m_FaceIndicesVertices.begin(); it != m_FaceIndicesVertices.end(); it++) {
		for (std::vector<unsigned int>::iterator idx = it->begin(); idx != it->end(); idx++) {
			*idx = vertexLookUp[*idx];
		}
		//*it = vertexLookUp[*it];
	}

	std::cout << "Removed " << numV-cnt << " duplicate vertices of " << numV << " vertices" << std::endl;
	m_Vertices = std::vector<point3d<FloatType>>(new_verts.begin(), new_verts.end());
	if (hasPerVertexColors())		m_Colors = std::vector<point4d<FloatType>>(new_color.begin(), new_color.end());
	if (hasPerVertexNormals())		m_Normals = std::vector<point3d<FloatType>>(new_normals.begin(), new_normals.end());
	if (hasPerVertexTexCoords())	m_TextureCoords = std::vector<point2d<FloatType>>(new_tex.begin(), new_tex.end());

	return cnt;
}



template <class FloatType>
unsigned int MeshData<FloatType>::hasNearestNeighbor( const vec3i& coord, SparseGrid3D<std::list<std::pair<point3d<FloatType>,unsigned int>>> &neighborQuery, const point3d<FloatType>& v, FloatType thresh )
{
	FloatType threshSq = thresh*thresh;
	for (int i = -1; i <= 1; i++) {
		for (int j = -1; j <= 1; j++) {
			for (int k = -1; k <= 1; k++) {
				vec3i c = coord + vec3i(i,j,k);
				if (neighborQuery.exists(c)) {
					for (const std::pair<point3d<FloatType>, unsigned int>& n : neighborQuery[c]) {
						if (point3d<FloatType>::distSq(v,n.first) < threshSq) {
							return n.second;
						}
					}
				}
			}
		}
	}
	return (unsigned int)-1;
}

template <class FloatType>
unsigned int MeshData<FloatType>::hasNearestNeighborApprox(const vec3i& coord, SparseGrid3D<unsigned int> &neighborQuery, FloatType thresh ) {
	FloatType threshSq = thresh*thresh;

	for (int i = -1; i <= 1; i++) {
		for (int j = -1; j <= 1; j++) {
			for (int k = -1; k <= 1; k++) {
				vec3i c = coord + vec3i(i,j,k);
				if (neighborQuery.exists(c)) {
					return neighborQuery[c];
				}
			}
		}
	}
	return (unsigned int)-1;
}



template <class FloatType>
unsigned int MeshData<FloatType>::mergeCloseVertices(FloatType thresh, bool approx)
{
	if (thresh <= (FloatType)0)	throw MLIB_EXCEPTION("invalid thresh " + std::to_string(thresh));	
	unsigned int numV = (unsigned int)m_Vertices.size();

	std::vector<unsigned int> vertexLookUp;	vertexLookUp.resize(numV);
	std::vector<point3d<FloatType>> new_verts; new_verts.reserve(numV);
	std::vector<point4d<FloatType>> new_color;		if (hasPerVertexColors())		new_color.reserve(m_Colors.size());
	std::vector<point3d<FloatType>> new_normals;	if (hasPerVertexNormals())		new_normals.reserve(m_Normals.size());
	std::vector<point2d<FloatType>> new_tex;		if (hasPerVertexTexCoords())	new_tex.reserve(m_TextureCoords.size());

	unsigned int cnt = 0;
	if (approx) {
		SparseGrid3D<unsigned int> neighborQuery(0.6f, numV*2);
		for (unsigned int v = 0; v < numV; v++) {

			const point3d<FloatType>& vert = m_Vertices[v];
			vec3i coord = toVirtualVoxelPos(vert, thresh);		
			unsigned int nn = hasNearestNeighborApprox(coord, neighborQuery, thresh);

			if (nn == (unsigned int)-1) {
				neighborQuery[coord] = cnt;
				new_verts.push_back(vert);
				vertexLookUp[v] = cnt;
				cnt++;
				if (hasPerVertexColors())		new_color.push_back(m_Colors[v]);
				if (hasPerVertexNormals())		new_normals.push_back(m_Normals[v]);
				if (hasPerVertexTexCoords())	new_tex.push_back(m_TextureCoords[v]);
			} else {
				vertexLookUp[v] = nn;
			}
		}
	} else {
		SparseGrid3D<std::list<std::pair<point3d<FloatType>, unsigned int>>> neighborQuery(0.6f, numV*2);
		for (unsigned int v = 0; v < numV; v++) {

			const point3d<FloatType>& vert = m_Vertices[v];
			vec3i coord = toVirtualVoxelPos(vert, thresh);		
			unsigned int nn = hasNearestNeighbor(coord, neighborQuery, vert, thresh);

			if (nn == (unsigned int)-1) {
				neighborQuery[coord].push_back(std::make_pair(vert,cnt));
				new_verts.push_back(vert);
				vertexLookUp[v] = cnt;
				cnt++;
				if (hasPerVertexColors())		new_color.push_back(m_Colors[v]);
				if (hasPerVertexNormals())		new_normals.push_back(m_Normals[v]);
				if (hasPerVertexTexCoords())	new_tex.push_back(m_TextureCoords[v]);
			} else {
				vertexLookUp[v] = nn;
			}
		}
	}

	// Update faces
	for (std::vector<std::vector<unsigned int>>::iterator it = m_FaceIndicesVertices.begin(); it != m_FaceIndicesVertices.end(); it++) {
		for (std::vector<unsigned int>::iterator idx = it->begin(); idx != it->end(); idx++) {
			*idx = vertexLookUp[*idx];
		}
	}

	m_Vertices = std::vector<point3d<FloatType>>(new_verts.begin(), new_verts.end());
	if (hasPerVertexColors())		m_Colors = std::vector<point4d<FloatType>>(new_color.begin(), new_color.end());
	if (hasPerVertexNormals())		m_Normals = std::vector<point3d<FloatType>>(new_normals.begin(), new_normals.end());
	if (hasPerVertexTexCoords())	m_TextureCoords = std::vector<point2d<FloatType>>(new_tex.begin(), new_tex.end());

	removeDegeneratedFaces();
	std::cout << "Merged " << numV-cnt << " of " << numV << " vertices" << std::endl;
	return cnt;
}




}  // namespace ml

#endif  // CORE_MESH_MESHDATA_H_

