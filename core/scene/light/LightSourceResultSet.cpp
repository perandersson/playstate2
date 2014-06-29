#include "../../Memory.h"
#include "LightSourceResultSet.h"
using namespace core;

LightSourceResultSet::LightSourceResultSet() 
: mMemoryPool(20, 5)
{
}

LightSourceResultSet::~LightSourceResultSet()
{
}

LightSourceBlock* LightSourceResultSet::Create()
{
	auto block = mMemoryPool.Allocate();
	return block;
}

uint32 LightSourceResultSet::GetSize() const
{
	return mMemoryPool.GetSize();
}

void LightSourceResultSet::Reset()
{
	mMemoryPool.Reset();
}

LightSourceResultSet::Iterator LightSourceResultSet::GetIterator()
{
	return LightSourceBlockResultSetIterator(this);
}

LightSourceBlock* LightSourceResultSet::GetLightSourceBlocks()
{
	return mMemoryPool.GetFirstElement();
}
