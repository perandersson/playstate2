#include "../Memory.h"
#include "ActiveWindow_script.h"
using namespace core;

int ActiveWindow_GetWidth(struct lua_State* L)
{
	const Size& size = ActiveWindow::GetSize();
	return ScriptUtils::PushInt32(L, size.width);
}

int ActiveWindow_GetHeight(struct lua_State* L)
{
	const Size& size = ActiveWindow::GetSize();
	return ScriptUtils::PushInt32(L, size.height);
}

int ActiveWindow_GetSize(struct lua_State* L)
{
	const Size& size = ActiveWindow::GetSize();
	return ScriptUtils::PushPoint(L, size);
}
