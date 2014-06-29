#pragma once
#include "IFileSystem.h"

namespace core
{
	class FileSystem
	{
	public:
		static std::shared_ptr<IFile> OpenFile(const std::string& absolutePath);
		static std::shared_ptr<IDirectory> OpenRootDir();
		static std::shared_ptr<IDirectory> OpenDirectory(const std::string& absolutePath);
		static void AddFileChangedListener(const std::regex& regex, IFileChangedListener* listener);
		static void AddFileChangedListener(const std::string& absolutePath, IFileChangedListener* listener);
		static void RemoveFileChangedListener(IFileChangedListener* listener);
	};
}
