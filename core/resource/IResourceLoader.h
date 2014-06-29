#pragma once
#include "../typedefs.h"
#include "exception/LoadResourceException.h"
#include "../filesystem/IFile.h"
#include "ResourceObject.h"

namespace core
{
	class IResourceLoader
	{
	public:
		virtual ~IResourceLoader() {}

		/*!
			\brief Method called when this resource type is loading it's resource.

			The user can assume that the file exists if this method is called.

			\param file
						The file we want to load
			\throws LoadResourceException If the resource failed to load for some reason
		*/
		virtual ResourceObject* Load(const IFile* file) = 0;

		/*!
			\brief Retrieves the default resource for this type

			\return The default resource object. This instance will be used when a resource for this type doesn't exist
					or if it fails to load by some reason.
		*/
		virtual ResourceObject* GetDefaultResource() = 0;

		/*!
			\return TRUE if the resource loader want to listen for file system changes; FALSE otherwise
		*/
		virtual bool WatchForFileSystemChanges() const = 0;
	};
}