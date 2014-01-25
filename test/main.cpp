
#include "main.h"

class App
{
public:
	void go();

private:
	TestVector m_vector;
	TestString m_string;
	TestUtility m_utility;
};

void App::go()
{
	Console::openLogFile("console.txt");

	m_vector.run();
	m_string.run();
	m_utility.run();

	Console::log("All tests completed");
	std::cin.get();
}

void main()
{
	App a;
	a.go();

}
