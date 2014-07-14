#include "../../Memory.h"
#include "EffectResourceLoader.h"
#include "EffectXmlVisitor.h"
#include "../../configuration/Configuration.h"
#include "../../logging/Logger.h"
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

	try {
		EffectXmlVisitor visitor(GetRenderContext(), file);
		if (!document.Accept(&visitor))
			THROW_EXCEPTION(LoadResourceException, "Could not build scene group: '%s'", file->GetAbsolutePath().c_str());
		return visitor.GetEffect();
	}
	catch (LoadResourceException e) {
		Logger::Error("Could not load effect resource. Reason: '%s'", e.GetMessage().c_str());
		if (Configuration::ToBool("graphics.developmentmode", false)) {
			return new Effect(0);
		}
		throw e;
	}
}

ResourceObject* EffectResourceLoader::GetDefaultResource()
{
	return nullptr;
}
