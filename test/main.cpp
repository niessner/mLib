
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


	std::string s = "hello.exe";
	std::cout << StringUtil::findAndReplace(s, "ll", "") << std::endl;
	ParameterFile pm("bla.txt");
	unsigned int a = 4;
	pm.getParameterForValue("a", a);
	std::cout << a << std::endl;

	//App a;
	//a.go();

	getchar();
	return 0;
}
