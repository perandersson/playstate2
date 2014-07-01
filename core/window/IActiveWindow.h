#pragma once
#include "IWindowClosedListener.h"
#include "IWindowResizedListener.h"
#include "AlertType.h"
#include "../math/Point.h"
#include "../Exception.h"

namespace core
{
	class IActiveWindow
	{
	public:
		virtual ~IActiveWindow() {}

		//
		// @return TRUE if the window is active; FALSE otherwise
		virtual bool IsActive() const = 0;

		//
		//
		virtual void SetSize(const Size& size) = 0;
		virtual const Size& GetSize() const = 0;
		virtual void SetTitle(const std::string& title) = 0;
		virtual const std::string& GetTitle() const = 0;
		virtual void AddWindowClosedListener(IWindowClosedListener* listener) = 0;
		virtual void RemoveWindowClosedListener(IWindowClosedListener* listener) = 0;
		virtual void AddWindowResizedListener(IWindowResizedListener* listener) = 0;
		virtual void RemoveWindowResizedListener(IWindowResizedListener* listener) = 0;
		virtual void SetFullscreen(bool fullscreen) = 0;
		virtual void Alert(const std::string& title, const std::string& text, AlertType::Enum type) = 0;
		virtual void Alert(const std::string& title, const Exception& e, AlertType::Enum type) = 0;
	};

}