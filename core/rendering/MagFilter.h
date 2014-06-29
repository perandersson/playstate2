#pragma once
#include "../typedefs.h"
#include <GL/glew.h>

namespace core
{
	class MagFilter
	{
	public:
		enum Enum {
			NEAREST = 0,
			LINEAR,

			SIZE
		};

		static const Enum DEFAULT = LINEAR;
		
		/*!
			\brief Parse the supplied string and return the enum representing it

			\param s
						The name of the enum
			\param defaultValue
						Value if the supplied string does not map to an enum
		*/
		static Enum Parse(const std::string& s, Enum defaultValue);

		//
		// Convert the supplied enum into a OpenGL enum
		//
		// @param e
		static GLenum Parse(Enum e);
		
		/*!
			\brief Retrieves the values available in this enum

			\return A map matching name with enum value
		*/
		static std::hash_map<std::string, int32> GetValues();
	};
}
