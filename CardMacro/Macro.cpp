#include <thread>
#include <windows.h>

#include "Keyboard.hpp"
#include "Mouse.hpp"
#include "MutexHolder.hpp"
#include "Overlay.hpp"

#include "Macro.hpp"


constexpr auto RUNNING_MUTEX_NAME = L"$$CARD_MACRO::MUTEX::RUNNING$$";
constexpr auto TERMINATION_MUTEX_NAME = L"$$CARD_MACRO::MUTEX::TERMINATION$$";

constexpr auto OVERLAY_RUNNING = 0;
constexpr auto OVERLAY_STOPPING = 1;


static bool mutexIsAvailable(_In_ LPCWSTR lpName) {
	return MutexHolder(lpName, true).isHeld();
}

bool macroEntrypoint() {
	MutexHolder singletonMutex(RUNNING_MUTEX_NAME, true);

	if (!singletonMutex)
		return false;

	if (!singletonMutex.isHeld()) {

		// notify the first instance to terminate
		MutexHolder mutex(TERMINATION_MUTEX_NAME, true);
		if (!mutex.lockWithRetries(10))
			return false;


		// wait for first instance to terminate
		const Overlay::Overlay overlay(OVERLAY_STOPPING, 200, 0, 200, 200, RGB(0, 255, 0));
		while (!singletonMutex.lock())
			std::this_thread::sleep_for(std::chrono::milliseconds(500));

		return true;
	}


	// run until notified to terminate
	const Overlay::Overlay overlay(OVERLAY_RUNNING, 0, 0, 200, 200, RGB(255, 0, 0));
	while (mutexIsAvailable(TERMINATION_MUTEX_NAME)) {
		Mouse::click(Mouse::RIGHT, 2);
		Keyboard::press('R', 100);
		Mouse::click(Mouse::LEFT, 40);

		std::this_thread::sleep_for(std::chrono::milliseconds(100));
	}

	return true;
}