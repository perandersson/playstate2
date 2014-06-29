#include "../Memory.h"
#include "KeyboardKey.h"
using namespace core;

std::hash_map<std::string, int32> KeyboardKey::GetValues()
{
	std::hash_map<std::string, int32> enums; 
	ENUM_STRING(enums, KeyboardKey, SPACE);
	ENUM_STRING(enums, KeyboardKey, ESC);
	ENUM_STRING(enums, KeyboardKey, BACKSPACE);
	ENUM_STRING(enums, KeyboardKey, ENTER);
	ENUM_STRING(enums, KeyboardKey, INSERT);
	ENUM_STRING(enums, KeyboardKey, DELETE);
	ENUM_STRING(enums, KeyboardKey, HOME);
	ENUM_STRING(enums, KeyboardKey, END);
	ENUM_STRING(enums, KeyboardKey, PAGE_UP);
	ENUM_STRING(enums, KeyboardKey, PAGE_DOWN);
	ENUM_STRING(enums, KeyboardKey, TAB);
	ENUM_STRING(enums, KeyboardKey, LEFT_CTRL);
	ENUM_STRING(enums, KeyboardKey, RIGHT_CTRL);
	ENUM_STRING(enums, KeyboardKey, LEFT_SHIFT);
	ENUM_STRING(enums, KeyboardKey, RIGHT_SHIFT);
	ENUM_STRING(enums, KeyboardKey, LEFT_ALT);
	ENUM_STRING(enums, KeyboardKey, RIGHT_ALT);
	ENUM_STRING(enums, KeyboardKey, LEFT);
	ENUM_STRING(enums, KeyboardKey, RIGHT);
	ENUM_STRING(enums, KeyboardKey, UP);
	ENUM_STRING(enums, KeyboardKey, DOWN);
	ENUM_STRING(enums, KeyboardKey, A);
	ENUM_STRING(enums, KeyboardKey, B);
	ENUM_STRING(enums, KeyboardKey, C);
	ENUM_STRING(enums, KeyboardKey, D);
	ENUM_STRING(enums, KeyboardKey, E);
	ENUM_STRING(enums, KeyboardKey, F);
	ENUM_STRING(enums, KeyboardKey, G);
	ENUM_STRING(enums, KeyboardKey, H);
	ENUM_STRING(enums, KeyboardKey, I);
	ENUM_STRING(enums, KeyboardKey, J);
	ENUM_STRING(enums, KeyboardKey, K);
	ENUM_STRING(enums, KeyboardKey, L);
	ENUM_STRING(enums, KeyboardKey, M);
	ENUM_STRING(enums, KeyboardKey, N);
	ENUM_STRING(enums, KeyboardKey, O);
	ENUM_STRING(enums, KeyboardKey, P);
	ENUM_STRING(enums, KeyboardKey, Q);
	ENUM_STRING(enums, KeyboardKey, R);
	ENUM_STRING(enums, KeyboardKey, S);
	ENUM_STRING(enums, KeyboardKey, T);
	ENUM_STRING(enums, KeyboardKey, U);
	ENUM_STRING(enums, KeyboardKey, V);
	ENUM_STRING(enums, KeyboardKey, W);
	ENUM_STRING(enums, KeyboardKey, X);
	ENUM_STRING(enums, KeyboardKey, Y);
	ENUM_STRING(enums, KeyboardKey, Z);
	ENUM_STRING(enums, KeyboardKey, _0);
	ENUM_STRING(enums, KeyboardKey, _1);
	ENUM_STRING(enums, KeyboardKey, _2);
	ENUM_STRING(enums, KeyboardKey, _3);
	ENUM_STRING(enums, KeyboardKey, _4);
	ENUM_STRING(enums, KeyboardKey, _5);
	ENUM_STRING(enums, KeyboardKey, _6);
	ENUM_STRING(enums, KeyboardKey, _7);
	ENUM_STRING(enums, KeyboardKey, _8);
	ENUM_STRING(enums, KeyboardKey, _9);
	return enums;
}
