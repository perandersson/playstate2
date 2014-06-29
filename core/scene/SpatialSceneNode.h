#pragma once
#include "SceneNode.h"
#include "../collision/AABB.h"
#include "../LinkedList.h"

namespace core
{
	class ISpatialTree;

	/*!
		\brief Class representing a scene node located inside a spatial tree 
		
		Nodes that require spatial tree's are usually those that require collision detection of some kind, like renderable objects and lights.
	*/
	class SpatialSceneNode : public SceneNode
	{
	public:
		LinkedListLink<SpatialSceneNode> SpatialSceneNodeLink;
		DECLARE_SCRIPT_OBJECT(SpatialSceneNode);

	public:
		SpatialSceneNode();
		virtual ~SpatialSceneNode();

		/*!
			\brief Set the spatial tree associated with this node

			\param tree
						The spatial tree this node is associated with
		*/
		void SetSpatialTree(ISpatialTree* tree);

		/*!
			\brief Retrieves the tree associated with this node

			\return The tree node; nullptr is this node isn't attached to any spatial tree
		*/
		inline ISpatialTree* GetSpatialTree() {
			return mSpatialTree;
		}

		/*!
			\brief Retrieves the bounding box for this node. 
			
			\remark The bounding box is positioned in relative to the scene groups coordinates

			\return The bounding box for this node
		*/
		inline const AABB& GetBoundingBox() const {
			return mBoundingBox;
		}

		/*!
			\brief Set the bounding box for this node

			\param boundingBox
						The new bounding box
		*/
		void SetBoundingBox(const AABB& boundingBox);

		/*!
			\brief Set a bounding box for this node at the supplied coordinates

			\param boundingBox
						The new bounding box
			\param position
						The position where the bounding box should be located (relative to the scene groups position)
			\param rotation
			\param scale
		*/
		void SetBoundingBox(const AABB& boundingBox, const Vector3& position, const Vector3& rotation, const Vector3& scale);

		virtual void OnPositionChanged();
		virtual void OnRotationChanged();
		virtual void OnScaleChanged();

	private:
		AABB mBoundingBox;
		ISpatialTree* mSpatialTree;
	};
}
