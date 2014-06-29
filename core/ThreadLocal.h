#pragma once
#include "typedefs.h"

#ifdef WIN32
#define THREAD_LOCAL __declspec(thread)
#else
#error Define a thread local keyworrd for non WIN32 compilers!
#endif

namespace core
{
	//
	// Inherit from this if you want to have singleton-ish behaviour for a class. Instead of being completely singleton, this class
	// indicates that the class should have a unique instance per thread.
	template<class T>
	class ThreadLocal
	{
	private:
		static THREAD_LOCAL T gThreadLocal;

	public:
		//
		// Retrieve the value for this class associated with the currently running thread
		//
		// @return A thread-local instance if set; nullptr otherwise.
		static T GetThreadLocal() {
			return gThreadLocal;
		}

		//
		// Associates the supplied instance with the currently running thread
		//
		// @param instance
		//			The instance we want to associate with the currently running thread
		static void SetThreadLocal(T instance) {
			gThreadLocal = instance;
		}
	};
}
