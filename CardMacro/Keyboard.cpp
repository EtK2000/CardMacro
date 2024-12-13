#include <thread>

#include "Keyboard.hpp"


HHOOK g_keyboardHook = nullptr;
Keyboard::ListenerOnKeyPressFunc onKeyPressFunc = nullptr;


static LRESULT CALLBACK KeyboardProc(const int nCode, const WPARAM wParam, const LPARAM lParam) {
	if (nCode < 0)
		return CallNextHookEx(g_keyboardHook, nCode, wParam, lParam);

	if (wParam == WM_KEYDOWN || wParam == WM_SYSKEYDOWN) {
		const KBDLLHOOKSTRUCT* const pKeyboard = reinterpret_cast<KBDLLHOOKSTRUCT*>(lParam);
		onKeyPressFunc(pKeyboard->vkCode);
	}

	return CallNextHookEx(g_keyboardHook, nCode, wParam, lParam);
}


class KeyPressHolder {
private:
	const short key;
	const HWND target;

public:
	KeyPressHolder(_In_ const WCHAR key, _In_opt_ const HWND target) : key(VkKeyScan(key)), target(target) {
		if (target != nullptr)
			PostMessage(target, WM_KEYDOWN, key, 0);
		else
			keybd_event(key, 0, 0, 0);
	}

	~KeyPressHolder() {
		if (target != nullptr)
			PostMessage(target, WM_KEYUP, key, 0);
		else
			keybd_event(key, 0, KEYEVENTF_KEYUP, 0);
	}
};


namespace Keyboard {
	void disableListener() {
		if (g_keyboardHook != nullptr) {
			UnhookWindowsHookEx(g_keyboardHook);
			g_keyboardHook = nullptr;
			onKeyPressFunc = nullptr;
		}
	}

	bool enableListener(_In_ ListenerOnKeyPressFunc listener) {
		if (g_keyboardHook == nullptr) {
			g_keyboardHook = SetWindowsHookEx(WH_KEYBOARD_LL, KeyboardProc, nullptr, 0);
			if (g_keyboardHook == nullptr)
				return false;
		}
		onKeyPressFunc = listener;
		return true;
	}

	void press(_In_ WCHAR key, _In_ uint16_t durationInMillis, _In_opt_ HWND target) {
		const KeyPressHolder click(key, target);
		std::this_thread::sleep_for(std::chrono::milliseconds(durationInMillis));
	}
}