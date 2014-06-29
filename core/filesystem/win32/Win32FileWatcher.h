#pragma once
#include "../IFileSystem.h"
#include <Windows.h>

#include <thread>
#include <mutex>
#include <atomic>

namespace core
{
	struct Win32FileListener	
	{
		IFileChangedListener* callback;
		std::regex regex;
	};

	struct Win32FileEvent
	{
		IFileChangedListener* callback;
		std::string fileName;
		FileChangeAction::Enum action;
	};

	class Win32FileSystem;

	class Win32FileWatcher
	{
		typedef std::list<Win32FileListener*> FileChangedListeners;
		typedef std::vector<Win32FileEvent*> FileEvents;

	public:
		Win32FileWatcher(Win32FileSystem* fileSystem, const std::string& rootDirPath);
		~Win32FileWatcher();

		//
		// Start watching for file changes
		void Start();

		//
		// Stop watching for file changes
		void Stop();

		void AddListener(const std::regex& regex, IFileChangedListener* listener);
		void RemoveListener(IFileChangedListener* listener);

		//
		// Refresh this file watcher and look file changes in the file system
		//
		void LookForChanges();

		void GetAndClearEvents(FileEvents& events);

		//
		// This method is executed from another thread
		void RunFromThread();

	private:
		Win32FileSystem* mFileSystem;
		FileChangedListeners mFileChangedListeners;
		std::mutex mListenersLock;

		FileEvents mFileEvents;
		std::mutex mFileEventsLock;

		HANDLE mRootDirHandle;
		std::thread mThread;
		std::string mRootDirPath;
	};
}
