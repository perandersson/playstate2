#pragma once
#include "../typedefs.h"
#include "../scene/Scene.h"
#include "../camera/Camera.h"
#include "IRenderContext.h"

namespace core
{
	/*!
		\brief Class representing 
	*/
	class IRenderPipeline
	{
	public:
		virtual ~IRenderPipeline() {}

		/*!
			\brief Method called when the the supplied scene should be drawn.

			\param scene
			\param camera
		*/
		virtual void Render(const Scene& scene, const Camera* camera) = 0;
	};

}