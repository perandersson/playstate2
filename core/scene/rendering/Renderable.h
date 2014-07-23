#pragma once
#include "../../LinkedList.h"
#include "RenderBlockResultSet.h"
#include "../SpatialSceneNode.h"
#include "../FindQuery.h"

namespace core
{
	/*!
		\brief Base class for renderable scene nodes

		A renderable scene node are able to generate RenderBlock, which is used by the rendering pipeline to draw the vertices onto the scene.

		Renderable instances are static shadow casters (They will generate a shadow map only once) by default, but not dynamic shadow casters.
	*/
	class Renderable : public SpatialSceneNode
	{
	public:
		LinkedListLink<Renderable> RenderableLink;
		DECLARE_SCRIPT_OBJECT(Renderable);

	public:
		Renderable();
		virtual ~Renderable();

		/*!
			\brief Retrieves the model matrix for this renderable node
		*/
		inline const Matrix4x4& GetModelMatrix() const {
			return mModelMatrix;
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
			\brief Performs the pre-render stage for this node.

			This does not render the items onto the screen, but collects the information needed to draw it on the
			render pipeline later on.

			\param state
			\param resultSet
		*/
		virtual void PreRender(const FindQuery& query, RenderBlockResultSet* resultSet) = 0;
		
	protected:
		void UpdateModelMatrix(); 
		virtual void OnPositionChanged();
		virtual void OnRotationChanged();
		virtual void OnScaleChanged();
		virtual void OnAddedToSceneGroup();
		virtual void OnRemovedFromSceneGroup();

	protected:
		Matrix4x4 mModelMatrix;
		bool mStaticShadowCaster;
		bool mDynamicShadowCaster;
	};
}
