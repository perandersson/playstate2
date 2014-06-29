#pragma once
#include "IGame.h"

namespace core
{
	class Game
	{
	public:
		static void Stop();
		static void SetRenderPipeline(IRenderPipeline* renderPipeline);
		static Scene* GetScene();
	};
}
