
#include "main.h"

class App
{
public:
	void go();

private:
	TestVector m_vector;
	TestUtility m_utility;
	TestMath m_math;
	TestANN m_ANN;
	TestLodePNG m_lodePNG;
	TestBinaryStream m_binaryStream;
};

void App::go()
{
	Console::openLogFile("console.txt");
	m_vector.run();
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
	unsigned int m_Int;
	void print() {
		std::cout << m_Coords << " : " << m_Int << std::endl;
	}
};

template<>
struct std::hash<vec3i> : public std::unary_function<vec3i, size_t> {
public:
	size_t operator()(const vec3i& v) const {
		const size_t p0 = 73856093;
		const size_t p1 = 19349669;
		const size_t p2 = 83492791;
		const size_t res = ((size_t)v.x * p0)^((size_t)v.y * p1)^((size_t)v.z * p2);
		//std::cout << "hash (of " << v << ") : " << res << std::endl;
		return res;
	}
private:

};

int main()
{
	// Enable run-time memory check for debug builds.
#if defined(DEBUG) | defined(_DEBUG)
	_CrtSetDbgFlag( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );
#endif

	unsigned int check = 0;
	Clock timer;
	std::unordered_map<vec3i, VoxelEntry, std::hash<vec3i>> map;
	//map.max_load_factor(0.01);
	map.reserve(1 << 14);
	for (unsigned int i = 0; i < 50; i++) {
		//vec3i coords = vec3i(i,i,i);
		vec3i coords = vec3i(i,0,0);
		VoxelEntry ve;
		ve.m_Coords = coords;
		ve.m_Int = i/10;
		std::pair<vec3i, VoxelEntry> pair(coords, ve);
		auto iter = map.insert(pair);

		//if (map.find(coords) != map.end()) {
		//	check++;
		//}

		//std::cout << "max_bucket_count: " << map.max_bucket_count() << std::endl;
		//std::cout << "bucket_count: " << map.bucket_count() << std::endl;
		//std::cout << "load_factor: " << map.load_factor() << std::endl;
	}

	for (size_t i = 0; i < map.bucket_count(); i++) {
		if (map.bucket_size(i) > 1) {
			std::cout << "bucket_size " << i << " : " << map.bucket_size(i) << std::endl;
		}
	}
	std::cout << "time elapsed: " << timer.elapsed() << std::endl;
	std::cout << "check: " << check << std::endl;
	std::cout << "bucket_count: " << map.bucket_count() << std::endl;
	std::cout << "load_factor: " << map.load_factor() << std::endl;

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
