#pragma once
#include "../../xml/XMLDefaultVisitor.h"
#include "../SamplerObject.h"
#include "../FrontFace.h"
#include "../CullFace.h"
#include "../DepthFunc.h"
#include "../BlendFunc.h"
#include "../IRenderContext.h"
#include "UniformProperty.h"

namespace core
{
	class Effect;

	class EffectXmlVisitor : public XMLDefaultVisitor
	{
		typedef std::hash_map<std::string, std::shared_ptr<UniformProperty>> UniformProperties;

	public:
		EffectXmlVisitor(IRenderContext* renderContext);
		virtual ~EffectXmlVisitor();

		/*!
			\brief Retrieves the actual effect created by this visitor
		*/
		Effect* GetEffect();

	// XMLVisitor
	public:
		virtual bool VisitEnter(const tinyxml2::XMLElement& element, const tinyxml2::XMLAttribute* firstAttribute);
		virtual bool VisitExit(const tinyxml2::XMLElement& element);

	private:
		IRenderContext* mRenderContext;
		Effect* mEffect;
		GLuint mVertexShaderID;
		GLuint mFragmentShaderID;
		GLuint mGeometryShaderID;
		UniformProperties mProperties;

		bool mDepthTest;
		DepthFunc::Enum mDepthFunc;

		bool mStencilTest;
		uint32 mStencilMask;

		bool mBlend;
		SrcFactor::Enum mSrcFactor;
		DestFactor::Enum mDestFactor;

		FrontFace::Enum mFrontFace;
		CullFace::Enum mCullFace;
	};
}
