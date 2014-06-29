#pragma once
#include "RenderBlockResultSet.h"
#include "IRenderBlockSorter.h"

namespace core
{
	/*!
		\brief Default RenderBlock result-set sorter

		A quick-sort implementation used by the engine to sort render blocks. Useful for making sore 
		we are rendering the same items in succession instead of drawing them on the 
		screen randomly (random => many state changes => slow)
	*/
	class DefaultRenderBlockResultSetSorter : public IRenderBlockSorter
	{
	public:
		DefaultRenderBlockResultSetSorter();
		virtual ~DefaultRenderBlockResultSetSorter();

		virtual void Sort(RenderBlock** renderBlocks, uint32 count);

	private:
		void QuickSort(RenderBlock** items, int32 left, int32 right);
	};
}
