#include "../../Memory.h"
#include "Win32FileSystem.h"
#include "Win32Directory.h"
#include "Win32File.h"
#include "../../StringUtils.h"
#include <algorithm>

using namespace core;

Win32FileSystem::Win32FileSystem(const std::string& path)
: mRootDirectory(path), mFileWatcher(this, path)
{
	mFileWatcher.Start();
}

Win32FileSystem::~Win32FileSystem()
{
	mFileWatcher.Stop();
}

std::shared_ptr<IFile> Win32FileSystem::OpenFile(const std::string& absolutePath) const
{
	// Unknown if the path is relative - must start with a '/'
	if (IsRelative(absolutePath))
		return std::shared_ptr<IFile>(new Win32File(*this, absolutePath));

	HANDLE fileHandle = GetHighestPriorityPathHandle(absolutePath);
	if(fileHandle != NULL)
		return std::shared_ptr<IFile>(new Win32File(*this, fileHandle, absolutePath));
	else
		return std::shared_ptr<IFile>(new Win32File(*this, absolutePath));
}

std::shared_ptr<IDirectory> Win32FileSystem::OpenRootDir() const
{
	std::string path = "/";
	HANDLE fileHandle = GetHighestPriorityPathHandle(path);
	return std::shared_ptr<IDirectory>(new Win32Directory(*this, fileHandle, path));
}

std::shared_ptr<IDirectory> Win32FileSystem::OpenDirectory(const std::string& absolutePath) const
{
	// Unknown if the path is relative - must start with a '/'
	if (IsRelative(absolutePath))
		return std::shared_ptr<IDirectory>(new Win32Directory(*this, absolutePath));
	
	HANDLE fileHandle = GetHighestPriorityPathHandle(absolutePath);
	if(fileHandle != NULL)
		return std::shared_ptr<IDirectory>(new Win32Directory(*this, fileHandle, absolutePath));
	else
		return std::shared_ptr<IDirectory>(new Win32Directory(*this, absolutePath));
}

HANDLE Win32FileSystem::GetHighestPriorityPathHandle(const std::string& path) const
{
	std::string absolutePath = mRootDirectory + path;
	HANDLE handle = CreateFile(absolutePath.c_str(), GENERIC_READ, FILE_SHARE_READ, 
			NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

	if(handle == INVALID_HANDLE_VALUE) {
		// Try as a folder
		handle = CreateFile(absolutePath.c_str(), GENERIC_READ, FILE_SHARE_READ, 
			NULL, OPEN_EXISTING, FILE_FLAG_BACKUP_SEMANTICS, NULL);
	}

	if(handle != INVALID_HANDLE_VALUE) {
		return handle;
	}

	return NULL;
}

void Win32FileSystem::ProcessEvents()
{
	mFileWatcher.LookForChanges();
}

bool Win32FileSystem::IsRelative(const std::string& path)
{
	if(path.empty())
		return true;

	return path.c_str()[0] != '/';
}

void Win32FileSystem::AddFileChangedListener(const std::regex& regex, IFileChangedListener* listener)
{
	mFileWatcher.AddListener(regex, listener);
}

void Win32FileSystem::AddFileChangedListener(const std::string& path, IFileChangedListener* listener)
{
	std::string dotsSlash = StringUtils::Replace(path, '.', std::string("\\."));
	std::string end("$");
	std::regex r(dotsSlash + end);
	mFileWatcher.AddListener(r, listener);
}

void Win32FileSystem::RemoveFileChangedListener(IFileChangedListener* listener)
{
	mFileWatcher.RemoveListener(listener);
}
