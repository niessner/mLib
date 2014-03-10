class TestLodePNG : public Test
{
public:
	void test0()
	{
		using namespace ml;
		Bitmap bmp(41, 17);
		bmp.clear(RGBColor::Magenta);
		bmp(1, 1) = RGBColor::Blue;
		bmp(0, 1) = RGBColor::Red;
		bmp(6, 13) = RGBColor::Green;
		bmp(7, 2) = RGBColor::White;

		LodePNG::save(bmp, "test.png");
		Bitmap bmpReloaded = LodePNG::load("test.png");

		MLIB_ASSERT_STR(bmp == bmpReloaded, "bitmaps do not match");
		Console::log("LodePNG test done");
	}

	std::string name()
	{
		return "LodePNG";
	}
};
