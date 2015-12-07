
namespace ml
{

  ColorImageR8G8B8A8 LodePNG::load(const std::string &filename)
  {
    if (!ml::util::fileExists(filename))
    {
      Console::log("LodePNG::load file not found: " + filename);
      return ColorImageR8G8B8A8();
    }
    std::vector<BYTE> image;
    UINT width, height;

    UINT error = lodepng::decode(image, width, height, filename);

    MLIB_ASSERT_STR(!error, std::string(lodepng_error_text(error)) + ": " + filename);

    ColorImageR8G8B8A8 result;

    if (!error)
    {
        result.allocate(width, height);
        memcpy(result.getPointer(), &image[0], 4 * width * height);
    }

    return result;
  }

  void LodePNG::save(const ColorImageR8G8B8A8 &image, const std::string &filename, bool saveTransparency)
  {
    const UINT pixelCount = image.getWidth()*image.getHeight();

    if (saveTransparency)
    {
        lodepng::encode(filename, (const BYTE *)image.getPointer(), image.getWidth(), image.getHeight(), LodePNGColorType::LCT_RGBA);
    }
    else
    {
        //
        // images should be saved with no transparency, which unfortunately requires us to make a copy of the bitmap data.
        //
        RGBColor *copy = new RGBColor[pixelCount];
        memcpy(copy, image.getPointer(), pixelCount * 4);
        for (UINT i = 0; i < pixelCount; i++)
            copy[i].a = 255;

        lodepng::encode(filename, (const BYTE *)copy, image.getWidth(), image.getHeight(), LodePNGColorType::LCT_RGBA);
        delete[] copy;
    }
  }

}  // namespace ml
