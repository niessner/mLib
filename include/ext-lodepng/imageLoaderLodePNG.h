
typedef Grid<RGBColor> Bitmap;

class LodePNG
{
public:
	static Bitmap load(const String &filename);
	static void save(const Bitmap &bmp, const String &filename);
};
