
Bitmap LodePNG::load(const String &filename)
{
	Bitmap result;

	std::vector<BYTE> image;
	UINT width, height;

	UINT error = lodepng::decode(image, width, height, filename.ptr());

	MLIB_ASSERT(!error, String(lodepng_error_text(error)) + ": " + filename);

	result.allocate(height, width);
	memcpy(result.ptr(), &image[0], 4 * width * height);

	return result;
}

void LodePNG::save(const Bitmap &bmp, const String &filename)
{
	const UINT pixelCount = bmp.rows() * bmp.cols();
	
	//
	// images should be saved with no transparency, which unfortunately requires us to make a copy of the bitmap data.
	//
	RGBColor *copy = new RGBColor[pixelCount];
	memcpy(copy, bmp.ptr(), pixelCount * 4);
	for(UINT i = 0; i < pixelCount; i++)
		copy[i].a = 255;

	lodepng::encode(std::string(filename.ptr()), (const BYTE *)copy, bmp.cols(), bmp.rows(), LodePNGColorType::LCT_RGBA);
	delete[] copy;
}