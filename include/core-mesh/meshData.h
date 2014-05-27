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

	bool isConsistent(bool detailedCheck = false) const {

		bool consistent = true;
		if (m_FaceIndicesNormals.size() > 0			&& m_FaceIndicesVertices.size() != m_FaceIndicesNormals.size())			consistent = false;
		if (m_FaceIndicesTextureCoords.size() > 0	&& m_FaceIndicesVertices.size() != m_FaceIndicesTextureCoords.size())	consistent = false;
		if (m_FaceIndicesColors.size() > 0			&& m_FaceIndicesColors.size() != m_FaceIndicesColors.size())			consistent = false;

		if (hasPerVertexNormals() && m_Vertices.size() != m_Normals.size())			consistent = false;
		if (hasPerVertexTexCoords() && m_Vertices.size() != m_TextureCoords.size()) consistent = false;
		if (hasPerVertexColors() && m_Vertices.size() != m_Colors.size())			consistent = false;

		if (detailedCheck) {
			//make sure no index is out of bounds
			for (auto& face : m_FaceIndicesVertices) {
				for (auto& idx : face) {
					if (idx >= m_Vertices.size())	consistent = false;
				}
			}
			for (auto& face : m_FaceIndicesColors) {
				for (auto& idx : face) {
					if (idx >= m_Colors.size())	consistent = false;
				}
			}
			for (auto& face : m_FaceIndicesNormals) {
				for (auto& idx : face) {
					if (idx >= m_FaceIndicesNormals.size())	consistent = false;
				}
			}
		}
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

	bool hasVertexIndices() const { return m_FaceIndicesVertices.size() > 0; }
	bool hasColorIndices() const { return m_FaceIndicesColors.size() > 0; }
	bool hasNormalIndices() const { return m_FaceIndicesNormals.size() > 0; }
	bool hasTexCoordsIndices() const { return m_FaceIndicesTextureCoords.size() > 0; }

	//! todo check this
	bool isEmpty() const {
		return m_Vertices.size() == 0 && m_FaceIndicesVertices.size() == 0;
	}

	//! merges two meshes (assumes the same memory layout/type)
	void merge(const MeshData<FloatType>& other);
	unsigned int removeDuplicateVertices();
	unsigned int removeDuplicateFaces();
	unsigned int mergeCloseVertices(FloatType thresh, bool approx = false);
	unsigned int removeDegeneratedFaces();

	//! also removes isolated normals, colors, etc.
	unsigned int removeIsolatedVertices();

	//! removes all the vertices that are behind a plane (faces with one or more of those vertices are being deleted as well)
	unsigned int removeVerticesBehindPlane(const Plane<FloatType>& plane, FloatType thresh);

	std::vector<point3d<FloatType>>	m_Vertices;			//vertices are indexed (see below)
	std::vector<point3d<FloatType>>	m_Normals;			//normals are indexed (see below/or per vertex)
	std::vector<point2d<FloatType>>	m_TextureCoords;	//tex coords are indexed (see below/or per vertex)
	std::vector<point4d<FloatType>>	m_Colors;			//colors are not indexed (see below/or per vertex) 
	std::vector<std::vector<unsigned int>>	m_FaceIndicesVertices;		//indices in face array
	std::vector<std::vector<unsigned int>>	m_FaceIndicesNormals;		//indices in normal array (if size==0, indicesVertices is used)
	std::vector<std::vector<unsigned int>>	m_FaceIndicesTextureCoords;	//indices in texture array (if size==0, indicesVertices is used)
	std::vector<std::vector<unsigned int>>	m_FaceIndicesColors;		//indices in color array (if size==0, indicesVertices is used)

	//! Debug print with all details
	void print() const {
		std::cout << "Vertices:\n";
		std::cout << m_Vertices << std::endl;
		std::cout << "Faces:\n";
		std::cout << m_FaceIndicesVertices << std::endl;
	}

	//! inserts a midpoint into every faces; and triangulates the result
	void subdivideFaces() {
		m_Vertices.reserve(m_Vertices.size() + m_FaceIndicesVertices.size());	//there will be 1 new vertex per face
		if (hasPerVertexColors())		m_Colors.reserve(m_Colors.size() + m_FaceIndicesVertices.size());
		if (hasPerVertexNormals())		m_Normals.reserve(m_Normals.size() + m_FaceIndicesVertices.size());
		if (hasPerVertexTexCoords())	m_TextureCoords.reserve(m_TextureCoords.size() + m_FaceIndicesVertices.size());

		std::vector<std::vector<unsigned int>> newFaces;
		for (auto& face : m_FaceIndicesVertices) {
			point3d<FloatType> centerP = point3d<FloatType>(0,0,0);
			for (auto& idx : face) {
				centerP += m_Vertices[idx];
			}
			centerP /= (FloatType)face.size();
			m_Vertices.push_back(centerP);

			if (hasPerVertexColors()) {
				point4d<FloatType> centerC = point4d<FloatType>(0,0,0,0);
				for (auto& idx : face) {
					centerC += m_Colors[idx];
				}
				centerC /= (FloatType)face.size();
				m_Colors.push_back(centerC);
			}
			if (hasPerVertexNormals()) {
				point3d<FloatType> centerN = point3d<FloatType>(0,0,0);
				for (auto& idx : face) {
					centerN += m_Normals[idx];
				}
				centerN /= (FloatType)face.size();
				m_Normals.push_back(centerN);
			}
			if (hasPerVertexTexCoords()) {
				point2d<FloatType> centerT = point2d<FloatType>(0,0);
				for (auto& idx : face) {
					centerT += m_TextureCoords[idx];
				}
				centerT /= (FloatType)face.size();
				m_TextureCoords.push_back(centerT);
			}
			

			unsigned int newIdx = (unsigned int)m_Vertices.size() - 1;
			for (size_t i = 0; i < face.size(); i++) {
				newFaces.push_back(std::vector<unsigned int>(3));
				newFaces[newFaces.size()-1][0] = face[i];
				newFaces[newFaces.size()-1][1] = face[(i+1)%face.size()];
				newFaces[newFaces.size()-1][2] = newIdx;
			}
		}

		m_FaceIndicesVertices = newFaces;
	}

    //! inserts a midpoint into every faces; and triangulates the result
    FloatType subdivideFacesLoop(float edgeThresh = 0.0f) {
        m_Vertices.reserve(m_Vertices.size() + m_FaceIndicesVertices.size());	//there will be 1 new vertex per face (TODO FIX)
        if (hasPerVertexColors())		m_Colors.reserve(m_Colors.size() + m_FaceIndicesVertices.size());
        if (hasPerVertexNormals())		m_Normals.reserve(m_Normals.size() + m_FaceIndicesVertices.size());
        if (hasPerVertexTexCoords())	m_TextureCoords.reserve(m_TextureCoords.size() + m_FaceIndicesVertices.size());


        struct Edge {
            Edge(unsigned int _v0, unsigned int _v1) {
                if (_v0 < _v1) {
                    v0 = _v0;
                    v1 = _v1;
                } else {
                     v1 = _v0;
                     v0 = _v1;
                }
            }
            bool operator==(const Edge& other) const {
                return v0==other.v0 && v1==other.v1;
            }

            bool needEdgeVertex(float thresh, const std::vector<point3d<FloatType>>& vertices) const {
                if (thresh == 0.0f) return true;
                else {
                    return ((vertices[v0] - vertices[v1]).lengthSq() > thresh*thresh);
                }
            }

            float edgeLength(const std::vector<point3d<FloatType>>& vertices) const {
                return (vertices[v0] - vertices[v1]).length();
            } 

            unsigned int v0;
            unsigned int v1;
        };

        struct EdgeHash {
            size_t operator()(const Edge& e) const {
                //TODO larger prime number (64 bit) to match size_t
                const size_t p[] = {73856093, 19349669};
                return e.v0*p[0] ^ e.v1*p[1];
                //const size_t res = ((size_t)v.x * p0)^((size_t)v.y * p1)^((size_t)v.z * p2);
            }
        };

        FloatType maxEdgeLen = 0.0f;

        //maps edges to new vertex indices
        std::unordered_map<Edge, unsigned int, EdgeHash> edgeMap;
        for (const std::vector<unsigned int>& face : m_FaceIndicesVertices) {

            for (unsigned int i = 0; i < face.size(); i++) {
                Edge e(face[i], face[(i+1)%face.size()]);

                FloatType edgeLen = e.edgeLength(m_Vertices);
                if (edgeLen > maxEdgeLen) maxEdgeLen = edgeLen;

                if (e.needEdgeVertex(edgeThresh, m_Vertices)) {
                    if (edgeMap.find(e) == edgeMap.end()) {
                        m_Vertices.push_back((FloatType)0.5*(m_Vertices[e.v0] + m_Vertices[e.v1]));
                        if (hasPerVertexColors()) m_Colors.push_back((FloatType)0.5*(m_Colors[e.v0] + m_Colors[e.v1]));
                        if (hasPerVertexNormals()) m_Normals.push_back((FloatType)0.5*(m_Normals[e.v0] + m_Normals[e.v1]));
                        if (hasPerVertexTexCoords()) m_TextureCoords.push_back((FloatType)0.5*(m_TextureCoords[e.v0] + m_TextureCoords[e.v1]));
                        unsigned int idx = (unsigned int)m_Vertices.size() - 1;
                        edgeMap[e] = idx;
                    }
                }
            }

        }

        std::vector<std::vector<unsigned int>> newFaces;    newFaces.reserve(m_FaceIndicesVertices.size() * 4);
        for (const std::vector<unsigned int>& face : m_FaceIndicesVertices) {
            bool allEdgesExist = true;
            bool noneEdgesExist = true;
            for (unsigned int i = 0; i < face.size(); i++) {
                Edge e(face[i], face[(i+1)%face.size()]);
                if (edgeMap.find(e) == edgeMap.end())   {
                    allEdgesExist = false;
                } else {
                    noneEdgesExist = false;
                }
            }

            if (allEdgesExist) {
                std::vector<unsigned int> centerFace(face.size());
                for (unsigned int i = 0; i < face.size(); i++) {
                    Edge ePrev(face[i], face[(i+1)%face.size()]);
                    Edge eNext(face[(i+1)%face.size()], face[(i+2)%face.size()]);
                    newFaces.push_back(std::vector<unsigned int>(3));
                    newFaces.back()[0] = edgeMap[ePrev];
                    newFaces.back()[1] = face[(i+1)%face.size()];
                    newFaces.back()[2] = edgeMap[eNext];

                    centerFace[i] = newFaces.back()[0];
                }
                newFaces.push_back(centerFace);

            } 
            else if (noneEdgesExist) {
                newFaces.push_back(face);
            }
            else {
                std::vector<unsigned int> cFace;
                for (unsigned int i = 0; i < face.size(); i++) {
                    cFace.push_back(face[i]);             
                    Edge e(face[i], face[(i+1)%face.size()]);
                    if (edgeMap.find(e) != edgeMap.end())   cFace.push_back(edgeMap[e]);
                }

                //centroid based vertex insertion
                point3d<FloatType> centerP = point3d<FloatType>(0,0,0);
                for (auto& idx : face) {
                    centerP += m_Vertices[idx];
                }
                centerP /= (FloatType)face.size();
                m_Vertices.push_back(centerP);

                if (hasPerVertexColors()) {
                    point4d<FloatType> centerC = point4d<FloatType>(0,0,0,0);
                    for (auto& idx : face) {
                        centerC += m_Colors[idx];
                    }
                    centerC /= (FloatType)face.size();
                    m_Colors.push_back(centerC);
                }
                if (hasPerVertexNormals()) {
                    point3d<FloatType> centerN = point3d<FloatType>(0,0,0);
                    for (auto& idx : face) {
                        centerN += m_Normals[idx];
                    }
                    centerN /= (FloatType)face.size();
                    m_Normals.push_back(centerN);
                }
                if (hasPerVertexTexCoords()) {
                    point2d<FloatType> centerT = point2d<FloatType>(0,0);
                    for (auto& idx : face) {
                        centerT += m_TextureCoords[idx];
                    }
                    centerT /= (FloatType)face.size();
                    m_TextureCoords.push_back(centerT);
                }


                unsigned int newIdx = (unsigned int)m_Vertices.size() - 1;
                for (size_t i = 0; i < cFace.size(); i++) {
                    newFaces.push_back(std::vector<unsigned int>(3));
                    newFaces[newFaces.size()-1][0] = cFace[i];
                    newFaces[newFaces.size()-1][1] = cFace[(i+1)%cFace.size()];
                    newFaces[newFaces.size()-1][2] = newIdx;
                }
            } 
        }
     
        m_FaceIndicesVertices = std::vector<std::vector<unsigned int>>(newFaces.begin(), newFaces.end());
        return maxEdgeLen;
    }
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


} // namespace ml

#include "meshData.cpp"

#endif  // CORE_MESH_MESHDATA_H_

