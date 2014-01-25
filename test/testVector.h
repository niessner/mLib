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

		Console::log() << "vector test passed" << std::endl;
	}

	void test1()
	{
		Grid<String> g(10, 20);

		g.clear("cde");

		g(4, 8) = "zzz";
		g(6, 3) = "aaa";
		g.setRow(2, g.getRow(5));

		for(UINT y = 0; y < g.rows(); y++)
			for(UINT x = 0; x < g.cols(); x++)
				g(y, x) = g(y, x).findAndReplace("aaa", "bbb");

		MLIB_ASSERT(g(6, 3) == "bbb", "grid test0 failed");
		MLIB_ASSERT(g.minIndex().first == 6 && g.minIndex().second == 3, "grid test1 failed");
		MLIB_ASSERT(g.maxIndex().first == 4 && g.maxIndex().second == 8, "grid test2 failed");

		Console::log() << "grid test passed" << std::endl;
	}

	std::string name()
	{
		return "vector";
	}
};