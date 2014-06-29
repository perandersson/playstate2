#pragma once
#include "../typedefs.h"
#undef DELETE

namespace core
{
	/*!
		\brief Available keyboard keys
	*/
	class KeyboardKey
	{
	public:
		enum Enum {
			SPACE = 0, ESC, BACKSPACE, ENTER, INSERT, DELETE, HOME, END,
			PAGE_UP, PAGE_DOWN, TAB,
			LEFT_CTRL, RIGHT_CTRL,
			LEFT_SHIFT, RIGHT_SHIFT,
			LEFT_ALT, RIGHT_ALT,
			LEFT, RIGHT, UP, DOWN,
			A, B, C, D, E, F, G, H, I, J, K, L, M, N, O,
			P, Q, R, S, T, U, V, W, X, Y, Z,
			_0, _1, _2, _3, _4, _5, _6, _7, _8, _9,

			SIZE
		};
		
		/*!
			\brief Retrieves the values available in this enum

			\return A map matching name with enum value
		*/
		static std::hash_map<std::string, int32> GetValues();
	};
}
