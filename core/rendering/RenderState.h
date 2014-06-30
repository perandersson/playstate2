#pragma once
#include "../typedefs.h"
#include "../ThreadLocal.h"
#include "../math/Rect.h"
#include "../math/Color.h"
#include "DepthFunc.h"
#include "ClearType.h"
#include "BlendFunc.h"
#include "CullFace.h"
#include "GraphicsCapabilities.h"
#include "effect/EffectState.h"
#include "IUniform.h"
#include <gl/glew.h>
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
		EffectState* BindEffect(const Effect* effect);

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

			\param cullFace
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
			\brief Set the render target at the given index

			\remark Note that the render targets are not bound until you perform some kind of
					rendering-action, such as clearing the screen.

			\param renderTarget
						The texture we want to draw our result to
			\param index
						The texture index for the render target. Useful if you want to
						render to multiple textures at once.
		*/
		void SetRenderTarget(const RenderTarget2D* renderTarget, GLenum index);
		
		/*!
			\brief Set the depth render target at the given index

			\remark Note that the render targets are not bound until you perform some kind of
					rendering-action, such as clearing the screen.

			\param renderTarget
						The texture we want to draw our depth result to
		*/
		void SetDepthRenderTarget(const RenderTarget2D* renderTarget);
		
		/*!
			\brief Retrieves a uniform based on it's name in the shader bound to this thread.

			\param name
						The name of the uniform variable
			\name The uniform if found; A pointer to an UniformNotFound instance if no component is found.
		*/
		IUniform* FindUniform(const char* name);
		
		/*!
			\brief Retrieves a uniform based on it's name in the shader bound to this thread.

			\param name
						The name of the uniform variable
			\name The uniform if found; A pointer to an UniformNotFound instance if no component is found.
		*/
		IUniform* FindUniform(const std::string& name);

		/*!
			\brief
		*/
		inline bool IsActive(uint32 effectUID) const {
			return effectUID == mEffectUID;
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

		static GLenum GetDepthFuncAsEnum(DepthFunc::Enum depthFunc);
		static GLenum GetSrcFactorAsEnum(SrcFactor::Enum sfactor);
		static GLenum GetDestFactorAsEnum(DestFactor::Enum dfactor);
		static GLenum GetCullFaceAsEnum(CullFace::Enum cullFace);


	friend class OpenGLRenderContext;
	friend class XMLEffectType;
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

		//
		// Blending
		//

		bool mBlend;
		BlendFunc mBlendFunc;

		//
		// Vretex culling
		//

		CullFace::Enum mCullFace;

		//
		// Clear states
		//

		Color mClearColor;
		float mClearDepth;

		//
		// Bound textures
		//

		uint32 mTextureUID[MAX_ACTIVE_TEXTURES];
		GLenum mTextureTarget[MAX_ACTIVE_TEXTURES];
		uint32 mActiveTextureIndex;
		uint32 mSamplerObjectUID[MAX_ACTIVE_TEXTURES];

		//
		// Bound render targets
		//

		uint32 mRenderTargetUID[MAX_RENDER_TARGETS];
		const RenderTarget2D* mRenderTargets[MAX_RENDER_TARGETS];
		const RenderTarget2D* mDepthRenderTarget;
		GLenum mDepthRenderTargetType;
		uint32 mDepthRenderTargetUID;
		GLuint mFrameBufferID;
		bool mApplyRenderTarget;
		bool mFrameBufferApplied;

		//
		// Uniforms
		//

		EffectState* mEffectState;
		EffectStates mEffectStates;
		uint32 mNextTextureIndex;
		GLuint mMaxTextureIndexes;
		bool mApplyEffectState;
	};
}
