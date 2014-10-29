
class TestOpenMesh : public Test {
public:
	void test0() 
	{
		TriMeshf s = shapes::sphere(0.1f, vec3f(0.0f), 50, 50);
		MeshIOf::saveToFile("bla0.ply",s.getMeshData());
		OpenMeshTriMesh::Mesh omesh;
		OpenMeshTriMesh::convertToOpenMesh(s, omesh);
		OpenMeshTriMesh::convertToTriMesh(omesh, s);
		MeshIOf::saveToFile("bla1.ply",s.getMeshData());
		
		MeshDataf meshData0 = s.getMeshData();
		meshData0.mergeCloseVertices(0.00001f, true);
		s = TriMeshf(meshData0);
		
		MeshIOf::saveToFile("bla2.ply",s.getMeshData());

		size_t vertsBefore = s.getVertices().size();
		OpenMeshTriMesh::decimate(s, 500);
		std::cout << "mesh reduced from " << vertsBefore << " to " << s.getVertices().size() << std::endl;
		MeshIOf::saveToFile("bla3.ply",s.getMeshData());


		ml::Console::log() << "Open Mesh test0 passed" << std::endl;
	}



	std::string name() {
		return "Open Mesh";
	}
private:


};