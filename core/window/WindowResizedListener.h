#pragma once
#include "../LinkedList.h"
#include "../math/Point.h"

namespace core
{
	/*!
		\brief Inerit from this class if you want to receive events whenever the active window is resized
	*/
	class WindowResizedListener
	{
	public:
		LinkedListLink<WindowResizedListener> WindowResizedListenerLink;

	public:
		virtual ~WindowResizedListener() {}

		/*!
			\brief Method called when the supplied window has been resized.

			This is called at the end of the frame (during the ProcessEvents state of the Kernel).

			\param newSize
					The new window size
			\return TRUE if the resize is acceptable; FALSE if we want to return to the previous window size
		*/
		virtual bool OnWindowResized(const Size& newSize) = 0;
	};
}
