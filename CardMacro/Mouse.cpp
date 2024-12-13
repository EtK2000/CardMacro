#include <thread>

#include "Mouse.hpp"


class ClickHolder {
private:
	const HWND target;
	const Mouse::Button mouseButton;

public:
	ClickHolder(_In_ const Mouse::Button& mouseButton, _In_opt_ const HWND target) : mouseButton(mouseButton), target(target) {
		if (target != nullptr)
			PostMessage(target, this->mouseButton.WM_BUTTONDOWN, this->mouseButton.MK_BUTTON, 0);
		else
			mouse_event(this->mouseButton.MOUSEEVENTF_DOWN, 0, 0, 0, 0);
	}

	~ClickHolder() {
		if (target != nullptr)
			PostMessage(target, this->mouseButton.WM_BUTTONUP, 0, 0);
		else
			mouse_event(this->mouseButton.MOUSEEVENTF_UP, 0, 0, 0, 0);
	}
};


namespace Mouse {
	Mouse::Button LEFT{
		MOUSEEVENTF_LEFTDOWN , MOUSEEVENTF_LEFTUP,
		MK_LBUTTON, WM_LBUTTONDOWN, WM_LBUTTONUP
	};
	Mouse::Button RIGHT{
		MOUSEEVENTF_RIGHTDOWN , MOUSEEVENTF_RIGHTUP,
		MK_RBUTTON, WM_RBUTTONDOWN, WM_RBUTTONUP
	};

	void click(_In_ const Mouse::Button& mouseButton, _In_ uint8_t durationInSeconds, _In_opt_ HWND target) {
		const ClickHolder click(mouseButton, target);
		std::this_thread::sleep_for(std::chrono::seconds(durationInSeconds));
	}
}