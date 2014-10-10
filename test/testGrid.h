
class TestGrid : public Test {
public:
	void test0() 
	{
		unsigned int s = 10;
		ml::BinaryGrid3d grid(s,s,s);
		for (unsigned int i = 0; i < s; i++) {
			for (unsigned int j = 0; j < s; j++) {
				for (unsigned int k = 0; k < s; k++) {
					grid.setVoxel(i,j,k);
					MLIB_ASSERT_STR(grid.isVoxelSet(i,j,k), "voxel set failed");
					MLIB_ASSERT_STR(checkIfAllOtherAreZero(grid, i,j,k), "voxel set failed");
					grid.clearVoxel(i,j,k);
				}
			}
		}

		grid.clear();
		for (unsigned int i = 0; i < s; i++) {
			grid.setVoxel(i,i,i);
		}

		ml::Console::log() << "binaryGrid3d test passed" << std::endl;
	}

	void test1()
	{
		unsigned int s = 50;
		ml::BinaryGrid3d grid(s,s,s);
		for (unsigned int i = 0; i < s; i++) {
			grid.setVoxel(i,i,i);
			grid.setVoxel(50-1-i,i,i);
			grid.setVoxel(i,50-1-i,i);
			grid.setVoxel(i,i,50-1-i);
		}
		ml::PointCloudf pc(grid, 1.0f);
		ml::PointCloudIOf::saveToFile("gridcloud0.ply", pc);
	}

	void test2() 
	{
		//BoundingBox3f bb;
		//bb.include(vec3f(0.0f,0.0f,0.0f));
		//bb.include(vec3f(1.0f,1.0f,1.0f));
		//std::vector<vec3f> p;	std::vector<vec3ui> i;
		//bb.makeTriMesh(p, i);
		//TriMeshf tmesh(p.size(), i.size() * 3, p.data(), (unsigned int*)i.data());
		//MeshIOf::saveToFile("box.ply", tmesh.getMeshData());

		ml::TriMeshf box = ml::shapes::sphere(5.0f, ml::vec3f(0,0,0), 128, 128);
		box.transform(ml::mat4f::translation(ml::vec3f(6.0f)));

		ml::BinaryGrid3d grid = box.voxelize(0.05f);
		ml::PointCloudf pc(grid, 1.0f);
		ml::PointCloudIOf::saveToFile("gridcloud1.ply", pc);

		TriMeshf voxelMesh(grid);
		MeshIOf::saveToFile("box.ply", voxelMesh.getMeshData());

		int a = 5;
	}


	std::string name() {
		return "grid";
	}
private:

	bool checkIfAllOtherAreZero(const ml::BinaryGrid3d& grid, unsigned int _i, unsigned int _j, unsigned int _k) {
		for (unsigned int i = 0; i < grid.rows(); i++) {
			if (i == _i) continue;
			for (unsigned int j = 0; j < grid.cols(); j++) {
				if (j == _j) continue;
				for (unsigned int k = 0; k < grid.slices(); k++) {
					if (k == _k) continue;
					if (grid.isVoxelSet(i,j,k)) return false;
				}
			}
		}
		return true;
	}
};