
class TestGrid : public Test {
public:
	void test0() 
	{
		unsigned int s = 10;
		ml::BinaryGrid3d grid(s,s,s);
		grid.clear();
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
		grid.print();
		for (unsigned int i = 0; i < s; i++) {
			grid.setVoxel(i,i,i);
		}
		grid.print();

		ml::Console::log() << "binaryGrid3d test passed" << std::endl;
	}

	void test1()
	{

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