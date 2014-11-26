
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
			points[i].y *= 2.0f;
			points[i] = mat3f::rotationZ(45) * points[i];
		}

		points = CGALWrapperf::convexHull(points);

		std::cout << "PCA" << std::endl;
		std::cout << CGALWrapperf::computeOrientedBoundingBox(points, CGALWrapperf::PCA) << std::endl;
		std::cout << "CONSTRAIN_Z" << std::endl;
		std::cout << CGALWrapperf::computeOrientedBoundingBox(points, CGALWrapperf::CONSTRAIN_Z) << std::endl;
		std::cout << "MIN_PCA" << std::endl;
		std::cout << CGALWrapperf::computeOrientedBoundingBox(points, CGALWrapperf::MIN_PCA) << std::endl;
		std::cout << "CONSTRAIN_AXIS" << std::endl;
		std::cout << CGALWrapperf::computeOrientedBoundingBox(points, CGALWrapperf::CONSTRAIN_AXIS, vec3f(0,0,-1).getNormalized()) << std::endl;
		std::cout << "AABB_FALLBACK" << std::endl;
		std::cout << CGALWrapperf::computeOrientedBoundingBox(points, CGALWrapperf::AABB_FALLBACK) << std::endl;

		ml::Console::log() << "box test0 passed" << std::endl;
	}

	std::string name()
	{
		return "Box";
	}
private:

};
