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
		IFileChangedListener* Callback;
		std::regex Regex;
	};

	class Win32FileSystem;

	class Win32FileWatcher
	{
		typedef std::list<Win32FileListener*> FileChangedListeners;

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
		// This method is executed from another thread
		void RunFromThread();

	private:
		Win32FileSystem* mFileSystem;
		FileChangedListeners mFileChangedListeners;
		std::mutex mListenersLock;

		HANDLE mRootDirHandle;
		std::thread mThread;
		std::string mRootDirPath;
	};
}
