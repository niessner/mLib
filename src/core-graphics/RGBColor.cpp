
const RGBColor RGBColor::White(255, 255, 255);
const RGBColor RGBColor::Gray(128, 128, 128);
const RGBColor RGBColor::Red(255, 0, 0);
const RGBColor RGBColor::Green(0, 255, 0);
const RGBColor RGBColor::Blue(0, 0, 255);
const RGBColor RGBColor::Yellow(255, 255, 0);
const RGBColor RGBColor::Magenta(255, 0, 255);
const RGBColor RGBColor::Cyan(0, 255, 255);
const RGBColor RGBColor::Black(0, 0, 0);
const RGBColor RGBColor::Orange(255, 91, 0);
const RGBColor RGBColor::Purple(111, 49, 152);

RGBColor::RGBColor(const String &hex)
{
	MLIB_ASSERT(hex.length() >= 6, "bad rgb hex code");
	UINT offset = hex.length() - 6;
	const char* carray = hex.ptr();
	char channel[3];
	Vector<BYTE> color(3);

	for (UINT c = 0; c < 3; c++)
	{
		channel[0] = carray[offset++];
		channel[1] = carray[offset++];
		channel[2] = '\0';

		char first = channel[0];
		char second = channel[1];

		long int a = strtol(channel, NULL, 16);
		color[c] = (byte) strtol(channel, NULL, 16);
	}
	r = color[0];
	g = color[1];
	b = color[2];
}

RGBColor::RGBColor(const vec3f &V)
{
    r = Utility::boundToByte(V.x * 255.0f);
    g = Utility::boundToByte(V.y * 255.0f);
    b = Utility::boundToByte(V.z * 255.0f);
	a = 0;
}

RGBColor::RGBColor(const vec4f &V)
{
    r = Utility::boundToByte(V.x * 255.0f);
    g = Utility::boundToByte(V.y * 255.0f);
    b = Utility::boundToByte(V.z * 255.0f);
    a = Utility::boundToByte(V.w * 255.0f);
}

RGBColor RGBColor::interpolate(RGBColor L, RGBColor R, float s)
{
    return RGBColor(Utility::boundToByte(int(int(L.r) + s * float(int(R.r) - int(L.r)))),
                    Utility::boundToByte(int(int(L.g) + s * float(int(R.g) - int(L.g)))),
                    Utility::boundToByte(int(int(L.b) + s * float(int(R.b) - int(L.b)))),
                    Utility::boundToByte(int(int(L.a) + s * float(int(R.a) - int(L.a)))));
}
