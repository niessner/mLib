class TestBox : public Test
{
public:
	void test0()
	{
		size_t size = 1000;
		std::vector<vec3f> points(size);
		RNG rng;
		for (size_t i = 0; i < size; i++) {
			points[i] = vec3f(rng.rand_closed01(), rng.rand_closed01(), rng.rand_closed01());
		}

		ml::Console::log() << "box test0 passed" << std::endl;
	}

	std::string name()
	{
		return "Box";
	}
private:
};