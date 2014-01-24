class TestVector : public Test
{
public:
	void test0()
	{
		Vector<int> v0;
		INT64 sum = 0, product = 1;

		auto f = [](int n) {return std::max(2 * n * n - 3 * n + 4, 1);};

		for(int i = 0; i < 100; i++)
		{
			sum += f(i);
			product *= f(i);
			v0.pushBack(i);
		}

		Vector<int> v1 = v0.map([f](int n)
		{
			return f(n);
		});

		Vector<std::string> v2 = v1.map([](int n)
		{
			std::stringstream s;
			s << n;
			return s.str();
		});

		Vector<int> v3 = v2.map([](const std::string &s)
		{
			return atoi(s.c_str());
		});

		//
		// product will actually be 0 because of overflow but still consistent
		//

		MLIB_ASSERT(sum == v1.sum() && sum == v3.sum(), "sum check failed");
		MLIB_ASSERT(product == v1.product() && product == v3.product(), "product check failed");

		Console::log() << "vector test0 passed" << std::endl;
	}

	std::string name()
	{
		return "vector";
	}
};