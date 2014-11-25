
class TestBox : public Test
{
public:
	void test0()
	{
		size_t size = 1000;
		std::vector<vec3f> points(size);
		RNG rng;
		for (size_t i = 0; i < size; i++) {
			points[i] = vec3f((float)rng.rand_closed01(), (float)rng.rand_closed01(), (float)rng.rand_closed01());
			points[i].x *= 3.0f;
		}

		OBBf obb = CGALWrapperf::computeOrientedBoundingBox(points);

		ml::Console::log() << "box test0 passed" << std::endl;
	}

	std::string name()
	{
		return "Box";
	}
private:

};
