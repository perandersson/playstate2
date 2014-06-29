#pragma once
#include "../typedefs.h"
#include "../Exception.h"
#include <thread>
#include <mutex>
#include <future>
#include <vector>
#include <list>

namespace core
{
	/*!
		\brief
	*/
	class ThreadPool
	{
		typedef std::vector<std::thread> WorkerThreads;
		typedef std::list<std::function<void()>> Tasks;

	public:
		ThreadPool();
		~ThreadPool();

		/*!
			\brief Initializes this thread pool

			\param numThreads
						The number of threads we want the thread-pool to use for job execution
		*/
		void Initialize(uint32 numThreads);

	private:
		WorkerThreads mWorkers;
		Tasks mTasks;

		std::condition_variable mCondition;
		std::mutex mMutex;
		volatile bool mRunning;

	public:
		/*!
			\brief Add a new task to be processed by this ThreadPool

			\param func
						The function we want to execute in on of the running threads
			\param args
						Arguments to be supplied to the executing thread
			\return A value containing the future value returned by the executing function
		*/
		template<class Func, class... Args>
		std::future<typename std::result_of<Func(Args...)>::type> AddTask(Func&& func, Args&&... args) {
			if (!mRunning)
				THROW_EXCEPTION(Exception, "You are not allowed to add a new task after the thread pool has been closed");

			typedef typename std::result_of<Func(Args...)>::type return_type;
			auto task = std::make_shared<std::packaged_task<return_type()>>(
				std::bind(std::forward<Func>(func), std::forward<Args>(args)...)
			);

			std::future<return_type> res = task->get_future();
			mMutex.lock();
			mTasks.push_back( [task](){ 
				(*task)(); 
			});
			mMutex.unlock();
			mCondition.notify_one();
			return res;
		}
	};
}
