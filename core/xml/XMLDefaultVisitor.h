#pragma once
#include "../typedefs.h"
#include <tinyxml2.h>

namespace core
{
	class XMLDefaultVisitor : public tinyxml2::XMLVisitor
	{
	public:
		XMLDefaultVisitor();
		virtual ~XMLDefaultVisitor();

		//
		// @return The name of the supplied element in lower-case
		static std::string GetLowerCaseName(const tinyxml2::XMLElement& element);

		//
		// @return The value of the supplied element in lower-case
		static std::string GetLowerCaseValue(const tinyxml2::XMLElement& element);

		//
		// @param element
		// @param lowerCaseAttributeName
		//			The attribute name we are looking for in lowercase format
		// @return The attribute 
		static const char* GetAttribute(const tinyxml2::XMLElement& element, const std::string& lowerCaseAttributeName);

		//
		// @param element
		// @param lowerCaseAttributeName
		//			The attribute name we are looking for in lowercase format
		// @param defaultValue
		// @return The attribute 
		static const char* GetAttribute(const tinyxml2::XMLElement& element, const std::string& lowerCaseAttributeName,
			const std::string& defaultValue);
	};
}