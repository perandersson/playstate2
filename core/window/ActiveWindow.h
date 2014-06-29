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
		static void AddWindowClosedListener(IWindowClosedListener* listener);
		static void RemoveWindowClosedListener(IWindowClosedListener* listener);
		static void AddWindowResizedListener(IWindowResizedListener* listener);
		static void RemoveWindowResizedListener(IWindowResizedListener* listener);
		static void Alert(const std::string& title, const std::string& text, AlertType::Enum type);
		static void Alert(const std::string& title, const Exception& e, AlertType::Enum type);
	};
}
