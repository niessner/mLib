

#include "stdafx.h"

#include "main.h"

using namespace ml;


class App
{
public:
	void go() {
		m_grid.run();

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
	App a;
	a.go();

	getchar();
	return 0;
}
