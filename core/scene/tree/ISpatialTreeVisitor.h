#pragma once

namespace core
{
	class SpatialSceneNode;

	class ISpatialTreeVisitor
	{
	public:
		virtual ~ISpatialTreeVisitor() {}

		//
		// Method invoked when a spatial node has been found in the tree
		//
		// @param node The found node.
		virtual void Visit(SpatialSceneNode* node) = 0;
	};
}
