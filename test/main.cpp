
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
};

void App::go()
{
	Console::openLogFile("console.txt");
	m_vector.run();
	m_utility.run();
	m_math.run();
	m_ANN.run();
	m_lodePNG.run();

	Console::log("All tests completed");
	std::cin.get();
}




int main()
{
	// Enable run-time memory check for debug builds.
#if defined(DEBUG) | defined(_DEBUG)
	_CrtSetDbgFlag( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );
#endif

	/*CalibratedSensorData data;	data.m_ColorImageWidth = 500;
	BinaryDataStreamFile streamOut("test.out", true);
	streamOut << data;
	streamOut.closeStream();
	CalibratedSensorData reRead;
	BinaryDataStreamFile streamIn("test.out", false);
	streamIn >> reRead;

	int a = 5;*/


	//std::string s = "hello.exe";
	//std::cout << StringUtil::findAndReplace(s, "ll", "") << std::endl;
	//ParameterFile pm("bla.txt");
	//unsigned int a = 4;
	//pm.getParameterForValue("a", a);
	//std::cout << a << std::endl;

	//mat3f m;
	//m(0,0) = 1;
	//std::cout << mat3f::identity() << std::endl;
	//std::cout << mat3f::scale(3.0f) << std::endl;
	//mat4f m4;
	//std::cout << mat4f::rotation(0, 0, 45) << std::endl;

	//mat2f m2 = mat2f::identity();
	//m2(0,0) = 4;
	//m2 = m2 * m2.getInverse();
	//std::cout << m2 << std::endl;

	App a;
	a.go();

	getchar();
	return 0;
}
