class TestBox : public Test
{
public:
	void test0()
	{
		ml::Console::log() << "box test0 passed" << std::endl;
	}

	std::string name()
	{
		return "Box";
	}
private:
};