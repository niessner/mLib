#ifndef EXT_OPENMESH_TRIMESH_H_
#define EXT_OPENMESH_TRIMESH_H_

#include <string>

namespace ml {
//////////////////////////////////////////////////////////////////////////
// TriMesh Operations (simplification, fairing etc)						//
//////////////////////////////////////////////////////////////////////////

namespace OpenMeshTriMesh {

	struct Traits : OpenMesh::DefaultTraits
	{
		typedef OpenMesh::Vec3f Point;
		typedef OpenMesh::Vec3f Normal; 
		typedef OpenMesh::Vec4f Color;
		typedef OpenMesh::Vec2f TexCoord2D;

		VertexAttributes(OpenMesh::Attributes::Normal | OpenMesh::Attributes::Color | OpenMesh::Attributes::TexCoord2D );
		//FaceAttributes(Attributes::Status | Attributes::Normal);
		//EdgeAttributes(Attributes::Status);
	};

	typedef OpenMesh::TriMesh_ArrayKernelT<Traits> Mesh;					//mesh type
	typedef OpenMesh::Decimater::DecimaterT< Mesh > Decimater;				// Decimater type
	typedef OpenMesh::Decimater::ModQuadricT< Mesh >::Handle HModQuadric;	// Decimation Module Handle type


	static void convertToOpenMesh(const TriMeshf& triMesh, Mesh& out, bool keepVertexAttributes = false) {
		out.clear();
		
		if (keepVertexAttributes == false) {
			std::vector< Mesh::VertexHandle > vHandles;
			vHandles.reserve(triMesh.getVertices().size());

			//TODO ADD NORMALS

			for (const auto& v : triMesh.getVertices()) {
				vHandles.push_back(out.add_vertex(Mesh::Point(v.position.x, v.position.y, v.position.z)));
				//vHandles.push_back(out.add_vertex(
				//	Mesh::Point(v.position.x, v.position.y, v.position.z),
				//	Mesh::Normal(v.normal.x, v.normal.y, v.normal.z),
				//	Mesh::Color(v.color.x, v.color.y, v.color.z, v.color.w),
				//	Mesh::TexCoord2D(v.texCoord.x, v.texCoord.y)
				//	));	
			}
		
			for (const auto& f : triMesh.getIndices()) {
				out.add_face(vHandles[f.x], vHandles[f.y], vHandles[f.z]);
			}
		} else {
			throw MLIB_EXCEPTION("TODO implement");
		}
	}

	static void convertToTriMesh(Mesh& mesh, TriMeshf& out, bool keepVertexAttributes = false) {
		
		MeshDataf mData;
		if (keepVertexAttributes == false) {
			mData.m_Vertices.resize(mesh.n_vertices());
			for (unsigned int i = 0; i < mesh.n_vertices(); i++) {
				const auto& v = mesh.point(Mesh::VertexHandle(i));
				mData.m_Vertices[i] = vec3f(v.data()[0], v.data()[1], v.data()[2]);
			}

			mData.m_FaceIndicesVertices.resize(mesh.n_faces());
			for (unsigned int i = 0; i < mesh.n_faces(); i++) {
				const auto& f = mesh.face(Mesh::FaceHandle(i));
				auto iter = mesh.fv_iter(Mesh::FaceHandle(i));
				mData.m_FaceIndicesVertices[i][0] = iter->idx();
				iter++;									
				mData.m_FaceIndicesVertices[i][1] = iter->idx();
				iter++;									
				mData.m_FaceIndicesVertices[i][2] = iter->idx();			
			}
		} else {
			throw MLIB_EXCEPTION("TODO implement");
		}
		out = TriMeshf(mData);
	}

	//! decimates a mesh to a specific target vertex number; REVOMES ALSO ALL COLOR/NORMAL/TEXCOORD data
	static void decimate(TriMeshf& triMesh, size_t targetNumVertices, bool keepVertexAttributes = false) {
		Mesh        mesh;									// a mesh object
		convertToOpenMesh(triMesh, mesh, keepVertexAttributes);

		Decimater   decimater(mesh);						// a decimater object, connected to a mesh
		HModQuadric hModQuadric;							// use a quadric module
		decimater.add( hModQuadric );						// register module at the decimater
		decimater.initialize();								// let the decimater initialize the mesh and the modules
		decimater.decimate_to(targetNumVertices);			// do decimation
		//mesh.garbage_collection();
		convertToTriMesh(mesh, triMesh, keepVertexAttributes);
	}

}	//namespace OpenMeshTriMesh
}	//namespace ml

#endif	// EXT_OPENMESH_TRIMESH_H_