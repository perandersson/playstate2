#pragma once
#include "typedefs.h"

namespace core
{
	//
	// Singleton wrapper used by game engine services
	template<class T>
	class Singleton
	{
	private:
		static T* gSingleton;

	public:
		//
		// Retrieve the singleton pointer
		//
		// @return The singleton pointer
		static T* GetInstance() {
			return gSingleton;
		}

		//
		// Set the singleton pointer
		//
		// @param
		//			The pointer
		static void SetInstance(T* instance) {
			gSingleton = instance;
		}
	};

	////
	//// Function used to retrieve a registered game engine service for the specified interface
	////  
	//// @return A pointer to the service if registered; nullptr otherwise.
	//template<class T>
	//T* GetInstanceOf() {
	//	return Singleton<T>::GetInstance();
	//}

	////
	//// Function used to set a game engine service for the specified interface
	////
	//// @param instance
	////			The game engine service
	//template<class T>
	//void SetInstanceOf(T* instance) {
	//	Singleton<T>::SetInstance(instance);
	//}

	////
	//// Function used to set a game engine service for the specified interface
	////
	//// @param instance
	////			The game engine service
	//template<class T, class U>
	//void SetInstanceOf(std::shared_ptr<U> instance) {
	//	Singleton<T>::SetInstance(std::dynamic_pointer_cast<T, U>(instance).get());
	//}
}
