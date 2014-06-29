#pragma once
#include "RenderBlock.h"

namespace core
{
	class IRenderBlockSorter
	{
	public:
		virtual ~IRenderBlockSorter() {}

		/*!
		
		*/
		virtual void Sort(RenderBlock** renderBlocks, uint32 count) = 0;
	};
}
