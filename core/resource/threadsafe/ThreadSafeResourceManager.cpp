#include "../../Memory.h"
#include "ThreadSafeResourceManager.h"
#include "../../logging/Logger.h"
#include "../../configuration/Configuration.h"
using namespace core;

ThreadSafeResourceManager::ThreadSafeResourceManager(IFileSystem* fileSystem)
: IResourceManager(), mFileSystem(fileSystem), mAccessibility(ResourceAccess::ALL)
{
	assert_not_null(fileSystem);
}

ThreadSafeResourceManager::~ThreadSafeResourceManager()
{
	mFileSystem->RemoveFileChangedListener(this);

	ResourceNameToResource::iterator rit = mResources.begin();
	ResourceNameToResource::const_iterator rend = mResources.end();
	for (; rit != rend; ++rit) {
		ResourceObject* resource = rit->second->resource.load();
		if (resource != rit->second->defaultResource) {
			delete resource;
		}

		rit->second->resource = nullptr;
		rit->second->defaultResource = nullptr;
		delete rit->second;
		rit->second = nullptr;
	}

	ResourceLoaders::iterator lit = mResourceLoaders.begin();
	ResourceLoaders::const_iterator lend = mResourceLoaders.end();
	for (; lit != lend; ++lit) {
		delete lit->second;
	}
	mResourceLoaders.clear();
}

void ThreadSafeResourceManager::SetAccessibility(uint32 access)
{
	mAccessibility = access;
}

Resource<ResourceObject> ThreadSafeResourceManager::GetResource(const char* absolutePath)
{
	return ThreadSafeResourceManager::GetResource(std::string(absolutePath));
}

Resource<ResourceObject> ThreadSafeResourceManager::GetResource(const std::string& absolutePath)
{
	// TODO: This should only be a problem if actual resources are loaded (i.e. the IResourceLoader is called)
	if (!BIT_ISSET(mAccessibility.load(), ResourceAccess::LOADING))
		THROW_EXCEPTION(ResourceException, "You are not allowed to load resources at this time");

	auto suffix = GetSuffixFromName(absolutePath);
	auto resourceLoader = GetLoaderFromSuffix(suffix);
	if (resourceLoader == nullptr)
		THROW_EXCEPTION(ResourceException, "No ResourceLoader registered for suffix: %s", suffix.c_str());

	std::lock_guard<std::recursive_mutex> lg(mResourcesMutex);
	auto it = mResources.find(absolutePath);
	ResourceData* data;
	if (it != mResources.end()) {
		data = it->second;
		if (!IsDefaultResource(data)) {
			return it->second;
		}
	}
	else {
		data = new ResourceData();
		data->defaultResource = resourceLoader->GetDefaultResource();
		data->resource = data->defaultResource;
		mResources.insert(std::make_pair(absolutePath, data));
	}

	auto file = mFileSystem->OpenFile(absolutePath);
	if (!file->Exists()) {
		Logger::Error("Could not find resource: '%s'. Use default resource instead", file->GetAbsolutePath().c_str());
		data->resource = data->defaultResource;
		return Resource<ResourceObject>(data);
	}
	try {
		data->resource = resourceLoader->Load(file.get());
	} 
	catch (LoadResourceException e) {
#undef GetMessage
		Logger::Error("Could not load resource: '%s'. Reson: '%s'. Use default resource instead", file->GetAbsolutePath().c_str(), e.GetMessage().c_str());
		data->resource = data->defaultResource;
	}
	return Resource<ResourceObject>(data);
}

Resource<ResourceObject> ThreadSafeResourceManager::GetResource(std::shared_ptr<IFile> file)
{
	return GetResource(file->GetAbsolutePath());
}

void ThreadSafeResourceManager::RegisterLoader(IResourceLoader* loader, const std::string& suffix)
{
	assert(mResourceLoaders.find(suffix) == mResourceLoaders.end() && "You are trying to add the same loader twice");
	mResourceLoaders.insert(std::make_pair(suffix, loader));

	if (Configuration::ToBool("resources.developmentmode", false)) {
		std::string str = "[^\\s]+\\.";
		str += suffix;
		str += "$";
		mFileSystem->AddFileChangedListener(std::regex(str), this);
	}
}

void ThreadSafeResourceManager::UnloadResource(Resource<ResourceObject> resource)
{
	// TODO: Ignore if it's a default resource. Those are not unloaded anyways
	if (!BIT_ISSET(mAccessibility.load(), ResourceAccess::UNLOADING)) {
		Logger::Warn("You are not allowed to unload resources at the moment");
		return;
	}

	std::lock_guard<std::recursive_mutex> lg(mResourcesMutex);
	ResourceData* data = resource.GetResourceData();
	if (IsDefaultResource(data))
		return;

	data->resource = data->defaultResource;
	delete data;
}

void ThreadSafeResourceManager::AddResource(ResourceObject* object, const std::string& absolutePath)
{
	// TODO: Ignore if it's a default resource.
	if (!BIT_ISSET(mAccessibility.load(), ResourceAccess::LOADING)) {
		Logger::Warn("You are not allowed to load resources at the moment");
		return;
	}

	auto suffix = GetSuffixFromName(absolutePath);
	auto resourceLoader = GetLoaderFromSuffix(suffix);

	std::lock_guard<std::recursive_mutex> lg(mResourcesMutex);
	auto it = mResources.find(absolutePath);
	if (it != mResources.end()) {
		Logger::Error("Cannot add resource: '%s'. It's already loaded", absolutePath.c_str());
	}

	ResourceData* data = new ResourceData();
	data->defaultResource = resourceLoader->GetDefaultResource();
	data->resource = object;
	mResources.insert(std::make_pair(absolutePath, data));
}

IResourceLoader* ThreadSafeResourceManager::GetLoaderFromSuffix(const std::string& suffix) const
{
	ResourceLoaders::const_iterator it = mResourceLoaders.find(suffix);
	if (it == mResourceLoaders.end())
		return nullptr;

	return it->second;
}

std::string ThreadSafeResourceManager::GetSuffixFromName(const std::string& name) const
{
	std::string::size_type separator = name.find_last_of('.');
	assert(separator != std::string::npos && "No real filename was supplied");
	std::string suffix = name.substr(separator + 1);
	return suffix;
}

bool ThreadSafeResourceManager::IsDefaultResource(ResourceData* data) const
{
	return data->resource == data->defaultResource;
}

void ThreadSafeResourceManager::OnFileChanged(const IFile* file, FileChangeAction::Enum action)
{
	const std::string& absolutePath = file->GetAbsolutePath();
	if (action != FileChangeAction::MODIFIED)
		return;

	std::lock_guard<std::recursive_mutex> lg(mResourcesMutex);
	ResourceNameToResource::iterator it = mResources.find(absolutePath);
	if (it != mResources.end()) {
		ResourceData* data = it->second;
		if (!IsDefaultResource(data)) {
			Logger::Info("Reloading resource: %s", absolutePath.c_str());
			auto suffix = GetSuffixFromName(absolutePath);
			auto resourceLoader = GetLoaderFromSuffix(suffix);
			try {
				auto prevResource = data->resource.load();
				data->resource = resourceLoader->Load(file);
				delete prevResource;
			}
			catch (LoadResourceException e) {
#undef GetMessage
				Logger::Error("Could not load resource: '%s'. Reson: '%s'. Use default resource instead", file->GetAbsolutePath().c_str(), e.GetMessage().c_str());
				data->resource = data->defaultResource;
			}
		}
	}
}
