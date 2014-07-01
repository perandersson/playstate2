#include "RenderBlockResultSetIterator.h"
#include "RenderBlockResultSet.h"
using namespace core;

RenderBlockResultSetIterator::RenderBlockResultSetIterator(RenderBlockResultSet* resultSet)
: mCurrentResultIndex(0)
{
	assert_not_null(resultSet);
	mNumResults = resultSet->GetSize();
	mRenderBlocks = resultSet->GetRenderBlocks();
}

RenderBlockResultSetIterator::~RenderBlockResultSetIterator()
{

}

RenderBlock* RenderBlockResultSetIterator::Next()
{
	if (mCurrentResultIndex >= mNumResults)
		return NULL;

	return mRenderBlocks[mCurrentResultIndex++];
}
