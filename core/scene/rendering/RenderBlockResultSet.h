#pragma once
#include "../../MemoryPool.h"
#include "RenderBlock.h"
#include "../../rendering/GeometryBuilder.h"
#include "IRenderBlockSorter.h"
#include "RenderBlockResultSetIterator.h"

namespace core
{
	/* The initial render block size used by the result-set */
	static const uint32 InitialRenderBlocksCount = 1000;

	/* How much the memory pool will resize */
	static const uint32 RenderBlocksResizeCount = 100;

	/*!
		\brief 
	*/
	class RenderBlockResultSet
	{
	public:
		typedef RenderBlockResultSetIterator Iterator;
		typedef RenderBlock* Type;

	public:
		RenderBlockResultSet();
		virtual ~RenderBlockResultSet();

		/*!
			\brief Creates a new render block instance and returns the pointer to it.

			\param id
					A unique id for render block.
			\return A pointer to a block where we can put the information needed to draw an item to the scene.
		*/
		RenderBlock* Create(uint32 id);

		/*!
			\brief Sorts this resultset using the supplied array sorter

			\param sorter
						An interface used to sort the render blocks contained by this result-set
		*/
		void Sort(IRenderBlockSorter* sorter);

		/*!
			\brief Retrieves the number of render blocks put into this result-set

			\return 
		*/
		uint32 GetSize() const;

		/*!
			\brief Reset this result-set so that you can reuse it
		*/
		void Reset();

		/*!
			\brief Retrieves an iterator for this result-set.

			Useful if you want to iterate over all this result-set's elements
		*/
		Iterator GetIterator();

	friend class RenderBlockResultSetIterator;
	protected:
		/*!
			\brief Retrieves a pointer to a memory block where we can put sorted render blocks into
		*/
		RenderBlock** GetRenderBlocks();
		
	private:
		MemoryPool<RenderBlock> mMemoryPool;
		RenderBlock** mSortedRenderBlocks;
		uint32 mSortedRenderBlocksSize;
	};
}
