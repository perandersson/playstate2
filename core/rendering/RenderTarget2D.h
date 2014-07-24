#pragma once
#include "Texture2D.h"
#include <mutex>

namespace core
{
	class RenderTarget2D : public Texture2D
	{
	public:
		RenderTarget2D(GLuint textureID, const Size& size, TextureFormat::Enum format);
		virtual ~RenderTarget2D();

		/*!
			\brief Locks this render target.

			This is to ensure that only one thread is rendering to the same RenderTarget at the same time
		*/
		void Lock();

		/*!
			\brief Unlocks this render target

			This is to ensure that only one thread is rendering to the same RenderTarget at the same time
		*/
		void Unlock();

	private:
		std::recursive_mutex mMutex;
	};
}
