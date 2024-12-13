#pragma once


#include <functional>
#include <stdint.h>
#include <Windows.h>


namespace Keyboard {
	using ListenerOnKeyPressFunc = std::function<void(_In_ DWORD)>;

	void disableListener();
	bool enableListener(_In_ ListenerOnKeyPressFunc);
	void press(_In_ WCHAR key, _In_ uint16_t durationInMillis = 100, _In_opt_ HWND target = nullptr);
}