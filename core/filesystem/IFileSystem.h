#pragma once
#include "IFile.h"
#include "IDirectory.h"
#include "IFileChangedListener.h"
#include <regex>

namespace core
{
	/*!
		\brief The IFileSystem interface is used to access the computers file system

		IFileSystem is a service interface, which means that you can access it through the FileSystem class or by using:
		{@code
			IFileSystem* inputDevices = Kernel::GetFileSystem();			
		}

		The path to the file always starts with '/'. Where the '/' directory is located is decided by the implementation for 
		this interface but is usually the same directory as the running application.

		Example:
		{@code
			// Using the service interface directly
			IFileSystem* fileSystem = Kernel::GetFileSystem();
			std::shared_ptr<IFile> file = fileSystem->OpenFile("/file.txt");
			if(file->Exists()) {
				// Read data from file
			}

			// Using the helper class
			std::shared_ptr<IFile> file = FileSystem::OpenFile("/file.txt");
			if(file->Exists()) {
				// Read data from file
			}
		}

		You can also get events whenever changes occur in the file system by registering an instance that inherits from IFileChangedListener
	*/
	class IFileSystem
	{
	public:
		virtual ~IFileSystem() {}

	public:
		/*!
			\brief Tries to open a file for the supplied absolute path.

			\param absolutePath
						The absolute path to the file
			\return A handle to the file found at the supplied absolutePath. Use the {@see IFile::Exists} to see if the
					file exists.
		*/
		virtual std::shared_ptr<IFile> OpenFile(const std::string& absolutePath) const = 0;
		
		/*!
			\brief Open's a directory handle for file system root directory.

			This method is syntactic suggar for:
			{@code
				std::shared_ptr<IDirectory> rootDir = FileSystem::OpenDirectory("/");
			}

			\return A handle to the applications root directory
		*/
		virtual std::shared_ptr<IDirectory> OpenRootDir() const = 0;

		/*!
			\brief Open's a directory handle for the supplied absolute path.

			\param absolutePath
						The path to the directory
			\return A handle to the applications directory at the supplied absolutePath. Use the {@see IDirectory::Exists} to see if
					the directory exists.
		*/
		virtual std::shared_ptr<IDirectory> OpenDirectory(const std::string& absolutePath) const = 0;
		
		//
		// Adds a event listener for when a specific file within a regular expression file is is changed
		//
		// @param regex 
		//			A regular expression of the file we want to listen to
		// @param listener
		virtual void AddFileChangedListener(const std::regex& regex, IFileChangedListener* listener) = 0;

		//
		// Adds a event listener for when a specific file is changed
		//
		// @param absolutePath 
		//			The path to the file we want to listen for
		// @param listener
		virtual void AddFileChangedListener(const std::string& absolutePath, IFileChangedListener* listener) = 0;

		//
		// Removes an event listener for when a specific file is changed
		//
		// @param listener
		virtual void RemoveFileChangedListener(IFileChangedListener* listener) = 0;
	};
}