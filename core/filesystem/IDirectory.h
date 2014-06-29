#pragma once
#include "../typedefs.h"
#include "exception/FileException.h"
#include <vector>

namespace core
{
	class IFile;

	class IDirectory
	{
	public:
		virtual ~IDirectory() {}

		//
		// @return A vector containing the paths of all the files under this directory.
		virtual std::vector<std::shared_ptr<IFile>> GetFiles() const = 0;

		//
		// @return A vector containing the paths of all the directories under this directory.
		virtual std::vector<std::shared_ptr<IDirectory>> GetDirectories() const = 0;
		
		//
		// @param path
		//			The path to the file we want to open. This can be both relative and absolute.
		// @return A new file based on the current files path. This method always returns an object.
		virtual std::shared_ptr<IFile> OpenFile(const std::string& path) const = 0;

		//
		// @param path
		//			The path to the directory we want to open. This can be both relative and absolute.
		// @return A new file based on the current files path; NULL if the file doesn't exist.
		virtual std::shared_ptr<IDirectory> OpenDirectory(const std::string& path) const = 0;

		//
		// @return The path to this directory.
		virtual const std::string& GetAbsolutePath() const = 0;

		//
		// @return TRUE if the file exists; FALSE otherwise
		virtual bool Exists() const = 0;
	};

}
