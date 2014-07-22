#pragma once
#include "WindowClosedListener.h"
#include "WindowResizedListener.h"
#include "AlertType.h"
#include "../math/Point.h"
#include "../Exception.h"

namespace core
{
	class IActiveWindow
	{
	public:
		virtual ~IActiveWindow() {}

		/*!
			\brief Check to see if the window is active or not

			\return TRUE if the window is active; FALSE otherwise
		*/
		virtual bool IsActive() const = 0;

		virtual void SetSize(const Size& size) = 0;
		virtual const Size& GetSize() const = 0;
		virtual void SetTitle(const std::string& title) = 0;
		virtual const std::string& GetTitle() const = 0;
		virtual void AddWindowClosedListener(WindowClosedListener* listener) = 0;
		virtual void RemoveWindowClosedListener(WindowClosedListener* listener) = 0;
		virtual void AddWindowResizedListener(WindowResizedListener* listener) = 0;
		virtual void RemoveWindowResizedListener(WindowResizedListener* listener) = 0;
		virtual void SetFullscreen(bool fullscreen) = 0;
		virtual void Alert(const std::string& title, const std::string& text, AlertType::Enum type) = 0;
		virtual void Alert(const std::string& title, const Exception& e, AlertType::Enum type) = 0;
	};

}