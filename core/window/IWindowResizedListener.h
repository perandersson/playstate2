#pragma once
#include "../math/Point.h"

namespace core
{
	/*!
		\brief 
	*/
	class IWindowResizedListener
	{
	public:
		virtual ~IWindowResizedListener() {}

		/*!
			\brief Method called when the supplied window has been resized.

			This is called at the end of each frame (during the ProcessEvents state of the Kernel).

			\param newSize
					The new window size
		*/
		virtual void OnWindowResized(const Size& newSize) = 0;
	};
}
