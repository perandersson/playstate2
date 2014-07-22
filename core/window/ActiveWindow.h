#pragma once
#include "IActiveWindow.h"

namespace core
{
	class ActiveWindow
	{
	public:
		static bool IsActive();
		static void SetSize(const Size& size);
		static const Size& GetSize();
		static void SetTitle(const std::string& title);
		static const std::string& GetTitle();
		static void AddWindowClosedListener(WindowClosedListener* listener);
		static void RemoveWindowClosedListener(WindowClosedListener* listener);
		static void AddWindowResizedListener(WindowResizedListener* listener);
		static void RemoveWindowResizedListener(WindowResizedListener* listener);
		static void SetFullscreen(bool fullscreen);
		static void Alert(const std::string& title, const std::string& text, AlertType::Enum type);
		static void Alert(const std::string& title, const Exception& e, AlertType::Enum type);
	};
}
