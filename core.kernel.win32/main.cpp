#ifdef _DEBUG
#include <float.h>
// Unterupt for when floating points become invalid
// NOT USABLE WHEN USING gDEBugger
unsigned int fp_control_state = _controlfp(_EM_INEXACT | _EM_INVALID | _EM_UNDERFLOW | _EM_OVERFLOW, _MCW_EM);
#endif

#include <core/Memory.h>
#include <iostream>
#include <core/game/IGame.h>
#include "kernel/Win32Kernel.h"
using namespace core;

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

	auto kernel = new Win32Kernel();
	if (kernel->Initialize()) {
		assert(gGameFactory != nullptr && "You have to specify the game with a PLAY_GAME define");
		auto game = gGameFactory->Create(kernel);
		kernel->StartGame(game);
	}

	//
	// Destroy the game engine kernel
	//

	kernel->Release();
	delete kernel;

	return 0;
}

