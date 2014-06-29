#pragma once
#include "../../LinkedList.h"
#include "RenderBlockResultSet.h"
#include "../SpatialSceneNode.h"
#include "../FindQuery.h"

namespace core
{
	class RenderableFilter
	{
	public:
		enum Enum {
			GEOMETRY = BIT(1),
			TEXTURES = BIT(2),
			DEBUG_INFO = BIT(3),
		};

		static const uint32 DEFAULT = GEOMETRY | TEXTURES;
	};

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
			\brief Performs the pre-render stage for this node.

			This does not render the items onto the screen, but collects the information needed to draw it on the
			render pipeline later on.

			\param state
			\param resultSet
		*/
		virtual void PreRender(const FindQuery& state, RenderBlockResultSet* resultSet) = 0;
		
	protected:
		void UpdateModelMatrix(); 
		virtual void OnPositionChanged();
		virtual void OnRotationChanged();
		virtual void OnScaleChanged();
		virtual void OnAddedToSceneGroup();
		virtual void OnRemovedFromSceneGroup();

	protected:
		Matrix4x4 mModelMatrix;
	};
}
