#include "../../Memory.h"
#include "RenderBlockResultSet.h"
using namespace core;

RenderBlockResultSet::RenderBlockResultSet()
: mMemoryPool(InitialRenderBlocksCount, RenderBlocksResizeCount), mSortedRenderBlocks(nullptr), mNumElements(0)
{
}

RenderBlockResultSet::~RenderBlockResultSet()
{
	free(mSortedRenderBlocks);
	mSortedRenderBlocks = nullptr;
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
	if (mNumElements < size) {
		mSortedRenderBlocks = (RenderBlock**)realloc(mSortedRenderBlocks, size * sizeof(RenderBlock**));
		mNumElements = size;
	}

	// If the pointer located to the memory has been changed then make sure to
	// re-initialize the pointers used to sort the renderblocks
	RenderBlock* data = mMemoryPool.GetFirstElement();
	for (uint32 i = 0; i < size; ++i) {
		mSortedRenderBlocks[i] = &data[i];
	}

	if (mSortedRenderBlocks != nullptr)
		sorter->Sort(mSortedRenderBlocks, mMemoryPool.GetSize());
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
