#pragma once
#include "../../typedefs.h"
#include "../../resource/ResourceObject.h"
#include "../GLEWMX.h"

namespace core
{
	/*!
		\brief Class representing a shader object (geometry-, vertex- or fragment shader)
	*/
	class ShaderObject : public ResourceObject
	{
	public:
		ShaderObject(GLuint shaderID, GLenum shaderType);
		virtual ~ShaderObject();

		/*!
			\brief Retrieves the shader ID for this shader object

			\return The shader ID
		*/
		inline GLuint GetShaderID() const {
			return mShaderID;
		}

		/*!
			\brief Retrieves what type of shader this is
		*/
		inline GLenum GetShaderType() const {
			return mShaderType;
		}

	private:
		GLuint mShaderID;
		GLenum mShaderType;
	};
}
