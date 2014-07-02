#include "../Memory.h"
#include "ThreadPool.h"
using namespace core;

ThreadPool::ThreadPool()
{
	
}

ThreadPool::~ThreadPool()
{
	mMutex.lock();
	mRunning = false;
	mMutex.unlock();

	// Wait for all the threads to close
	mCondition.notify_all();
	uint32 size = mWorkers.size();
	for (uint32 i = 0; i < size; ++i) {
		mWorkers[i].join();
	}
}

void ThreadPool::Initialize(uint32 numThreads)
{
	for (uint32 i = 0; i < numThreads; ++i) {
		mWorkers.emplace_back([this] {
			while (true) {
				std::unique_lock<std::mutex> lock(mMutex);

				// Wait until a rask is found in the list
				while (mRunning && mTasks.empty())
					mCondition.wait(lock);

				// If no longer running then return/close the thread
				if (!mRunning && mTasks.empty())
					return;

				// Get the task from the list of tasks and remove it from the
				// task queue
				std::function<void()> currentTask(mTasks.front());
				mTasks.pop_front();
				lock.unlock();

				// Run the task
				currentTask();
			}
		});
	}
}
