#pragma once
#include "../typedefs.h"
#include "../LinkedList.h"
#include "FindQuery.h"
#include "Event.h"
#include "../script/ScriptObject.h"

namespace core
{
	class Scene;
	class Component;
	class SceneNode;
	class Updatable;
	class Tickable;
	class Renderable;
	class LightSource;
	class LightSourceResultSet;
	class RenderBlockResultSet;

	class SceneGroup : public ScriptObject
	{
		friend class SceneNode;
		DECLARE_SCRIPT_OBJECT(SceneGroup);

	public:
		LinkedListLink<SceneGroup> SceneLink;

	public:
		SceneGroup();
		virtual ~SceneGroup();

		//
		// Add the supplied scene node to this group
		//
		// @param node
		//			The node we want to add to this group
		virtual void AddSceneNode(SceneNode* node);

		//
		// Remove the supplied node from this group
		//
		// @param node
		virtual void RemoveSceneNode(SceneNode* node);
		
		/*!
			\brief Add the supplied component to this group

			\param component
					The component we want to add
		*/
		void AddComponent(Component* component);

		/*!
			\brief Remove the supplied component from this group

			\param component
					The component we want to remove
		*/
		void RemoveComponent(Component* component);

		/*!
			\brief Retrieves the first component matching the the supplied type. 
			
			\param typeMask
			\return The component if found; nullptr otherwise.
		*/
		Component* GetFirstComponent(typemask typeMask);

		/*!
			\brief Attach this group to the scene
		*/
		void AttachedToScene();

		/*!
			\brief Detaches this group from the scene
		*/
		void DetachedFromScene();

		/*!
			\brief Checks if this group is attached to the scene
			
			\return TRUE if this group is attached to the scene; FALSE otherwise
		*/
		inline bool IsAttachedToScene() const {
			return SceneLink.IsLinked();
		}

		//
		// Find the scene node with the supplied nodeID in this group
		//
		// @param nodeID
		// @return 
		SceneNode* FindSceneNode(const std::string& nodeID);

		//
		// Add the supplied updatable item. This is needed if we want it to receive updates
		//
		// @param updatable
		virtual void AddUpdatable(Updatable* updatable);

		//
		// Remove the supplied updatable item. This will make sure that the supplied object stop receive updates
		//
		// @param updatable
		virtual void RemoveUpdatable(Updatable* updatable);

		//
		// Add the supplied tickable item. This is needed if we want it to receive tick events
		//
		// @param tickable
		virtual void AddTickable(Tickable* tickable);

		//
		// Remove the supplied tickable item
		virtual void RemoveTickable(Tickable* updatable);

		//
		// Add the supplied renderable instance to this group
		virtual void AddRenderable(Renderable* renderable);

		//
		// Remove the supplied renderable instance from this group
		virtual void RemoveRenderable(Renderable* renderable);

		virtual void AddLightSource(LightSource* lightSource);

		virtual void RemoveLightSource(LightSource* lightSource);

		//
		// Fire an event receivable from this groups child nodes. Useful for sending global events.
		//
		// @param evt
		//			The event we want to send to all objects in this group
		void FireEvent(const Event* evt);

		//
		// Fire an event receivable from this groups child nodes. Useful for sending global events.
		//
		// @param evt
		//			The event we want to send to the objects in this group
		// @param typeMask 
		//			Only nodes of a specific type receives this event
		void FireEvent(const Event* evt, typemask typeMask);

		//
		// Sets this group's ID
		//
		// @param groupID
		void SetGroupID(const std::string& groupID);

		//
		// Set the type for this group
		//
		// @param typeMask
		void SetTypeMask(typemask typeMask);

		//
		// Sets the position of this node
		//
		// @param position
		//			The new position for this group
		void SetPosition(const Vector3& position);

		//
		// @return This group's ID
		inline const std::string& GetGroupID() const {
			return mGroupID;
		}

		//
		// @return The type mask for this group
		inline typemask GetTypeMask() const {
			return mTypeMask;
		}

		//
		// @return The position of this scene group
		inline const Vector3& GetPosition() const {
			return mPosition;
		}

		//
		// Update the internal updatable objects
		virtual void UpdateUpdatables();

		//
		// Update the internal tickable objects
		virtual void UpdateTickables();

		/*!
			\brief Search for renderable blocks in on this scene group

			\param query
					Query parameters used when searching for renderable blocks
			\param _out_resultSet
					Where we put the result
			\return TRUE if we found renderable blocks; FALSE otherwise
		*/
		virtual bool Find(const FindQuery& query, RenderBlockResultSet* _out_resultSet) const;

		//
		// Search for lightsources in this scene group.
		// 
		// @param query
		// @param _out_resultSet
		virtual bool Find(const FindQuery& query, LightSourceResultSet* _out_resultSet) const;

	public:
		//
		// Method called whenever the supplied scene node is added to this group
		virtual void OnSceneNodeAdded(SceneNode* node);

		//
		// Method called whenever the supplied node is removed from this group
		virtual void OnSceneNodeRemoved(SceneNode* node);

	protected:		
		/*!
			\brief Method called whenever this group is attached to the scene
		*/
		virtual void OnAttachedToScene();
		
		/*!
			\brief Method called whenever this group is detached from the scene
		*/
		virtual void OnDetachedFromScene();

	// ScriptObject
	public:
		virtual bool OnRegisterObject();

	private:
		std::string mGroupID;
		typemask mTypeMask;

		Vector3 mPosition;

		LinkedList<Component> mComponents;
		LinkedList<SceneNode> mChildren;

		std::shared_ptr<ScriptMethod> mOnAttachedToSceneMethodPtr;
		std::shared_ptr<ScriptMethod> mOnDetachedFromSceneMethodPtr;
	};

}
