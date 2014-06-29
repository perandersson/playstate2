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
			\brief Method called whenever we are trying to collect light source information to be drawn

			\param query
			\param _out_resultSet
		*/
		virtual void CollectLightBlocks(const FindQuery& query, LightSourceResultSet* _out_resultSet) const = 0;

		virtual void OnAddedToSceneGroup();
		virtual void OnRemovedFromSceneGroup();
	};
}
