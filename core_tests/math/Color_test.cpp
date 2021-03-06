#include <core/math/Color.h>
#include "../test/Test.h"
using namespace core;

TEST_SUITE(Color)
{
	UNIT_TEST(HexToRGB_White_Color)
	{
		const Color unitToTest = Color::HexToRGB("#FFFFFF");
		ASSERT_EQUALS(unitToTest, Color::WHITE);
	}

	UNIT_TEST(HexToRGB_Black_Color)
	{
		const Color unitToTest = Color::HexToRGB("#000000");
		ASSERT_EQUALS(unitToTest, Color::BLACK);
	}

	UNIT_TEST(HexToRGB_One_Element)
	{
		// Converts into this (acording to Google Chrome): rgb(238, 238, 238);
		const Color unitToTest = Color::HexToRGB("#EE");
		const Color expected(238 / 255.0f, 238 / 255.0f, 238 / 255.f);
		ASSERT_EQUALS(unitToTest, expected);
	}
}
