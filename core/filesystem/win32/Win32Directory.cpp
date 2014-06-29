#include "../../Memory.h"
#include "Win32Directory.h"
#include "Win32FileSystem.h"
#include <cassert>

using namespace core;

Win32Directory::Win32Directory(const Win32FileSystem& fileSystem, const std::string& path)
: mFileSystem(fileSystem), mPath(path), mDirectoryHandle(NULL)
{
}

Win32Directory::Win32Directory(const Win32FileSystem& fileSystem, HANDLE handle, const std::string& path) 
: mFileSystem(fileSystem), mPath(path), mDirectoryHandle(handle)
{
}

Win32Directory::~Win32Directory()
{
	if(mDirectoryHandle != NULL) {
		CloseHandle(mDirectoryHandle);
		mDirectoryHandle = NULL;
	}

	mPath.clear();
}

std::vector<std::shared_ptr<IFile>> Win32Directory::GetFiles() const
{
	assert_not_implemented();
	std::vector<std::shared_ptr<IFile>> files;
	/*WIN32_FIND_DATAA ffd;
	HANDLE fileHandle = INVALID_HANDLE_VALUE;
	
	fileHandle = FindFirstFileA(mPath.c_str(), &ffd);
	if(fileHandle == INVALID_HANDLE_VALUE) {
		return files;
	}

	do {
		files.push_back(ffd.cFileName);
	} while(FindNextFileA(fileHandle, &ffd) != 0);

	FindClose(fileHandle);*/
	return files;
}

std::vector<std::shared_ptr<IDirectory>> Win32Directory::GetDirectories() const
{
	assert_not_implemented();
	std::vector<std::shared_ptr<IDirectory>> directories;
	return directories;
}

std::shared_ptr<IFile> Win32Directory::OpenFile(const std::string& path) const
{
	std::string fullPath = path;
	if (mFileSystem.IsRelative(path)) {
		fullPath = mPath + "/" + path;
	}

	return mFileSystem.OpenFile(fullPath);
}

std::shared_ptr<IDirectory> Win32Directory::OpenDirectory(const std::string& path) const
{
	std::string fullPath = path;
	if(mFileSystem.IsRelative(path)) {
		fullPath = mPath + path;
	}
	
	return mFileSystem.OpenDirectory(fullPath);
}

const std::string& Win32Directory::GetAbsolutePath() const
{
	return mPath;
}

bool Win32Directory::Exists() const
{
	return mDirectoryHandle != NULL;
}
