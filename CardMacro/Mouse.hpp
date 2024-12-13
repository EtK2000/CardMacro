#pragma once


#include <stdint.h>
#include <Windows.h>


namespace Mouse {
	struct Button {
		const short MOUSEEVENTF_DOWN, MOUSEEVENTF_UP,
			MK_BUTTON, WM_BUTTONDOWN, WM_BUTTONUP;
	} extern LEFT, RIGHT;

	void click(_In_ const Mouse::Button& mouseButton, _In_ uint8_t durationInSeconds, _In_opt_ HWND target = nullptr);
}