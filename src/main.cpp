#include <sdk/SexySDK.hpp>
#include <callbacks/callbacks.hpp>
#include "logger/logger.hpp"
#include "input/input.hpp"

void init()
{
	logger::init("gamepad");

	if(SDL_Init(SDL_INIT_EVERYTHING) != 0)
	{
		PRINT_ERROR("Error initalizing SDL2 (%s)", SDL_GetError());
		return;
	}

	input::init();

	callbacks::on(callbacks::type::main_loop, []()
	{
		input::update();
	});
}

DWORD WINAPI OnAttachImpl(LPVOID lpParameter)
{
	init();
	return 0;
}

DWORD WINAPI OnAttach(LPVOID lpParameter)
{
	__try
	{
		return OnAttachImpl(lpParameter);
	}
	__except (0)
	{
		FreeLibraryAndExitThread((HMODULE)lpParameter, 0xDECEA5ED);
	}

	return 0;
}

BOOL WINAPI DllMain(HMODULE hModule, DWORD dwReason, LPVOID lpReserved)
{
	switch (dwReason)
	{
	case DLL_PROCESS_ATTACH:
		DisableThreadLibraryCalls(hModule);
		CreateThread(nullptr, 0, OnAttach, hModule, 0, nullptr);
		return true;
	}

	return false;
}
