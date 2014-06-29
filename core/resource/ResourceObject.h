#pragma once
#include "../typedefs.h"

namespace core
{
	class ResourceObject
	{
	protected:
		ResourceObject();

	public:
		virtual ~ResourceObject();

		//
		// @return A unique object ID for this instance
		inline uint32 GetUID() const {
			return mUID;
		}

	private:
		uint32 mUID;
	};
}
