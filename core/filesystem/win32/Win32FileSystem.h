#pragma once
#include "../IFileSystem.h"
#include "Win32FileWatcher.h"
#include <windows.h>

namespace core
{
	class Win32FileSystem : public IFileSystem
	{
		typedef std::vector<std::string> Paths;

	public:
		Win32FileSystem(const std::string& path);
		virtual ~Win32FileSystem();

		//
		// Check if a path is relative or not
		// @return TRUE if the supplied path is a relative path; FALSE otherwise.
		static bool IsRelative(const std::string& path);

		//
		// Retrieves the Windows file handle for the supplied file that has the highest priority
		// @param path
		HANDLE GetHighestPriorityPathHandle(const std::string& path) const;

		/*!
			\brief Processes the internal file system events
		*/
		void ProcessEvents();

	// IFileSystem
	public:
		virtual std::shared_ptr<IFile> OpenFile(const std::string& path) const;
		virtual std::shared_ptr<IDirectory> OpenRootDir() const;
		virtual std::shared_ptr<IDirectory> OpenDirectory(const std::string& path) const;
		virtual void AddFileChangedListener(const std::regex& regex, IFileChangedListener* listener);
		virtual void AddFileChangedListener(const std::string& path, IFileChangedListener* listener);
		virtual void RemoveFileChangedListener(IFileChangedListener* listener);

	private:
		std::string mRootDirectory;
		Win32FileWatcher mFileWatcher;
	};
}
