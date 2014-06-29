#include "../Memory.h"
#include "FileSystem.h"
#include "../kernel/Kernel.h"
using namespace core;

std::shared_ptr<IFile> FileSystem::OpenFile(const std::string& path)
{
	return Kernel::GetFileSystem()->OpenFile(path);
}

std::shared_ptr<IDirectory> FileSystem::OpenRootDir()
{
	return Kernel::GetFileSystem()->OpenRootDir();
}

std::shared_ptr<IDirectory> FileSystem::OpenDirectory(const std::string& path)
{
	return Kernel::GetFileSystem()->OpenDirectory(path);
}

void FileSystem::AddFileChangedListener(const std::regex& regex, IFileChangedListener* listener)
{
	Kernel::GetFileSystem()->AddFileChangedListener(regex, listener);
}

void FileSystem::AddFileChangedListener(const std::string& path, IFileChangedListener* listener)
{
	Kernel::GetFileSystem()->AddFileChangedListener(path, listener);
}

void FileSystem::RemoveFileChangedListener(IFileChangedListener* listener)
{
	Kernel::GetFileSystem()->RemoveFileChangedListener(listener);
}
