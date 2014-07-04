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
			\brief Add a new uniform property

			\param p
		*/
		void AddUniformProperty(std::shared_ptr<UniformProperty> p);

		/*!
			\brief Retrieves this effects uniform properties
		*/
		inline const std::hash_map<std::string, std::shared_ptr<UniformProperty>>& GetUniformProperties() const {
			return mProperties;
		}

		inline void SetDepthTest(bool enable) {
			mDepthTest = enable;
		}

		inline bool GetDepthTest() const {
			return mDepthTest;
		}

		inline void SetDepthFunc(DepthFunc::Enum func) {
			mDepthFunc = func;
		}

		inline DepthFunc::Enum GetDepthFunc() const {
			return mDepthFunc;
		}

		inline void SetStencilTest(bool enable) {
			mStencilTest = enable;
		}

		inline bool GetStencilTest() const {
			return mStencilTest;
		}

		inline void SetBlend(bool enable) {
			mBlend = enable;
		}

		inline bool GetBlend() const {
			return mBlend;
		}

		inline void SetBlendFunc(SrcFactor::Enum sfactor, DestFactor::Enum dfactor) {
			mBlendFunc.sfactor = sfactor;
			mBlendFunc.dfactor = dfactor;
		}

		inline const BlendFunc& GetBlendFunc() const {
			return mBlendFunc;
		}

		inline void SetCullFace(CullFace::Enum cullFace) {
			mCullFace = cullFace;
		}

		inline CullFace::Enum GetCullFace() const {
			return mCullFace;
		}

		inline void SetClearColor(const Color& color) {
			mClearColor = color;
		}

		inline const Color& GetClearColor() const {
			return mClearColor;
		}

		inline void SetClearDepth(float32 depth) {
			mClearDepth = depth;
		}

		inline float32 GetClearDepth() const {
			return mClearDepth;
		}

	private:
		bool IsActive() const;

	private:
		GLuint mProgramID;
		UniformProperties mProperties;


		bool mDepthTest;
		DepthFunc::Enum mDepthFunc;
		bool mStencilTest;
		bool mBlend;
		BlendFunc mBlendFunc;
		CullFace::Enum mCullFace;
		Color mClearColor;
		float32 mClearDepth;
		bool mApplyRenderTarget;
	};
}
