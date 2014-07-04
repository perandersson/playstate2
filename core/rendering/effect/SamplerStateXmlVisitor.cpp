#include "../../Memory.h"
#include "SamplerStateXmlVisitor.h"
#include "../../xml/TagsAndAttributes.h"
#include "../../StringUtils.h"
using namespace core;

SamplerStateXmlVisitor::SamplerStateXmlVisitor(const std::string& id)
: XMLDefaultVisitor()
{
	mSamplerState = std::shared_ptr<UniformProperty>(new UniformProperty());
	mSamplerState->name = id;
}

SamplerStateXmlVisitor::~SamplerStateXmlVisitor()
{

}

std::shared_ptr<UniformProperty> SamplerStateXmlVisitor::GetSamplerState()
{
	return mSamplerState;
}

bool SamplerStateXmlVisitor::VisitEnter(const tinyxml2::XMLElement& element, const tinyxml2::XMLAttribute* firstAttribute)
{
	const std::string name = GetLowerCaseName(element);
	if (name == TAG_MIN_FILTER) {
		mSamplerState->minFilter = MinFilter::Parse(GetLowerCaseValue(element), MinFilter::DEFAULT);
	}
	else if (name == TAG_MAG_FILTER) {
		mSamplerState->magFilter = MagFilter::Parse(GetLowerCaseValue(element), MagFilter::DEFAULT);
	}
	else if (name == TAG_WRAP_S) {
		mSamplerState->wraps = TextureWrap::Parse(GetLowerCaseValue(element), TextureWrap::DEFAULT);
	}
	else if (name == TAG_WRAP_T) {
		mSamplerState->wrapt = TextureWrap::Parse(GetLowerCaseValue(element), TextureWrap::DEFAULT);
	}
	else if (name == TAG_WRAP_R) {
		mSamplerState->wrapr = TextureWrap::Parse(GetLowerCaseValue(element), TextureWrap::DEFAULT);
	}

	return true;
}
