#pragma once
#include "../IResourceManager.h"
#include "../../filesystem/IFileSystem.h"
#include <mutex>
#include <atomic>

namespace core
{
	class ThreadSafeResourceManager : public IResourceManager, public IFileChangedListener
	{
		typedef std::hash_map<std::string, IResourceLoader*> ResourceLoaders;
		typedef std::hash_map<std::string, ResourceData*> ResourceNameToResource;

	public:
		ThreadSafeResourceManager(IFileSystem* fileSystem);
		virtual ~ThreadSafeResourceManager();

		virtual void SetAccessibility(uint32 access);
		virtual Resource<ResourceObject> GetResource(const char* absolutePath);
		virtual Resource<ResourceObject> GetResource(const std::string& absolutePath);
		virtual Resource<ResourceObject> GetResource(std::shared_ptr<IFile> file);
		virtual void RegisterLoader(IResourceLoader* loader, const std::string& suffix);
		virtual void UnloadResource(Resource<ResourceObject> resource);
		virtual void OnFileChanged(const IFile* file, FileChangeAction::Enum action);

	private:
		IResourceLoader* GetLoaderFromSuffix(const std::string& suffix) const;
		std::string GetSuffixFromName(const std::string& name) const;
		void _UnloadResource(uint32 uid);
		bool IsDefaultResource(ResourceData* data) const;

	private:
		IFileSystem* mFileSystem;
		std::atomic<uint32> mAccessibility;
		std::recursive_mutex mResourcesMutex;
		ResourceNameToResource mResources;

		ResourceLoaders mResourceLoaders;
	};
}