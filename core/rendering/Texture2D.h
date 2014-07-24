#pragma once
#include "Texture.h"
#include "../math/Color.h"

namespace core
{
	class Texture2D : public Texture
	{
	public:
		Texture2D(GLuint textureID, const Size& size, TextureFormat::Enum format);
		virtual ~Texture2D();

		/*!
			\brief Resizes this texture

			\param newSize
		*/
		void Resize(const Size& newSize);
		
		/*!
			\brief Update this texture with new data. 
			
			The data is assumed to have the same size and format as before

			\param size
			\param memory
		*/
		void Update(Color* pixels);

		/*!
			\brief Update this texture with new data. 
			
			The data is assumed to have the same format as before

			\param size
			\param memory
		*/
		void Update(const Size& size, Color* pixels);
	};
}
