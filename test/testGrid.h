
class TestGrid : public Test {
public:
	void test0() 
	{
		unsigned int s = 10;
		ml::BinaryGrid3 grid(s,s,s);
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

		ml::Console::log() << "binaryGrid3d test0 passed" << std::endl;
	}

	void test1()
	{
		unsigned int s = 50;
		ml::BinaryGrid3 grid(s,s,s);
		for (unsigned int i = 0; i < s; i++) {
			grid.setVoxel(i,i,i);
			grid.setVoxel(50-1-i,i,i);
			grid.setVoxel(i,50-1-i,i);
			grid.setVoxel(i,i,50-1-i);
		}
		ml::PointCloudf pc(grid, 1.0f);
		ml::PointCloudIOf::saveToFile("gridcloud0.ply", pc);

		ml::Console::log() << "binaryGrid3d test1 passed" << std::endl;
	}

	void test2() 
	{
		BoundingBox3f bb;
		bb.include(vec3f(0.0f,0.0f,0.0f));
		bb.include(vec3f(10.0f,10.0f,10.0f));
		mat4f trans = bb.cubeToWorldTransform();
		vec3f p0 = trans * vec3f(0.0f,0.0f,0.0f);
		vec3f p1 = trans * vec3f(1.0f,1.0f,1.0f);

		//bb.include(vec3f(1.0f,1.0f,1.0f));
		//std::vector<vec3f> p;	std::vector<vec3ui> i;
		//bb.makeTriMesh(p, i);
		//TriMeshf tmesh(p.size(), i.size() * 3, p.data(), (unsigned int*)i.data());
		//MeshIOf::saveToFile("box.ply", tmesh.getMeshData());

		ml::TriMeshf sphere = ml::shapes::sphere(5.0f, ml::vec3f(0,0,0), 128, 128);

		sphere.transform(ml::mat4f::translation(ml::vec3f(-6.0053f)));
		sphere.transform(ml::mat4f::rotation(0.0012f, 0.021f, 0.0024f));

		Timer t;
		std::pair<ml::BinaryGrid3, ml::mat4f> grid = sphere.voxelize(0.25f);
		std::cout << "voxelization time " << t.getElapsedTimeMS() << std::endl;
		ml::PointCloudf pc(grid.first, 1.0f);
		ml::PointCloudIOf::saveToFile("gridcloud1.ply", pc);

		TriMeshf voxelMesh(grid.first, grid.second.getInverse());
		MeshIOf::saveToFile("box.ply", voxelMesh.getMeshData());

		ml::Console::log() << "binaryGrid3d test2 passed" << std::endl;
	}


	void test3() 
	{
		{
			//simplest case
			size_t s = 10;
			BinaryGrid3 grid(s, 1, 1);
			grid.setVoxel(0, 0, 0);
			grid.setVoxel(s - 1, 0, 0);
			DistanceField3f df(grid);
			//for (size_t i = 0; i < s; i++) {
			//	std::cout << df(i, 0, 0) << std::endl;
			//	//math::floatEqual()
			//}

			df.evalDist(grid, mat4f::identity());
		}
		{
			//2d test case
			size_t s = 10;
			BinaryGrid3 grid(s, s, 1);
			grid.setVoxel(0, 0, 0);
			grid.setVoxel(s - 1, s - 1, 0);
			DistanceField3f df(grid);
			//for (size_t i = 0; i < s; i++) {
			//	for (size_t j = 0; j < s; j++) {
			//		std::cout << math::round(df(i, j, 0)) << " ";
			//	}
			//	std::cout << std::endl;
			//}

		}


		ml::TriMeshf sphere = ml::shapes::sphere(5.0f, ml::vec3f(0, 0, 0), 128, 128);

		sphere.transform(ml::mat4f::translation(ml::vec3f(-6.0053f)));
		sphere.transform(ml::mat4f::rotation(0.0012f, 0.021f, 0.0024f));
		std::pair<ml::BinaryGrid3, ml::mat4f> grid = sphere.voxelize(0.25f);
		
		Timer t;
		DistanceField3f df(grid.first);
		std::cout << t.getElapsedTimeMS() << " ms" << std::endl;


		ml::Console::log() << "distanceField test3 passed" << std::endl;
	}

	std::string name() {
		return "grid";
	}
private:

	bool checkIfAllOtherAreZero(const ml::BinaryGrid3& grid, unsigned int _i, unsigned int _j, unsigned int _k) {
		for (unsigned int i = 0; i < grid.dimX(); i++) {
			if (i == _i) continue;
			for (unsigned int j = 0; j < grid.dimY(); j++) {
				if (j == _j) continue;
				for (unsigned int k = 0; k < grid.dimZ(); k++) {
					if (k == _k) continue;
					if (grid.isVoxelSet(i,j,k)) return false;
				}
			}
		}
		return true;
	}
};