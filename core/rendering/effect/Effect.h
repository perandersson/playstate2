#pragma once
#include "../../typedefs.h"
#include "../../resource/ResourceObject.h"
#include "../DepthFunc.h"
#include "../BlendFunc.h"
#include "../CullFace.h"
#include "../SamplerObject.h"
#include "../../math/Color.h"
#include "UniformProperty.h"
#include "../exception/RenderingException.h"

namespace core
{
	class VertexBuffer;
	class IndexBuffer;
	class RenderTarget2D;

	class Effect : public ResourceObject
	{
		typedef std::hash_map<std::string, std::shared_ptr<UniformProperty>> UniformProperties;

	public:
		Effect(GLuint programID);
		~Effect();

		/*!
			\brief Retrieves this programs ID
		*/
		inline GLuint GetProgramID() const {
			return mProgramID;
		}

		/*!
			\brief Retrieves this effects uniform properties
		*/
		inline const std::hash_map<std::string, std::shared_ptr<UniformProperty>>& GetUniformProperties() const {
			return mProperties;
		}

		inline bool GetDepthTest() const {
			return mDepthTest;
		}

		inline DepthFunc::Enum GetDepthFunc() const {
			return mDepthFunc;
		}

		inline bool GetStencilTest() const {
			return mStencilTest;
		}

		inline uint32 GetStencilMask() const {
			return mStencilMask;
		}

		inline bool GetBlend() const {
			return mBlend;
		}

		inline const BlendFunc& GetBlendFunc() const {
			return mBlendFunc;
		}

		inline CullFace::Enum GetCullFace() const {
			return mCullFace;
		}

		inline const Color& GetClearColor() const {
			return mClearColor;
		}

		inline float32 GetClearDepth() const {
			return mClearDepth;
		}

	friend class EffectXmlVisitor;
	private:
		/*!
			\brief Add a new uniform property used by this effect

			\param p
		*/
		void AddUniformProperty(std::shared_ptr<UniformProperty> p);

		inline void SetDepthTest(bool enable) {
			mDepthTest = enable;
		}

		inline void SetDepthFunc(DepthFunc::Enum func) {
			mDepthFunc = func;
		}

		inline void SetStencilTest(bool enable) {
			mStencilTest = enable;
		}

		inline void SetStencilMask(uint32 mask) {
			mStencilMask = mask;
		}

		inline void SetBlend(bool enable) {
			mBlend = enable;
		}

		inline void SetBlendFunc(SrcFactor::Enum sfactor, DestFactor::Enum dfactor) {
			mBlendFunc.sfactor = sfactor;
			mBlendFunc.dfactor = dfactor;
		}

		inline void SetCullFace(CullFace::Enum cullFace) {
			mCullFace = cullFace;
		}

		inline void SetClearColor(const Color& color) {
			mClearColor = color;
		}

		inline void SetClearDepth(float32 depth) {
			mClearDepth = depth;
		}

	private:
		bool IsActive() const;

	private:
		GLuint mProgramID;
		UniformProperties mProperties;

		bool mDepthTest;
		DepthFunc::Enum mDepthFunc;
		
		bool mStencilTest;
		uint32 mStencilMask;

		bool mBlend;
		BlendFunc mBlendFunc;

		CullFace::Enum mCullFace;

		Color mClearColor;
		float32 mClearDepth;
	};
}
