#include <core/Memory.h>
#include <iostream>
#include <core/playstate.h>
#include <demo/DemoGame.h>

#ifdef _DEBUG
int main(int argc, char** argv)
#else
#include <Windows.h>
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR cmdLine, int cmdShow)
#endif
{
#ifdef _DEBUG
#ifdef WIN32
	// Enable windows memory leak detection when running in debug mode
	int tmpFlag = _CrtSetDbgFlag(_CRTDBG_REPORT_FLAG);
	tmpFlag |= _CRTDBG_LEAK_CHECK_DF;
	_CrtSetDbgFlag(tmpFlag);
#endif
#endif

	//
	// Create the game engine kernel
	//

	auto kernel = CreateKernel(nullptr);

	//
	// Initialize and start the game
	//

	if (kernel->Initialize())
		kernel->StartGame(new DemoGame());

	//
	// Destroy the game engine kernel
	//

	kernel->Release();
	DestroyKernel(kernel);

	//
	// Register resource type loaders
	//

	return 0;
}

