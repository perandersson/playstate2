#pragma once
#include "../typedefs.h"
#include "GLEWMX.h"

namespace core
{
	class Texture;

	/*!
		\brief Structure containing the information used by the RenderState to manage RenderTargets
	*/
	struct RenderTargetInfo
	{
		uint32 uid;
		Texture* texture;
		GLenum attachmentType;
		GLenum textureTarget;
		bool dirty;

		RenderTargetInfo()
			: uid(0), texture(nullptr), attachmentType(0), textureTarget(0), dirty(false)
		{
		
		}

		RenderTargetInfo(const RenderTargetInfo& rhs)
			: uid(rhs.uid), texture(rhs.texture), attachmentType(rhs.attachmentType), textureTarget(rhs.textureTarget), dirty(rhs.dirty)
		{

		}

		~RenderTargetInfo()
		{

		}
	};
}
