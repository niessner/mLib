
class TestOpenMesh : public Test {
public:
	void test0() 
	{
		TriMeshf s = Shapesf::sphere(0.1f, vec3f(0.0f), 50, 50);
		MeshIOf::saveToFile("bla0.ply",s.makeMeshData());
		OpenMeshTriMesh::Mesh omesh;
		OpenMeshTriMesh::convertToOpenMesh(s, omesh);
		OpenMeshTriMesh::convertToTriMesh(omesh, s);
		MeshIOf::saveToFile("bla1.ply",s.makeMeshData());
		
		MeshDataf meshData0 = s.makeMeshData();
		meshData0.mergeCloseVertices(0.00001f, true);
		s = TriMeshf(meshData0);
		
		MeshIOf::saveToFile("bla2.ply",s.makeMeshData());

		size_t vertsBefore = s.getVertices().size();
		OpenMeshTriMesh::decimate(s, 500);
		std::cout << "mesh reduced from " << vertsBefore << " to " << s.getVertices().size() << std::endl;
		MeshIOf::saveToFile("bla3.ply",s.makeMeshData());


		std::cout <<"Open Mesh test0 passed" << std::endl;
	}



	std::string getName() {
		return "Open Mesh";
	}
private:


};