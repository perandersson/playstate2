#pragma once
#include "../typedefs.h"
#include "ResourceData.h"
#include "ResourceObject.h"
#include <atomic>

namespace core
{
	template<class T>
	class Resource
	{
	public:
		Resource() : mResourceData(nullptr) {
		}

		Resource(ResourceData* data) : mResourceData(data) {
			assert_not_null(data);
			ResourceObject* ptr1 = mResourceData->resource.load();
			if (dynamic_cast<T*>(ptr1) == nullptr) {
				assert_not_implemented();
			}
		}

		Resource(const Resource<T>& rhs) : mResourceData(const_cast<ResourceData*>(rhs.mResourceData)) {
		}

		template<class U>
		Resource(const Resource<U>& rhs) : mResourceData(nullptr) {
			if (rhs.IsType<T>()) {
				ResourceData* data = const_cast<Resource<U>&>(rhs).GetResourceData();
				mResourceData = data;
			}
		}

		~Resource() {
			mResourceData = nullptr;
		}

		inline void operator = (const Resource<T>& rhs) {
			mResourceData = rhs.mResourceData;
		}

		template<class U>
		inline void operator =  (const Resource<U>& rhs) {
			if (rhs.IsType<T>()) {
				ResourceData* data = const_cast<Resource<U>&>(rhs).GetResourceData();
				mResourceData = data;
			}
		}

		template<class U> 
		inline bool IsType() const {
			if (mResourceData == nullptr)
				return false;

			return dynamic_cast<U*>(mResourceData->resource.load()) != nullptr;
		}

		inline bool IsType() {
			return true;
		}

		inline T* operator -> () {
			assert_not_null(mResourceData);
			return static_cast<T*>(mResourceData->resource.load());
		}

		inline const T* operator -> () const {
			assert_not_null(mResourceData);
			return static_cast<const T*>(mResourceData->resource.load());
		}

		inline bool IsDefault() const {
			if (mResourceData == nullptr)
				return false;

			const ResourceData* ptr1 = mResourceData->resource.load();
			const ResourceData* ptr2 = mResourceData->defaultResource;
			return ptr1 == ptr2;
		}

		inline const T* Get() const {
			if (mResourceData == nullptr)
				return nullptr;

			return static_cast<T*>(mResourceData->resource.load());
		}

		inline T* Get() {
			if (mResourceData == nullptr)
				return nullptr;

			return static_cast<T*>(mResourceData->resource.load());
		}

		inline ResourceData* GetResourceData() {
			return mResourceData;
		}

		inline const ResourceData* GetResourceData() const {
			return mResourceData;
		}

		inline bool IsNull() const {
			return mResourceData == nullptr;
		}

		inline bool IsNotNull() const {
			return mResourceData != nullptr;
		}

	protected:
		ResourceData* mResourceData;
	};

}