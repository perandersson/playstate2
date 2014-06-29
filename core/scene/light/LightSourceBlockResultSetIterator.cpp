#include "LightSourceBlockResultSetIterator.h"
#include "LightSourceResultSet.h"
using namespace core;

LightSourceBlockResultSetIterator::LightSourceBlockResultSetIterator(LightSourceResultSet* resultSet)
: mResultSet(resultSet), mCurrentResultIndex(0)
{
	assert_not_null(resultSet);
	mNumResults = resultSet->GetSize();
	mLightSourceBlocks = resultSet->GetLightSourceBlocks();
}

LightSourceBlockResultSetIterator::~LightSourceBlockResultSetIterator()
{

}

LightSourceBlock* LightSourceBlockResultSetIterator::Next()
{
	if (mCurrentResultIndex >= mNumResults)
		return NULL;

	return &mLightSourceBlocks[mCurrentResultIndex++];
}
