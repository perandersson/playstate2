#include "../Memory.h"
#include "ResourceManager.h"
using namespace core;

void ResourceManager::SetAccessibility(uint32 access)
{
	Kernel::GetResourceManager()->SetAccessibility(access);
}

void ResourceManager::RegisterLoader(IResourceLoader* loader, const std::string& suffix)
{
	return Kernel::GetResourceManager()->RegisterLoader(loader, suffix);
}
