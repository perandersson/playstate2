#include "../Memory.h"
#include "SpatialSceneNode.h"
#include "tree/ISpatialTree.h"
using namespace core;

SpatialSceneNode::SpatialSceneNode()
: SceneNode(), 
mSpatialTree(nullptr)
{

}

SpatialSceneNode::~SpatialSceneNode()
{

}

void SpatialSceneNode::SetSpatialTree(ISpatialTree* tree)
{
	mSpatialTree = tree;
}

void SpatialSceneNode::SetBoundingBox(const AABB& boundingBox)
{
	mBoundingBox = boundingBox;
	mBoundingBox.SetPositionRotationScale(mAbsolutePosition, mAbsoluteRotation, mAbsoluteScale);
	if (mSpatialTree != nullptr)
		mSpatialTree->Invalidate(this);
}

void SpatialSceneNode::SetBoundingBox(const AABB& boundingBox, const Vector3& position, const Vector3& rotation, const Vector3& scale)
{
	mBoundingBox = boundingBox;
	mBoundingBox.SetPositionRotationScale(position, rotation, scale);
	if (mSpatialTree != nullptr)
		mSpatialTree->Invalidate(this);
}

void SpatialSceneNode::OnPositionChanged()
{
	mBoundingBox.SetPositionRotationScale(mAbsolutePosition, mAbsoluteRotation, mAbsoluteScale);
	if (mSpatialTree != nullptr)
		mSpatialTree->Invalidate(this);
}

void SpatialSceneNode::OnRotationChanged()
{
	mBoundingBox.SetPositionRotationScale(mAbsolutePosition, mAbsoluteRotation, mAbsoluteScale);
	if (mSpatialTree != nullptr)
		mSpatialTree->Invalidate(this);
}

void SpatialSceneNode::OnScaleChanged()
{
	mBoundingBox.SetPositionRotationScale(mAbsolutePosition, mAbsoluteRotation, mAbsoluteScale);
	if (mSpatialTree != nullptr)
		mSpatialTree->Invalidate(this);
}
