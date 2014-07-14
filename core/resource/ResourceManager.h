#pragma once
#include "IResourceManager.h"
#include "../kernel/Kernel.h"

namespace core
{
	class ResourceManager
	{
	public:
		static void SetAccessibility(uint32 access);

		template<class T>
		static Resource<T> GetResource(const char* absolutePath) {
			return Kernel::GetResourceManager()->GetResource(absolutePath);
		}

		template<class T>
		static Resource<T> GetResource(const std::string& absolutePath) {
			return Kernel::GetResourceManager()->GetResource(absolutePath);
		}

		template<class T>
		static Resource<T> GetResource(std::shared_ptr<IFile> file) {
			return Kernel::GetResourceManager()->GetResource(file);
		}

		static void RegisterLoader(IResourceLoader* loader, const std::string& suffix);

		template<class T>
		static void UnloadResource(Resource<T> resource) {
			Kernel::GetResourceManager()->UnloadResource(resource);
		}

		static void AddResource(ResourceObject* object, const std::string& absolutePath);
	};

}
