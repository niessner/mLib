
#include "main.h"

class App
{
public:
	void go();

private:
	TestUtility m_utility;
	TestMath m_math;
	TestANN m_ANN;
	TestLodePNG m_lodePNG;
	TestBinaryStream m_binaryStream;
};

void App::go()
{
	Console::openLogFile("console.txt");
	m_utility.run();
	m_math.run();
	m_ANN.run();
	m_lodePNG.run();
	m_binaryStream.run();

	Console::log("All tests completed");
	std::cin.get();
}


struct VoxelEntry {
	vec3i m_Coords;
	unsigned int m_Index;
	void print() {
		std::cout << m_Coords << " : " << m_Index << std::endl;
	}
};

inline std::ostream& operator<<(std::ostream& s, const VoxelEntry& e) {
	s << e.m_Index;
	return s;
}


int main()
{
	// Enable run-time memory check for debug builds.
#if defined(DEBUG) | defined(_DEBUG)
	_CrtSetDbgFlag( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );
#endif

	SparseGrid3D<VoxelEntry> grid;
	unsigned int check = 0;
	Clock timer;
	RNG rng;
	for (UINT i = 0; i < 100; i++) {
		const int ext = 100;
		vec3i coord = vec3i(rng.rand_int32()%ext, rng.rand_int32()%ext, rng.rand_int32()%ext);
		VoxelEntry entry;	entry.m_Coords = coord;		entry.m_Index = i;
		grid(coord) = entry;
		const VoxelEntry &res = grid(coord);
		const VoxelEntry &res1 = (*(const SparseGrid3D<VoxelEntry>*)&grid)(coord);
		//std::cout << res << std::endl;
	}

	std::cout << grid << std::endl;
	std::cout << "time elapsed: " << timer.elapsed() << std::endl;

	for (auto &a : grid ) {
		a.second.print();
	}
	//unsigned int check = 0;
	//Clock timer;
	//std::unordered_map<vec3i, VoxelEntry, std::hash<vec3i>> map;
	////map.max_load_factor(0.01);
	//map.reserve(1 << 14);
	//for (unsigned int i = 0; i < 50; i++) {
	//	//vec3i coords = vec3i(i,i,i);
	//	vec3i coords = vec3i(i,0,0);
	//	VoxelEntry ve;
	//	ve.m_Coords = coords;
	//	ve.m_Int = i/10;
	//	std::pair<vec3i, VoxelEntry> pair(coords, ve);
	//	auto iter = map.insert(pair);

	//	//if (map.find(coords) != map.end()) {
	//	//	check++;
	//	//}

	//	//std::cout << "max_bucket_count: " << map.max_bucket_count() << std::endl;
	//	//std::cout << "bucket_count: " << map.bucket_count() << std::endl;
	//	//std::cout << "load_factor: " << map.load_factor() << std::endl;
	//}

	//for (size_t i = 0; i < map.bucket_count(); i++) {
	//	if (map.bucket_size(i) > 1) {
	//		std::cout << "bucket_size " << i << " : " << map.bucket_size(i) << std::endl;
	//	}
	//}
	//std::cout << "time elapsed: " << timer.elapsed() << std::endl;
	//std::cout << "check: " << check << std::endl;
	//std::cout << "bucket_count: " << map.bucket_count() << std::endl;
	//std::cout << "load_factor: " << map.load_factor() << std::endl;

	//auto res = map.find(vec3i(5,3,5));
	//if (res != map.end()) {
	//	res->first.print();
	//} else {
	//	std::cout << "not found" << std::endl;
	//}

	//for (auto iter = map.begin(); iter != map.end(); iter++) {
	//	iter->second.print();
	//}
		 

	try {
		throw MLIB_EXCEPTION("bla");
	} catch (std::exception& e) {
		Console::log() << e.what() << std::endl;
	}

	App a;
	a.go();

	getchar();
	return 0;
}
