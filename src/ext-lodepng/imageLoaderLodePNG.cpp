
Bitmap LodePNG::load(const String &filename)
{
	Bitmap result;

	std::vector<BYTE> image;
	UINT width, height;

	UINT error = lodepng::decode(image, width, height, filename.ptr());

	MLIB_ASSERT(!error, String(lodepng_error_text(error)) + ": " + filename);

	result.allocate(width, height);
	memcpy(result.ptr(), &image[0], 4 * width * height);

	return result;
}
