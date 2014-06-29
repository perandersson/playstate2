#include "../Memory.h"
#include "XMLDefaultVisitor.h"
#include "../StringUtils.h"
using namespace core;
using namespace tinyxml2;

XMLDefaultVisitor::XMLDefaultVisitor()
{

}

XMLDefaultVisitor::~XMLDefaultVisitor()
{

}

std::string XMLDefaultVisitor::GetLowerCaseName(const tinyxml2::XMLElement& element)
{
	return StringUtils::ToLowerCase(element.Name());
}

std::string XMLDefaultVisitor::GetLowerCaseValue(const tinyxml2::XMLElement& element)
{
	return StringUtils::ToLowerCase(element.GetText());
}

const char* XMLDefaultVisitor::GetAttribute(const tinyxml2::XMLElement& element, const std::string& lowerCaseAttributeName)
{
	static const std::string _default("");
	return GetAttribute(element, lowerCaseAttributeName, _default);
}

const char* XMLDefaultVisitor::GetAttribute(const tinyxml2::XMLElement& element, const std::string& lowerCaseAttributeName,
	const std::string& defaultValue)
{
	auto attribute = element.FirstAttribute();
	while (attribute != nullptr) {
		const std::string name = StringUtils::ToLowerCase(attribute->Name());
		if (name == lowerCaseAttributeName) {
			return attribute->Value();
		}
		attribute = attribute->Next();
	}

	return defaultValue.c_str();
}
