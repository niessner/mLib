class TestLodePNG : public Test
{
public:
	void test0()
	{
		Bitmap bmp = LodePNG::load("test.png");
		int a = 5;
	}

	String name()
	{
		return "LodePNG";
	}
};
