#pragma once
#include "../LinkedList.h"
#include "SceneNode.h"
#include "../script/ScriptObject.h"

namespace core
{
	class Updatable;
	class Tickable;
	class Renderable;

	class Component : public ScriptObject
	{
		DECLARE_SCRIPT_OBJECT(Component);

	public:
		LinkedListLink<Component> ComponentLink;

	public:
		Component();
		virtual ~Component();

		//
		// Sets this component's ID
		void SetComponentID(const std::string& id);

		//
		// Sets the component type mask
		void SetTypeMask(typemask typeMask);

		//
		// @return This component's ID
		inline const std::string& GetComponetID() const {
			return mComponentID;
		}

		/*!
			\brief Retrieves the owner scene node for this component

			\return The scene node; nullptr if not attached to a scene node
		*/
		inline SceneNode* GetSceneNode() const {
			return mSceneNode;
		}

		/*!
			\brief Retrieves the owner scene group for this component

			\return The scene group; nullptr if not attached to a scene group
		*/
		inline SceneGroup* GetSceneGroup() const {
			return mSceneGroup;
		}

		//
		// @return This componenents type mask
		inline typemask GetTypeMask() const {
			return mTypeMask;
		}

		//
		// @return The bounding box
		inline const AABB& GetBoundingBox() const {
			return mBoundingBox;
		}

		void SetBoundingBox(const AABB& boundingBox);
		void SetBoundingBox(const AABB& boundingBox, const Vector3& position, const Vector3& rotation, const Vector3& scale);

		//
		// Method used by the game engine to retrieve the object responsible for updating this component.
		// If no update functionality is neccessary then return nullptr.
		//
		// @return A pointer to the updatable instance that this component implements
		virtual Updatable* GetUpdatable() { return nullptr; }

		//
		// Method used by the game engine to retrieve the object responsible for performing tick-updates on this component.
		// If no tick functionality is neccessary then return nullptr.
		//
		// @return A pointer to the tickable instance that this component implements
		virtual Tickable* GetTickable() { return nullptr; }

		//
		// @return A pointer to the renderable instance that this component implements
		virtual Renderable* GetRenderable() { return nullptr; }

		/*!
			\brief Method invoked whenever this component is added to the supplied node
		*/
		void ComponentAddedToNode(SceneNode* node);

		/*!
			\brief Method invoked whenever this node is added to the supplied group
		*/
		void ComponentAddedToGroup(SceneGroup* group);

		/*!
			\brief Method invoked whenever this component is removed from the supplied node
		*/
		void ComponentRemovedFromNode(SceneNode* node);

		/*!
			\brief Method invoked whenever this component is removed from the supplied group
		*/
		void ComponentRemovedFromGroup(SceneGroup* group);

	protected:
		/*!
			\brief Method called whenever this component is added to a scene node. 
		*/
		virtual void OnComponentAddedToNode();

		/*!
			\brief Method called whenever this component is removed from the scene node it's attached to
		*/
		virtual void OnComponentRemovedFromNode();
		
		/*!
			\brief Method called whenever this component is added to a scene group. 
		*/
		virtual void OnComponentAddedToGroup();

		/*!
			\brief Method called whenever this component is removed from the scene group it's attached to
		*/
		virtual void OnComponentRemovedFromGroup();

	public:
		//
		// Fire an event receivable by this component
		//
		// @param evt
		//			The event
		void FireEvent(const Event* evt);

		//
		// Fire an event receivable by this component
		//
		// @param evt 
		//			The event
		// @param typeMask 
		//			Only components of a specific type receives this event
		void FireEvent(const Event* evt, typemask typeMask);

	protected:
		//
		// Method called whenever this component receive an event
		//
		// @param evt
		virtual void OnEventReceived(const Event* evt);
		
	public:
		/*!
			\brief The group this component is attached to is added to the scene
		*/
		void AttachedToScene();
		
		/*!
			\brief The group this component is attached to is detached from the scene
		*/
		void DetachedFromScene();

	protected:
		/*!
			\brief Method called whenever this component is attached to the scene
		*/
		virtual void OnAttachedToScene();
		
		/*!
			\brief Method called whenever this component is detached from the scene
		*/
		virtual void OnDetachedFromScene();

	public:

		//
		// Method invoked whenever the owner node is moved
		virtual void OnPositionChanged(const Vector3& newPosition);

		//
		// Method invoked whenever the owner node is rotated
		virtual void OnRotationChanged(const Vector3& newRotation);

		//
		// Method invoked whenever the owner node is scaled
		virtual void OnScaleChanged(const Vector3& newScale);

	protected:
		std::string mComponentID;
		typemask mTypeMask;
		SceneNode* mSceneNode;
		SceneGroup* mSceneGroup;
		AABB mBoundingBox;
	};
}
