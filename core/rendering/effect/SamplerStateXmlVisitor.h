#pragma once
#include "../../xml/XMLDefaultVisitor.h"
#include "../SamplerObject.h"
#include "../MinFilter.h"
#include "../MagFilter.h"
#include "../TextureWrap.h"
#include "UniformProperty.h"

namespace core
{
	class SamplerStateXmlVisitor : public XMLDefaultVisitor
	{
	public:
		SamplerStateXmlVisitor(const std::string& id);
		virtual ~SamplerStateXmlVisitor();

		/*!
			\brief Retrieves the uniform property for this sampler state

			\return The sampler state property
		*/
		std::shared_ptr<UniformProperty> GetSamplerState();

	// XMLVisitor
	public:
		virtual bool VisitEnter(const tinyxml2::XMLElement& element, const tinyxml2::XMLAttribute* firstAttribute);

	private:
		std::shared_ptr<UniformProperty> mSamplerState;
	};
}
