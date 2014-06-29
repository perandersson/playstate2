#pragma once
#include "../LinkedList.h"

namespace core
{
	//
	// How many ticks occure every second. This is an approximation.
	//
	// Use "core::GameTickDeltaTime" to see a more accurate time in seconds has progressed since the last tick.
	static const uint32 TicksPerSecond = 32;

	//
	// Inherit this to give it's target support for tick notification
	class Tickable
	{
	public:
		LinkedListLink<Tickable> TickableLink;

	public:
		Tickable();
		virtual ~Tickable();

		//
		// Method called once every tick.
		//
		// @see "core::GameTickDeltaTime" to see a more accurate time in seconds has progressed since the last tick.
		virtual void Tick() = 0;
	};

	//
	// Global property containing an approximate amount of seconds since the last tick was performed
	extern float32 GameTickDeltaTime;
}
