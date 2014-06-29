#pragma once
#include "../../collision/AABB.h"
#include "../../camera/Frustum.h"
#include "ISpatialTreeVisitor.h"

namespace core
{
	class ISpatialTree
	{
	public:
		virtual ~ISpatialTree() {}

		//
		// Add a new spatial node to this tree
		//
		// @param node the node
		virtual bool Add(SpatialSceneNode* node) = 0;

		//
		// Remove the spatial node from this tree
		//
		// @param node the node we want to remove from this tree.
		virtual void Remove(SpatialSceneNode* node) = 0;

		//
		// Invalidates the supplied node. 
		//
		// @remark This usually means that the bounding-box has changed somehow - i.e. it needs 
		//	to be moved inside the tree structure.
		//
		// @param node The invalidated node
		virtual void Invalidate(SpatialSceneNode* node) = 0;

		//
		// Search for nodes based on the supplied frustum. The visitor will be called for each found item.
		//
		// @param frustum
		// @param visitor
		virtual void Find(const Frustum& frustum, ISpatialTreeVisitor* visitor) const = 0;
		//
		// Search for nodes based on the supplied boudingBox. The visitor will be called for each found item.
		//
		// @param boudingBox
		// @param visitor
		virtual void Find(const AABB& boundingBox, ISpatialTreeVisitor* visitor) const = 0;
	};
}
