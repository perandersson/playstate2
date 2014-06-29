#include "../Memory.h"
#include "Color.h"
using namespace core;

const Color Color::WHITE(1.0f, 1.0f, 1.0f, 1.0f);
const Color Color::BLACK(0.0f, 0.0f, 0.0f, 1.0f);
const Color Color::NOTHING(0.0f, 0.0f, 0.0f, 0.0f);

Color::Color() : r(0), g(0), b(0), a(0)
{
}

Color::Color(float32 _c) : r(_c), g(_c), b(_c), a(_c)
{
}

Color::Color(float32 _r, float32 _g, float32 _b) : r(_r), g(_g), b(_b), a(1)
{
}

Color::Color(float32 _r, float32 _g, float32 _b, float32 _a) : r(_r), g(_g), b(_b), a(_a)
{
}

Color::Color(const Color& c) : r(c.r), g(c.g), b(c.b), a(c.a)
{
}

Color Color::HexToRGB(const std::string& hex)
{
	std::string value;
	if (hex[0] == '#')
		value += &hex[1];
	else
		value += hex;

	if (value.length() == 2) {
		std::string replicateValue = value;
		value += replicateValue;
		value += replicateValue;
		value += replicateValue;
	}

	value = std::string("0x") + value;
	int32 hexbits = 0;
	sscanf(value.c_str(), "%x", &hexbits);
	return Color(
		((hexbits >> 16) & 0xFF) / 255.f,
		((hexbits >> 8) & 0xFF) / 255.f,
		((hexbits >> 0) & 0xFF) / 255.f
		);
}
