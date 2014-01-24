class TestString : public Test
{
public:
	void test0()
	{
		String s("this is ");
		s = s + "a string";
		MLIB_ASSERT(s.split(" ").size() == 4, "split test0 failed");
		MLIB_ASSERT(s.split(' ').back() == "string", "split test1 failed");

		MLIB_ASSERT(s.startsWith("this is"), "startsWith test failed");

		for(int i = 0; i < 100; i++)
		{
			MLIB_ASSERT(String(i).toInt32() == i, "convert test failed");
		}

		Console::log() << "string test0 passed" << std::endl;
	}

	std::string name()
	{
		return "vector";
	}
};