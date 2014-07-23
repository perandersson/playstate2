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
			\brief Enables- or Disables if light sources generate static shadows from this renderable node

			\param b
		*/
		void SetStaticShadowCaster(bool b);
		
		/*!
			\brief Enables- or Disables if light sources generate dynamic shadows from this renderable node

			\param b
		*/
		void SetDynamicShadowCaster(bool b);

		/*!
			\brief
		*/
		inline bool GetStaticShadowCaster() const {
			return mStaticShadowCaster;
		}
		
		/*!
			\brief
		*/
		inline bool GetDynamicShadowCaster() const {
			return mDynamicShadowCaster;
		}
		
		/*!
			\brief
		*/
		inline bool IsShadowCaster() const {
			return mStaticShadowCaster || mDynamicShadowCaster;
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

	protected:
		bool mStaticShadowCaster;
		bool mDynamicShadowCaster;
	};
}
