#pragma once
#include "LightSourceBlockResultSetIterator.h"
#include "../../MemoryPool.h"
#include "LightSourceBlock.h"

namespace core
{
	class LightSource;

	//
	// 
	class LightSourceResultSet
	{
	public:
		typedef LightSourceBlockResultSetIterator Iterator;
		typedef LightSourceBlock* Type;

	public:
		LightSourceResultSet();
		virtual ~LightSourceResultSet();

		/*!
			\brief Creates a new light-source block and return a pointer to it

			\param uid

			\return A pointer to a block where we can put the information needed to draw a light-source onto the scene.
		*/
		LightSourceBlock* Create(uint32 uid);
		
		/*!
			\brief Retrieves the number of LightSource blocks put into this result-set

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

	friend class LightSourceBlockResultSetIterator;
	protected:
		/*!
			\brief Retrieves a pointer to a memory block where we can put light source blocks into
		*/
		LightSourceBlock* GetLightSourceBlocks();

	private:
		MemoryPool<LightSourceBlock> mMemoryPool;
	};
}
