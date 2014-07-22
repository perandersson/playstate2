#pragma once
#include "SceneGroup.h"
#include "LinkedListSceneGroup.h"

namespace core
{
	/*!
		\brief Inherit from this class if you want to receive events whenever groups are added and removed

		It is assumed that if the group is not unlinked from the Scene before the scene is removed then it
		should be deleted by the Scene.
	*/
	class SceneGroupListener
	{
	public:
		LinkedListLink<SceneGroupListener> SceneGroupListenerLink;

	public:
		virtual ~SceneGroupListener() {}

		/*!
			\brief Method called whenever the supplied scene group is added

			\param group
		*/
		virtual void OnSceneGroupAdded(SceneGroup* group) = 0;
		
		/*!
			\brief Method called whenever the supplied scene group is removed

			\param group
		*/
		virtual void OnSceneGroupRemoved(SceneGroup* group) = 0;
	};
}
