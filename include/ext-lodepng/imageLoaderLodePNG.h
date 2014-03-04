
class LodePNG
{
public:
	static Bitmap load(const std::string &filename);
	static void save(const Bitmap &bmp, const std::string &filename);
};
