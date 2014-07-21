#include "../../Memory.h"
#include "RenderBlockResultSet.h"
using namespace core;

RenderBlockResultSet::RenderBlockResultSet()
: RenderBlockResultSet(InitialRenderBlocksCount, RenderBlocksResizeCount)
{
}

RenderBlockResultSet::RenderBlockResultSet(uint32 initCount, uint32 resizeCount)
: mMemoryPool(InitialRenderBlocksCount, RenderBlocksResizeCount), mSortedRenderBlocks(nullptr), mSortedRenderBlocksSize(0)
{

}

RenderBlockResultSet::~RenderBlockResultSet()
{
	if (mSortedRenderBlocks != nullptr) {
		free(mSortedRenderBlocks);
		mSortedRenderBlocks = nullptr;
	}
}

RenderBlock* RenderBlockResultSet::Create(uint32 id)
{
	auto block = mMemoryPool.Allocate();
	block->id = id;
	return block;
}

void RenderBlockResultSet::Sort(IRenderBlockSorter* sorter)
{
	// Resize the sorted items container if the amount of elements has been increased
	const uint32 size = mMemoryPool.GetSize();
	if (size > mSortedRenderBlocksSize) {
		const size_t memorySize = size * sizeof(RenderBlock**);
		mSortedRenderBlocks = (RenderBlock**)realloc(mSortedRenderBlocks, memorySize);
		mSortedRenderBlocksSize = size;
	}

	RenderBlock* data = mMemoryPool.GetFirstElement();
	for (uint32 i = 0; i < size; ++i) {
		mSortedRenderBlocks[i] = &data[i];
	}

	if (mSortedRenderBlocks != nullptr)
		sorter->Sort(mSortedRenderBlocks, size);
}

uint32 RenderBlockResultSet::GetSize() const
{
	return mMemoryPool.GetSize();
}

void RenderBlockResultSet::Reset()
{
	mMemoryPool.Reset();
}

RenderBlockResultSet::Iterator RenderBlockResultSet::GetIterator()
{
	return RenderBlockResultSetIterator(this);
}

RenderBlock** RenderBlockResultSet::GetRenderBlocks()
{
	return mSortedRenderBlocks;
}
