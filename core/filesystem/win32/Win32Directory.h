#pragma once
#include "../IDirectory.h"
#include <Windows.h>

namespace core
{
	class Win32FileSystem;

	class Win32Directory : public IDirectory
	{
	public:
		Win32Directory(const Win32FileSystem& fileSystem, const std::string& path);
		Win32Directory(const Win32FileSystem& fileSystem, HANDLE handle, const std::string& path);
		virtual ~Win32Directory();

	// IDirectory
	public:
		virtual std::vector<std::shared_ptr<IFile>> GetFiles() const;
		virtual std::vector<std::shared_ptr<IDirectory>> GetDirectories() const;
		virtual std::shared_ptr<IFile> OpenFile(const std::string& path) const;
		virtual std::shared_ptr<IDirectory> OpenDirectory(const std::string& path) const;
		virtual const std::string& GetAbsolutePath() const;
		virtual bool Exists() const;
		
	private:
		const Win32FileSystem& mFileSystem;

		std::string mPath;
		HANDLE mDirectoryHandle;
	};
}
