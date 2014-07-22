#pragma once
#include "SceneGroup.h"
#include "SceneGroupListener.h"
#include "FindQuery.h"
#include "../math/Color.h"
#include "rendering/RenderBlockResultSet.h"
#include "light/LightSourceResultSet.h"
#include "Event.h"
#include "../camera/Camera.h"

namespace core
{
	class Scene
	{
	public:
		Scene();
		~Scene();

		/*!
			\brief Adds a new scene group to the current scene

			\param group
					The group we want to add to the scene manager
		*/
		void AddSceneGroup(SceneGroup* group);

		/*!
			\brief Remove the supplied scene group from this scene

			\param group
		*/
		void RemoveSceneGroup(SceneGroup* group);
		
		/*!
			\brief Add a scene group listener

			\param listener
		*/
		void AddSceneGroupListener(SceneGroupListener* listener);

		/*!
			\brief Remove a scene group listener

			\param listener
		*/
		void RemoveSceneGroupListener(SceneGroupListener* listener);

		//
		// Update the scene
		void Update();

		//
		// Fire an event to the entire scene.
		//
		// @param evt
		//			The event we want to send to all objects on the scene
		void FireEvent(const Event* evt);

		//
		// Fire an event to the entire scene.
		//
		// @param evt
		//			The event we want to send to the objects on the scene
		// @param typeMask 
		//			Only nodes of a specific type receives this event
		void FireEvent(const Event* evt, typemask typeMask);

		/*!
			\brief Set the active camera for the scene

			\param camera
						The camera
		*/
		void SetActiveCamera(Camera* camera);

		//
		// @return The active camera in this scene
		inline Camera* GetActiveCamera() {
			return mActiveCamera;
		}

		//
		// @return The active camera in this scene
		inline const Camera* GetActiveCamera() const {
			return mActiveCamera;
		}

		//
		// Set the ambient lighting in this scene
		//
		// @param color
		//			The ambient lighting for the entire scene
		void SetAmbientLight(const Color& color);

		//
		// @return The ambient lighting in this scene
		inline const Color& GetAmbientLight() const {
			return mAmbientLight;
		}

		/*!
			\brief Query the scene for items located in it.

			\param query
					The query we are performing
			\param _out_resultSet
					Container where all the found items are put into
			\return TRUE if the query resulted in found elements
		*/
		bool Find(const FindQuery& query, RenderBlockResultSet* _out_resultSet) const;
		
		/*!
			\brief Query the scene for items located in it.

			\param query
					The query we are performing
			\param sorter
					Pointer to an implementation sorting the resulting render blocks
			\param _out_resultSet
					Container where all the found items are put into
			\return TRUE if the query resulted in found elements
		*/
		bool Find(const FindQuery& query, IRenderBlockSorter* sorter, RenderBlockResultSet* _out_resultSet) const;

		//
		// Search for lightsources in this scene group.
		// 
		// @param query
		// @param _out_resultSet
		virtual bool Find(const FindQuery& query, LightSourceResultSet* _out_resultSet) const;

	private:
		LinkedList<SceneGroup> mSceneGroups;
		float32 mTimeSinceLastTick;
		Camera* mActiveCamera;
		Color mAmbientLight;
		LinkedList<SceneGroupListener> mSceneGroupListeners;
	};
}
