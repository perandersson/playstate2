#pragma once
#include "../LinkedList.h"

namespace core
{
	/*!
		\brief Inherit from this class if you want to receive events whenever the active window is closed.
	*/
	class WindowClosedListener
	{
	public:
		LinkedListLink<WindowClosedListener> WindowClosedListenerLink;

	public:
		virtual ~WindowClosedListener() {}

		/*!
			\brief Method called when the supplied window is closing
		*/
		virtual void OnWindowClosing() = 0;
	};
}
