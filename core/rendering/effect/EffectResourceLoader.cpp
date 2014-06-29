#include "../../Memory.h"
#include "EffectResourceLoader.h"
#include "EffectXmlVisitor.h"
#include <tinyxml2.h>
using namespace core;
using namespace tinyxml2;

EffectResourceLoader::EffectResourceLoader()
{

}

EffectResourceLoader::~EffectResourceLoader()
{

}

ResourceObject* EffectResourceLoader::Load(const IFile* file)
{
	auto fileData = file->Read().str();

	XMLDocument document;
	XMLError result = document.Parse(fileData.c_str());
	if (result != XML_SUCCESS)
		THROW_EXCEPTION(LoadResourceException, "Could not parse file: '%s'", file->GetAbsolutePath().c_str());

	EffectXmlVisitor visitor(GetRenderContext());
	if (!document.Accept(&visitor))
		THROW_EXCEPTION(LoadResourceException, "Could not build scene group: '%s'", file->GetAbsolutePath().c_str());

	return visitor.GetEffect();
}

ResourceObject* EffectResourceLoader::GetDefaultResource()
{
	return nullptr;
}
