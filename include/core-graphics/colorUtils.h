#ifndef INCLUDE_CORE_GRAPHICS_COLORUTILS_H_
#define INCLUDE_CORE_GRAPHICS_COLORUTILS_H_

namespace ml
{

class ColorUtils
{
 public:
    /**
    * Converts an HSL color value to RGB. Conversion formula
    * adapted from http://en.wikipedia.org/wiki/HSL_color_space.
    * Assumes rgb and hsl values are in [0, 1].
    */
    template <typename RGB, typename T>
    static RGB hslToRgb(const T& hsl)
    {
        auto test = hsl[0];
        static_assert(static_cast<bool>(std::is_same<decltype(test),float>::value), "hslToRgb assumes float 0-1 range");

        // Helper for hsl to rgb conversion
        auto hue2rgb = [] (float p, float q, float t)
        {
            if (t < 0.0f) { t += 1.0f; }
            if (t > 1.0f) { t -= 1.0f; }
            if (t < 1.0f / 6.0f) { return p + (q - p) * 6.0f * t; }
            if (t < 1.0f / 2.0f) { return q; }
            if (t < 2.0f / 3.0f) { return p + (q - p) * (2.0f / 3.0f - t) * 6.0f; }
            return p;
        };
        RGB rgb;
        float h = hsl[0], s = hsl[1], l = hsl[2];
        if (s == 0)
        {
            rgb[0] = rgb[1] = rgb[2] = l;  // achromatic
        } else {
            float q = l < 0.5f ? l * (1 + s) : l + s - l * s;
            float p = 2.0f * l - q;
            rgb[0] = hue2rgb(p, q, h + 1.0f / 3);
            rgb[1] = hue2rgb(p, q, h);
            rgb[2] = hue2rgb(p, q, h - 1.0f / 3);
        }
        return rgb;
    }

    // Return a nice simple id-based color based on cycling through hsv space
    template<typename RGBA>
    static RGBA colorById(const int id)
    {
        float h = std::fmod(-3.88f * id, 2.0f * Math::PIf);
        if (h < 0) { h += 2.0f * Math::PIf; }
        h /= 2.0f * Math::PIf;

        float hsl[4] = {h, 0.6f + 0.4f * std::sin(0.42f * id), 0.5f};

        RGBA rgba = hslToRgb<RGBA>(hsl);
        rgba[3] = 1;
        return rgba;
    }
};

}  // namespace ml

#endif  // INCLUDE_CORE_GRAPHICS_COLORUTILS_H_
