#pragma once
#include "Resource.h"
#include "IResourceLoader.h"
#include "exception/ResourceException.h"
#include "ResourceAccess.h"

namespace core
{
	/*!
		\brief
	*/
	class IResourceManager
	{
	public:
		virtual ~IResourceManager() {}

	public:
		/*!
			\brief  Set the accessibility to the resource manager

			This enables us to prevent loading- and unloading of resources in different phases

			\param access
		*/
		virtual void SetAccessibility(uint32 access) = 0;

		/*!
			\brief Retrieves a resource at the given resource path

			\param absolutePath
			\return
		*/
		virtual Resource<ResourceObject> GetResource(const char* absolutePath) = 0;
		
		/*!
			\brief Retrieves a resource at the given resource path

			\param absolutePath
			\return
		*/
		virtual Resource<ResourceObject> GetResource(const std::string& absolutePath) = 0;

		/*!
			\brief Retrieves a resource

			\param file
			\return
		*/
		virtual Resource<ResourceObject> GetResource(std::shared_ptr<IFile> file) = 0;

		/*!
			\brief Registers a resource loader for a given file suffix

			\param loader
					A loader useful for the given file suffix
			\param suffix
					A resource file suffix (example: "png")
		*/
		virtual void RegisterLoader(IResourceLoader* loader, const std::string& suffix) = 0;
		
		/*!
			\brief Unload the given resource object.

			\remark Since the internal memory management uses a shared_ptr- like interface. It's not guaranteed that the
					memory to the pointer is destroyed if someone is holding a shared_ptr. This will, however, make sure that
					the objects internal resources are released (thus it will not contain any data).

			\param resource
					The resource we want to release
		*/
		virtual void UnloadResource(Resource<ResourceObject> resource) = 0;
		
	};

}
