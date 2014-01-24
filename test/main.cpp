
#include "main.h"

class App
{
public:
	void go();

private:
	TestVector m_vector;
	TestString m_string;
};

void App::go()
{
	m_vector.run();
	m_string.run();

	Console::log("All tests completed");
	std::cin.get();
}

void main()
{
	App a;
	a.go();
}
