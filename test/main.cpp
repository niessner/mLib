
#include "main.h"

class App
{
public:
	void go();

private:
	TestVector m_vector;
	TestString m_string;
	TestUtility m_utility;
	TestMath m_math;
	TestANN m_ANN;
	TestLodePNG m_lodePNG;
};

void App::go()
{
	Console::openLogFile("console.txt");
	m_vector.run();
	m_string.run();
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

	std::string sd;
	String s = "hello";
	s.reserve(100);
	s = s.removeChar('l');
	s.clear();
	s.pushBack('a');
	s.pushBack('b');
	s.popFront();
	std::cout << s << std::endl;
	//s << "a";
	//s << 1;
	//s << true;
	//std::cout << s << std::endl;
	//try {
	//	throw MLIB_EXCEPTION("exp");
	//} catch(std::exception& e) {
	//	std::cout << e.what() << std::endl;
	//}
	//MLIB_WARNING("bla");
	//App a;
	//a.go();

	getchar();
	return 0;
}
