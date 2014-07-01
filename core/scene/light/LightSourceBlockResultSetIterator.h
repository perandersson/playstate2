#pragma once
#include "../../typedefs.h"
#include "LightSourceBlock.h"

namespace core
{
	class LightSourceResultSet;

	/*!
		\brief
	*/
	class LightSourceBlockResultSetIterator
	{
	public:
		LightSourceBlockResultSetIterator(LightSourceResultSet* resultSet);
		~LightSourceBlockResultSetIterator();

		/*!
			\brief Retrieves the next render block
		*/
		LightSourceBlock* Next();

	private:
		uint32 mNumResults;
		uint32 mCurrentResultIndex;
		LightSourceBlock* mLightSourceBlocks;
	};
}
