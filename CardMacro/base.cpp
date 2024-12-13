#define ENABLE_LISTENER

#include <thread>
#include <windows.h>

#include "Keyboard.hpp"
#include "Macro.hpp"
#include "Overlay.hpp"

//CTRL+113

#ifdef ENABLE_LISTENER

static void macroEntrypointThread(void*) {
	macroEntrypoint();
}

static void listenForCtrlF2(_In_ DWORD vkCode) {
	if (vkCode == VK_F2 && (GetAsyncKeyState(VK_CONTROL) & 0x8000))
		_beginthread(macroEntrypointThread, 0, nullptr);
}

#endif // ENABLE_LISTENER


int APIENTRY WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nCmdShow) {

#if FALSE
	AllocConsole();
	FILE* fp;
	freopen_s(&fp, "CONOUT$", "w", stdout);  // Redirect stdout to the console
#endif

#ifdef ENABLE_LISTENER

	if (!Keyboard::enableListener(listenForCtrlF2))
		return 1;

	MSG msg;
	while (GetMessage(&msg, nullptr, 0, 0)) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	Keyboard::disableListener();

	return 0;

#else

	return macroEntrypoint() ? 0 : 1;

#endif // ENABLE_LISTENER
}