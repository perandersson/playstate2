#pragma once
#include "../../typedefs.h"
#include "RenderBlock.h"

namespace core
{
	class RenderBlockResultSet;

	/*!
		\brief
	*/
	class RenderBlockResultSetIterator
	{
	public:
		RenderBlockResultSetIterator(RenderBlockResultSet* resultSet);
		~RenderBlockResultSetIterator();

		/*!
			\brief Retrieves the next render block
		*/
		RenderBlock* Next();

	private:
		RenderBlockResultSet* mResultSet;
		uint32 mNumResults;
		uint32 mCurrentResultIndex;
		RenderBlock** mRenderBlocks;
	};
}
