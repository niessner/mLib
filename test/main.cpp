
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
};

void App::go()
{
	Console::openLogFile("console.txt");

	m_vector.run();
	m_string.run();
	m_utility.run();
	m_math.run();
	m_ANN.run();

	Console::log("All tests completed");
	std::cin.get();
}

void main()
{
	App a;
	a.go();

}
