#pragma once
#include "../IFile.h"
#include <Windows.h>

namespace core
{
	class Win32FileSystem;

	class Win32File : public IFile
	{
	public:
		Win32File(const Win32FileSystem& fileSystem, const std::string& path);
		Win32File(const Win32FileSystem& fileSystem, HANDLE handle, const std::string& path);
		virtual ~Win32File();

	// IFile
	public:
		virtual std::istringstream Read() const;
		virtual std::shared_ptr<IFile> OpenFile(const std::string& path) const;
		virtual size_t GetFileSize() const;
		virtual size_t ReadBytes(byte* buffer, size_t offset, size_t bufferSize) const;
		virtual size_t ReadBytes(byte* buffer, size_t bufferSize) const;
		virtual std::shared_ptr<byte> ReadBytes() const;
		virtual void WriteBytes(byte* bytes, size_t size);
		virtual const std::string& GetAbsolutePath() const;
		virtual std::shared_ptr<IDirectory> GetDirectory() const;
		virtual bool Exists() const;
		
	private:
		const Win32FileSystem& mFileSystem;

		std::string mPath;
		size_t mFileSize;
		HANDLE mFileHandle;
	};
}
