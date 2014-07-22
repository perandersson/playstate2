#pragma once
#include "../SpatialSceneNode.h"
#include "LightSourceResultSet.h"

namespace core
{
	class LightSource : public SpatialSceneNode
	{
		DECLARE_SCRIPT_OBJECT(LightSource);

	public:
		LinkedListLink<LightSource> LightSourceLink;

	public:
		LightSource();
		virtual ~LightSource();

		/*!
			\brief Retrieves a unique ID for this light source
		*/
		inline uint32 GetUID() const {
			return mUID;
		}

		/*!
			\brief Method called whenever we are trying to collect light source information to be drawn

			\param query
			\param _out_resultSet
		*/
		virtual void CollectLightBlocks(const FindQuery& query, LightSourceResultSet* _out_resultSet) const = 0;

		virtual void OnAddedToSceneGroup();
		virtual void OnRemovedFromSceneGroup();

	private:
		uint32 mUID;
	};
}
