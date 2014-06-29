#pragma once
#include "../typedefs.h"

namespace core
{
	struct Color
	{
		union
		{
			struct
			{
				float32 red;
				float32 green;
				float32 blue;
				float32 alpha;
			};

			struct
			{
				float32 r;
				float32 g;
				float32 b;
				float32 a;
			};

			float32 _array[4];
		};

		Color();
		Color(float32 _c);
		Color(float32 _r, float32 _g, float32 _b);
		Color(float32 _r, float32 _g, float32 _b, float32 _a);
		Color(const Color& _c);
		
		inline bool operator == (const Color& rhs) const {
			return FLOAT_EQUALS(rhs.r, r) && FLOAT_EQUALS(rhs.g, g) && FLOAT_EQUALS(rhs.b, b) && FLOAT_EQUALS(rhs.a, a);
		}
		
		inline bool operator != (const Color& rhs) const {
			return FLOAT_NEQUALS(rhs.r, r) || FLOAT_NEQUALS(rhs.g, g) || FLOAT_NEQUALS(rhs.b, b) || FLOAT_NEQUALS(rhs.a, a);
		}

		inline Color& operator = (const Color &c) {
			r = c.r;
			g = c.g;
			b = c.b;
			a = c.a;
			return *this;
		}

		// White color
		static const Color WHITE;

		// Black color
		static const Color BLACK;

		// Nothing
		static const Color NOTHING;

		//
		// Converts a supplied hex value into a color usable by the game engine
		//
		// @param hex 
		//			The hex decimal html color value. It can take the following combinations: "FF", "#FF", "AABBCC", "#AABBCC"
		// @return 
		static Color HexToRGB(const std::string& hex);
	};
}
