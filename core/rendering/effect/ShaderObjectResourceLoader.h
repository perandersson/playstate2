#pragma once
#include "../RenderContextResourceLoader.h"

namespace core
{
	class ShaderObjectResourceLoader : public RenderContextResourceLoader
	{
	public:
		ShaderObjectResourceLoader(GLenum type);
		virtual ~ShaderObjectResourceLoader();

		virtual ResourceObject* Load(const IFile* file);
		virtual ResourceObject* GetDefaultResource();

	private:
		GLenum mType;
	};
}
