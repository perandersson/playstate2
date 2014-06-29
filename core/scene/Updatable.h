#pragma once
#include "../LinkedList.h"

namespace core
{
	class Updatable
	{
	public:
		LinkedListLink<Updatable> UpdatableLink;

	public:
		Updatable();
		virtual ~Updatable();

		//
		// Method invoked when the current item is being updated. This is called once every frame.
		virtual void Update() = 0;
	};

	//
	// Global property containing the time since last update
	extern float32 GameDeltaTime;

	//
	// Global property containing the time since beginning of the game
	extern float64 GameTotalTime;
}
