#pragma once
#include "typedefs.h"

namespace core
{
	//
	// The MemoryPool base class is used for when handling massive new's and delete's withing the game engine
	template<typename T>
	class MemoryPool
	{
	public:
		typedef typename T* Type;

	public:
		//
		// Constructor
		//
		// @param initialBlocksCount 
		//			How many blocks this array contains when this instance is created.
		// @param blocksResizeCount 
		//			The amount of blocks added when the array is full.
		MemoryPool(size_t initialBlocksCount, size_t blocksResizeCount);

		//
		// Destructor
		~MemoryPool();

		//
		// Resets this result-set. This is to preseve the allocated memory.
		void Reset();

		//
		// Creates a new instance and returns it. This increases the Size with 1.
		//
		// @return Returns a free object from the memory pool. Resizes the array when the memory pool is full.
		Type Allocate();

		//
		// @return The first element in the memory pool array
		Type GetFirstElement() const;

		/*!
			\brief Retrieves the number of elements found in this memory pool
		*/
		size_t GetSize() const;

		/*!
			\brief Retrives the total abount of memory the memory pool has allocated

			\return The total memory size
		*/
		size_t GetMemorySize() const;

		/*!
			\brief Check to see if the memory pool is full and needs to be resized

			\return TRUE if this memory pool is full and needs to be resized; FALSE otherwise 
		*/
		bool IsResizeRequired() const;

	protected:
		Type mMemory;
		size_t mSize;
		size_t mNumElements;
		size_t mInitialBlocksCount;
		size_t mBlocksResizeCount;
	};

	template<typename T>
	MemoryPool<T>::MemoryPool(size_t initialBlocksCount, size_t blocksResizeCount) : mMemory(0), mSize(0), mNumElements(initialBlocksCount),
		mInitialBlocksCount(initialBlocksCount), mBlocksResizeCount(blocksResizeCount)
	{
		mMemory = (T*)malloc(initialBlocksCount * sizeof(T));
	}

	template<typename T>
	MemoryPool<T>::~MemoryPool()
	{
		free(mMemory);
		mMemory = NULL;
		mSize = 0;
		mNumElements = 0;
	}

	template<typename T>
	void MemoryPool<T>::Reset()
	{
		mSize = 0;
	}

	template<typename T>
	T* MemoryPool<T>::Allocate()
	{
		if (IsResizeRequired()) {
			mNumElements += mBlocksResizeCount;
			mMemory = (T*)realloc(mMemory, GetMemorySize());
		}

		const size_t index = mSize++;
		T* ref = &mMemory[index];
		memset(ref, 0, sizeof(T));
		return ref;
	}

	template<typename T>
	T* MemoryPool<T>::GetFirstElement() const
	{
		return mMemory;
	}

	template<typename T>
	size_t MemoryPool<T>::GetSize() const
	{
		return mSize;
	}

	template<typename T>
	bool MemoryPool<T>::IsResizeRequired() const
	{
		return mSize >= mNumElements;
	}

	template<typename T>
	size_t MemoryPool<T>::GetMemorySize() const
	{
		return mNumElements * sizeof(T);
	}
}
