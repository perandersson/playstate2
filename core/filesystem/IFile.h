#pragma once
#include "../typedefs.h"
#include "exception/FileException.h"
#include <sstream>

namespace core
{
	class IDirectory;

	class IFile
	{
	public:
		virtual ~IFile() {}

		//
		// @return A stream representing the entire file
		virtual std::istringstream Read() const = 0;

		//
		// @param path
		//			The path to the file we want to open. This can be both relative and absolute.
		// @return A new file based on the current files path. This method always returns an object.
		virtual std::shared_ptr<IFile> OpenFile(const std::string& path) const = 0;

		//
		// @return the size of the current file.
		virtual size_t GetFileSize() const = 0;
		
		//
		// @return The number of bytes that where read
		virtual size_t ReadBytes(byte* buffer, size_t offset, size_t bufferSize) const = 0;

		//
		// @return
		virtual size_t ReadBytes(byte* buffer, size_t bufferSize) const = 0;

		/*!
			\brief Read all the bytes located in the file

			To get the size of the shared_ptr array, use {@see IFile::GetFileSize}

			\return A shared_ptr containing the bytes
		*/
		virtual std::shared_ptr<byte> ReadBytes() const = 0;

		//
		//
		virtual void WriteBytes(byte* bytes, size_t size) = 0;

		//
		// @return The path to the file
		virtual const std::string& GetAbsolutePath() const = 0;
		
		//
		// @return A new directory instance where this file is located.
		virtual std::shared_ptr<IDirectory> GetDirectory() const = 0;

		//
		// @return TRUE if the file exists; FALSE otherwise
		virtual bool Exists() const = 0;
	};
}
