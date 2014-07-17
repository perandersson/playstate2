#pragma once
#include "../typedefs.h"
#include "../ThreadLocal.h"
#include "../math/Rect.h"
#include "../math/Color.h"
#include "DepthFunc.h"
#include "ClearType.h"
#include "BlendFunc.h"
#include "FrontFace.h"
#include "CullFace.h"
#include "PolygonMode.h"
#include "effect/EffectState.h"
#include "IUniform.h"
#include "RenderTargetInfo.h"
#include "GLEWMX.h"
#include <mutex>

namespace core
{
	class Effect;
	class EffectState;
	class Texture;
	class RenderTarget2D;
	class SamplerObject;
	class VertexBuffer;
	class IndexBuffer;

	/*!
		\brief Responsible for managing the current thread's state of the rendering engine
	*/
	class RenderState : public ThreadLocal<RenderState*>
	{
		typedef std::hash_map<uint32, std::shared_ptr<EffectState>> EffectStates;

	public:
		RenderState();
		~RenderState();

		/*!
			\brief Clear the view-port based on this effects assigned clear- depth and color.
			
			\param clearBits
		*/
		void Clear(uint32 clearBits);

		/*!
			\brief Bind the supplied effect to this rendering state

			\param effect
		*/
		EffectState* ApplyEffect(const Effect* effect);
		
		/*!
			\brief Draw the supplied buffer object using this graphics program.

			\param buffer
		*/
		void Render(const VertexBuffer* buffer);

		/*!
			\brief Draw the supplied buffer object using the supplied index buffer using this graphics program.

			\param buffer
			\param indexBuffer
		*/
		void Render(const VertexBuffer* buffer, const IndexBuffer* indexBuffer);

		/*!
			\brief Draw the supplied buffer object using the supplied index buffer using this graphics program.

			\param buffer
					The vertex buffer we want to draw. Cannot be nullptr.
			\param indexBuffer
					The indices of the supplied buffer we want to draw. Can be nullptr
			\param startIndex
					The index of the first vertex we want to draw
		*/
		void Render(const VertexBuffer* buffer, const IndexBuffer* indexBuffer, uint32 startIndex);

		/*!
			\brief Draw the supplied buffer object using the supplied index buffer using this graphics program.

			\param buffer
					The vertex buffer we want to draw. Cannot be nullptr.
			\param indexBuffer
					The indices of the supplied buffer we want to draw. Can be nullptr
			\param startIndex
					The index of the first vertex we want to draw
			\param numVertices
					How many vertices we want to draw
		*/
		void Render(const VertexBuffer* buffer, const IndexBuffer* indexBuffer, uint32 startIndex, uint32 numVertices);

		/*!
			\brief Bind the supplied vertex buffer

			\param vertexBuffer
		*/
		void BindVertexBuffer(const VertexBuffer* vertexBuffer);
		
		/*!
			\brief Bind the supplied index buffer

			\param indexBuffer
		*/
		void BindIndexBuffer(const IndexBuffer* indexBuffer);

		/*!
			\brief Set the viewport for the current rendering state
			
			\param viewport
		*/
		void SetViewport(const Rect& viewport);

		/*!
			\brief Enable- or disable depth testing for vertices

			\param enable
		*/
		void SetDepthTest(bool enable);
		
		/*!
			\brief Set how the depth is calculated for each vertice

			\param func
		*/
		void SetDepthFunc(DepthFunc::Enum func);

		/*!
			\brief Enables/Disables writing to the Z-Buffer

			\param mask
		*/
		void SetDepthMask(bool mask);

		/*!
			\brief Enable- or disable stencil testing for vertices

			\param enable
		*/
		void SetStencilTest(bool enable);

		/*!
			\brief Set the stencil mask

			\param mask
		*/
		void SetStencilMask(uint32 mask);

		/*!
			\brief Enable- or disable blending

			\param enable
		*/
		void SetBlend(bool enable);

		/*!
			\brief Set the blending function

			Blending function is used by the graphics driver to calculate the resulting color
			after the src- and destination value has been taken into consideration

			\param sfactor
			\param dfactor
		*/
		void SetBlendFunc(SrcFactor::Enum sfactor, DestFactor::Enum dfactor);

		/*!
			\brief Set which vertices to cull when draw more complex primitives

			\param frontFace
		*/
		void SetFrontFace(FrontFace::Enum frontFace);
		
		/*!
			\brief Set which vertices to cull when draw more complex primitives

			\param frontFace
		*/
		void SetCullFace(CullFace::Enum cullFace);

		/*!
			\brief Set color used when clearing the render state

			\param color
		*/
		void SetClearColor(const Color& color);

		/*!
			\brief Set the value used when cleaning the depth buffer

			\param depth
		*/
		void SetClearDepth(float32 depth);

		/*!
			\brief Bind the supplied texture to the given index

			\param texture
			\param index
		*/
		void BindTexture(const Texture* texture, uint32 index);

		/*!
			\brief Bind the sampler object at the given index

			\param samplerObject
			\param index
		*/
		void BindSampler(SamplerObject* samplerObject, uint32 index);

		/*!
			\brief Sets the mode for the upcomming polygons

			\param mode
					The polygon mode
		*/
		void SetPolygonMode(PolygonMode::Enum mode);


		/*!
			\brief Set the render target at the given index

			\remark Render targets are not actually bound until you perform some kind of
					rendering-action, such as clearing the screen.

			\param renderTarget
						The texture we want to draw our result to
			\param index
						The texture index for the render target. Useful if you want to
						render to multiple textures at once.
		*/
		void SetRenderTarget(const RenderTarget2D* renderTarget, uint32 index);
		
		/*!
			\brief Bind a side of the render target cube at the given index

			\remark Render targets are not actually bound until you perform some kind of
					rendering-action, such as clearing the screen.

			\param renderTarget
						The texture we want to draw our result to
			\param side
						The side of the cubed texture we want to draw our result to
			\param index
						The texture index for the render target. Useful if you want to
						render to multiple textures at once.
		*/
		void SetRenderTarget(const RenderTargetCube* renderTarget, TextureCubeSide::Enum side, uint32 index);
		
		/*!
			\brief Set the render target at the given index

			\remark Render targets are not actually bound until you perform some kind of
					rendering-action, such as clearing the screen.

			\param renderTarget
						The texture we want to draw our result to
			\param index
						The texture index for the render target. Useful if you want to
						render to multiple textures at once.
		*/
		void SetRenderTarget(const RenderTargetCube* renderTarget, uint32 index);

		/*!
			\brief Set the depth render target

			\remark Render targets are not bound until you perform some kind of
					rendering-action, such as clearing the screen.

			\param renderTarget
						The texture we want to draw our depth result to
		*/
		void SetDepthRenderTarget(const RenderTarget2D* renderTarget);
		
		/*!
			\brief Set the depth render target

			\remark Render targets are not bound until you perform some kind of
					rendering-action, such as clearing the screen.

			\param renderTarget
						The texture we want to draw our depth result to
		*/
		void SetDepthRenderTarget(const RenderTargetCube* renderTarget, TextureCubeSide::Enum side);

		/*!
			\brief Retrieves a uniform based on it's name in the shader bound to this thread.

			\param name
						The name of the uniform variable
			\name The uniform if found; A pointer to an UniformNotFound instance if not found.
		*/
		IUniform* FindUniform(const char* name);
		
		/*!
			\brief Retrieves a uniform based on it's name in the shader bound to this thread.

			\param name
						The name of the uniform variable
			\name The uniform if found; A pointer to an UniformNotFound instance if not found.
		*/
		IUniform* FindUniform(const std::string& name);

		/*!
			\brief Check to see if the supplied effect UID is the one bound to the current rendering state
		*/
		inline bool IsActive(uint32 effectUID) const {
			return effectUID == mEffectUID;
		}

		/*!
			\brief
		*/
		inline uint32 GetMaxActiveTextures() const {
			return mMaxActiveTextures;
		}

		/*!
			\brief
		*/
		inline uint32 GetMaxDrawBuffers() const {
			return mMaxDrawBuffers;
		}

		/*!
			\brief
		*/
		uint32 GetNextTextureIndex();

	private:
		bool IsRenderTargetsEnabled() const;
		void ApplyRenderTargets();
		void ApplyBuffers(const VertexBuffer* buffer, const IndexBuffer* indexBuffer);
		EffectState* GetEffectState(const Effect* effect);
		void InvalidateRenderTargets();
		
		/*!
			\brief Bind the supplied effect to this rendering state

			\param effect
		*/
		void BindEffect(const Effect* effect);

		static GLenum GetDepthFuncAsEnum(DepthFunc::Enum depthFunc);
		static GLenum GetSrcFactorAsEnum(SrcFactor::Enum sfactor);
		static GLenum GetDestFactorAsEnum(DestFactor::Enum dfactor);
		static GLenum GetFrontFaceAsEnum(FrontFace::Enum frontFace);
		static GLenum GetCullFaceAsEnum(CullFace::Enum cullFace);
		static GLenum GetPolygonModeAsEnum(PolygonMode::Enum mode);


	friend class OpenGLRenderContext;
	friend class XMLEffectType;
	friend class VertexBuffer;
	friend class IndexBuffer;
	protected:
		/*!
			\brief Unbinds the currently bound index buffer

			This method is useful after the render context has created a new index buffer (which unintentionally binds it to the render state)
			forcing the next index buffer to be bound
		*/
		void UnbindIndexBuffer();
		
		/*!
			\brief Unbinds the currently bound vertex buffer

			This method is useful after the render context has created a new vertex buffer (which unintentionally binds it to the render state)
			forcing the next vertex buffer to be bound
		*/
		void UnbindVertexBuffer();
		
		/*!
			\brief Unbinds the currently bound texture

			This method is useful after the render context has created a new texture (which unintentionally binds it to the render state)
			forcing the next texture to be bound
		*/
		void UnbindTexture();
		
		/*!
			\brief Unbinds the currently bound effect

			This method is useful after the render context has created a new effect (which unintentionally binds it to the render state)
			forcing the next texture to be bound
		*/
		void UnbindEffect();

	friend class Effect;
	protected:
		/*!
			\brief Check to see if the supplied effect is active

			\param effect
		*/
		bool IsEffectActive(const Effect* effect) const;
		
	private:
		Rect mViewport;

		//
		// Effect
		//

		uint32 mEffectUID;

		//
		// Buffers
		//

		uint32 mVertexBufferUID;
		uint32 mIndexBufferUID;
		GLuint mVertexArrayID;

		//
		// Depth states
		//

		bool mDepthTest;
		DepthFunc::Enum mDepthFunc;
		bool mDepthMask;

		//
		// Stencil test
		//
		
		bool mStencilTest;
		uint32 mStencilMask;

		//
		// Blending
		//

		bool mBlend;
		BlendFunc mBlendFunc;

		//
		// Vretex culling
		//

		FrontFace::Enum mFrontFace;
		CullFace::Enum mCullFace;

		//
		// Clear states
		//

		Color mClearColor;
		float mClearDepth;

		//
		// Bound textures
		//

		std::vector<uint32> mTextureUID;
		std::vector<GLenum> mTextureTarget;
		std::vector<uint32> mSamplerObjectUID;
		uint32 mActiveTextureIndex;

		//
		// Polygon mode
		//
		PolygonMode::Enum mPolygonMode;

		//
		// Bound render targets
		//
		std::vector<RenderTargetInfo> mRenderTargetInfo;
		RenderTargetInfo mDepthRenderTargetInfo;
		GLuint mFrameBufferID;
		bool mApplyRenderTarget;
		bool mFrameBufferApplied;

		//
		// Uniforms
		//

		EffectState* mEffectState;
		EffectStates mEffectStates;
		bool mApplyEffectState;

		//
		// Resource limit
		//

		uint32 mMaxDrawBuffers;
		uint32 mMaxActiveTextures;
		uint32 mNextTextureIndex;
		
	};
}
