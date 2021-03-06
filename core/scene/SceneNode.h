#pragma once
#include "../typedefs.h"
#include "../math/Vector3.h"
#include "../LinkedList.h"
#include "FindQuery.h"
#include "Event.h"
#include "../script/ScriptObject.h"

namespace core
{
	class Component;
	class Updatable;
	class Tickable;
	class Renderable;
	class LightSource;

	/*!
		\brief 
	*/
	class SceneNode : public ScriptObject
	{
		friend class SceneGroup;
		DECLARE_SCRIPT_OBJECT(SceneNode);

	public:
		LinkedListLink<SceneNode> SceneNodeLink;

	public:
		SceneNode();
		virtual ~SceneNode();

		/*!
			\brief Add the supplied node

			\param node
		*/
		void AddChildNode(SceneNode* node);

		/*!
			\brief Remove the supplied child scene node 

			\param node
						The node we want to remove
		*/
		void RemoveChildNode(SceneNode* node);

		/*!
			\brief Remove this node from the parent node

			Call this on a node if you want it to be removed from it's parent
		*/
		void RemoveFromParent();
		
		/*!
			\brief The group this node is attached to is added to the scene
		*/
		void AttachedToScene();
		
		/*!
			\brief The group this node is attached to is detached from the scene
		*/
		void DetachedFromScene();

		/*!
			\brief Retrieves the group this node is attached to

			\return The group instance; nullptr if not attached to any group.
		*/
		inline SceneGroup* GetSceneGroup() const {
			return mSceneGroup;
		}

		/*!
			\brief Find a scene node with the supplied id

			\param id
					The scene node's ID
			\return The scene node if found; nullptr otherwise.
		*/
		SceneNode* FindSceneNode(const std::string& id);

		/*!
			\brief Find the first component matching the supplied type mask

			\param typeMask
					A type mask that the scene node we are looking for have.
			\return The scene node if found; nullptr otherwise.
		*/
		SceneNode* FindFirstSceneNode(typemask typeMask);

		/*!
			\brief Add the supplied component to this node

			\param component
					The component we want to add to this node
		*/
		void AddComponent(Component* component);

		/*!
			\brief Remove the supplied component from this node

			\param component
					The component we want to remove
		*/
		void RemoveComponent(Component* component);

		//
		// Retrieves the first component matching the the supplied type. If no component is found then return NULL.
		//
		// @param typeMask 
		// @return The component if found; NULL otherwise
		Component* GetFirstComponent(typemask typeMask);

		//
		// Set the type mask for this node
		//
		// @parma typeMask
		void SetTypeMask(typemask typeMask);

		/*!
			\brief Set this node's ID
			
			\param id
						The ID for this node
		*/
		void SetID(const std::string& id);

		//
		// Sets the position of this node.
		void SetPosition(const Vector3& position);

		//
		// Sets the rotation of this node. 
		void SetRotation(const Vector3& rotation);

		//
		// Sets the scale of this node
		void SetScale(const Vector3& scale);

		//
		// @return The type mask for this node
		inline typemask GetTypeMask() const {
			return mTypeMask;
		}

		//
		// @return This nodes ID
		inline const std::string& GetID() const {
			return mID;
		}

		/*!
			\brief Retrieves the relative position of this node

			\remark The position is in relation to child- and parent nodes from this node's perspective

			\return The relative position
		*/
		inline const Vector3& GetPosition() const {
			return mPosition;
		}
		
		/*!
			\brief Retrieves the absolute position of this node

			\remark The position is not absolute world position, but in relation to the scene group. 

			\return The absolute position
		*/
		inline const Vector3& GetAbsolutePosition() const {
			return mAbsolutePosition;
		}
		
		/*!
			\brief Retrieves the relative rotation of this node

			\remark The rotation is in relation to child- and parent nodes from this node's perspective

			\return The relative rotation
		*/
		inline const Vector3& GetRotation() const {
			return mRotation;
		}
		
		/*!
			\brief Retrieves the absolute rotation of this node

			\remark The rotation is not absolute world rotation, but in relation to the scene group. 

			\return The absolute rotation
		*/
		inline const Vector3& GetAbsoluteRotation() const {
			return mAbsoluteRotation;
		}
		
		/*!
			\brief Retrieves the relative scale of this node

			\remark The scale is in relation to child- and parent nodes from this node's perspective

			\return The relative scale
		*/
		inline const Vector3& GetScale() const {
			return mScale;
		}
		
		/*!
			\brief Retrieves the absolute scale of this node

			\remark The scale is not absolute world scale, but in relation to the scene group. 

			\return The absolute scale
		*/
		inline const Vector3& GetAbsoluteScale() const {
			return mAbsoluteScale;
		}

		//
		// Checks if this node is being attached to a parent node
		//
		// @return TRUE if this node is attached to a parent node; FALSE otherwise
		inline bool IsAttachedToParent() const {
			return mParent != nullptr;
		}

		/*!
			\brief Check if this node is attached to a parent scene group

			\return TRUE if this node is attached to a parent scene group; FALSE otherwise
		*/
		inline bool IsAttachedToSceneGroup() const {
			return mSceneGroup != nullptr;
		}

		/*!
			\brief Checks if this node is attached to the scene or not

			\return TRUE if this node is attached to the scene; FALSE otherwise
		*/
		bool IsAttachedToScene() const;

		/*!
			\brief Method called by the scene group whenever this node is added to it

			\param group
						The group this node is added to
		*/
		void AddedToSceneGroup(SceneGroup* group);

		/*!
			\brief Method called by the scene group whenever this node is removed from it

			\param group
						The group we want to remove this node from. 
			\remark This method is invoked by the game engine and should never be called otherwise - unless you are absolutely
					sure what you are doing.
		*/
		void RemovedFromSceneGroup(SceneGroup* group);

		/*!
			\brief Fire an event receivable from this node's component

			Useful for communication between components inside the same node.

			\param evt
						The event
		*/
		void FireEvent(const Event* evt);

		/*!
			\brief Fire an event receivable from this node's component

			Useful for communication between components inside the same node.

			\param evt
						The event
			\param typeMask
						What types of componenets we want to receive this event
		*/
		void FireEvent(const Event* evt, typemask typeMask);

		//
		// Method called whenever this node receive an event
		//
		// @param evt
		void OnEventReceived(const Event* evt);

		virtual bool OnRegisterObject();

	protected:
		//
		// Method called when a child is added to this node
		//
		// @param node
		virtual void OnSceneNodeAdded(SceneNode* node);

		//
		// Method invoked when a child is removed from this node
		//
		// @param node
		virtual void OnSceneNodeRemoved(SceneNode* node);

		//
		// Method called whenever this node is removed from a parent node
		//
		// @param parent
		//			The parent node we are removing this node from
		virtual void OnDetachedFromParent(SceneNode* parent);

		//
		// Method called whenever this node is added to a parent node
		//
		// @param parent
		//			The parent node we are added this node to
		virtual void OnAttachedToParent(SceneNode* parent);
		
		/*!
			\brief Method called whenever this node is added to a scene group
		*/
		virtual void OnAddedToSceneGroup();

		/*!
			\brief Method called whenever this node is removed from a scene group
		*/
		virtual void OnRemovedFromSceneGroup();

		/*!
			\brief Method called whenever this node is attached to the scene
		*/
		virtual void OnAttachedToScene();
		
		/*!
			\brief Method called whenever this node is detached from the scene
		*/
		virtual void OnDetachedFromScene();

		/*!
			\brief Method called whenever the position of this node has been changed
		*/
		virtual void OnPositionChanged();

		/*!
			\brief
		*/
		virtual void OnRotationChanged();

		/*!
			\brief
		*/
		virtual void OnScaleChanged();

		//
		// 
		void AddUpdatable(Updatable* updatable);

		//
		// 
		void RemoveUpdatable(Updatable* updatable);

		//
		//
		void AddTickable(Tickable* tickable);

		//
		//
		void RemoveTickable(Tickable* tickable);

	protected:
		//
		// Updates the absolute position value based on the parents position
		void UpdatePosition(const Vector3& parentPosition);

		//
		// Updates the absolute rotation value based on the parents rotation
		void UpdateRotation(const Vector3& parentRotation);

		//
		// Updates the absolute scale value based on the parents scale
		void UpdateScale(const Vector3& parentScale);

	private:
		SceneNode* mParent;
		SceneGroup* mSceneGroup;
		typemask mTypeMask;

	protected:
		std::string mID;
		Vector3 mPosition;
		Vector3 mAbsolutePosition;
		Vector3 mRotation;
		Vector3 mAbsoluteRotation;
		Vector3 mScale;
		Vector3 mAbsoluteScale;

		LinkedList<SceneNode> mChildren;
		LinkedList<Component> mComponents;
		
		std::shared_ptr<ScriptMethod> mOnAddedToSceneGroupPtr;
		std::shared_ptr<ScriptMethod> mOnRemovedFromSceneGroupPtr;
		std::shared_ptr<ScriptMethod> mOnAttachedToSceneMethodPtr;
		std::shared_ptr<ScriptMethod> mOnDetachedFromSceneMethodPtr;
		std::shared_ptr<ScriptMethod> mOnPositionChangedPtr;
		std::shared_ptr<ScriptMethod> mOnRotationChangedPtr;
		std::shared_ptr<ScriptMethod> mOnScaleChangedPtr;
	};
}
