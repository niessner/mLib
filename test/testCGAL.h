
class TestCGAL : public Test {
public:
	void test0() 
	{
	
		std::vector<vec3f> points;
		points.push_back(vec3f(0, 0, 0));
		points.push_back(vec3f(0, 1, 0));
		points.push_back(vec3f(1, 1, 0));
		points.push_back(vec3f(1, 0, 0));
		points.push_back(vec3f(0.5f, 0.5f, 0));

		std::vector<vec3f> convexHull = CGALWrapper<float>::convexHull(points);

		ml::Console::log() << "CGAL test0 passed" << std::endl;
	}


	std::string name() {
		return "cgal";
	}

private:
};