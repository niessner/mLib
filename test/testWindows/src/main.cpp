

#include "stdafx.h"

#include "main.h"

using namespace ml;


class App
{
public:
	void go() {
		m_grid.run();
		m_binaryStream.run();

		//m_box.run();
		//m_cgal.run();
		//m_utility.run();
		//m_lodePNG.run();
		//m_binaryStream.run();
		//m_grid.run();
		//m_openMesh.run();

		std::cout << "<< all tests done >>" << std::endl;
	}

private:
	TestGrid m_grid;
	TestBox m_box;
	TestCGAL m_cgal;	
	TestUtility m_utility;
	TestLodePNG m_lodePNG;
	TestBinaryStream m_binaryStream;
	TestOpenMesh m_openMesh;
};

int main()
{
	std::cout << Directory::enumerateDirectoriesWithPath("../") << std::endl;

	std::array<unsigned int, 2> hist;
	hist[0] = hist[1] = 0;
	for (size_t i = 0; i < 10000; i++) {
		hist[math::randomCointoss()]++;
	}
	std::cout << "false: " << hist[0] << "\t true: " << hist[1] << std::endl;

	App a;
	a.go();


	getchar();
	return 0;
}
