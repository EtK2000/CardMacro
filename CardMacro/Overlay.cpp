#include <atomic>
#include <map>
#include <memory>
#include <process.h>
#include <thread>

#include "Gdi.hpp"
#include "Overlay.hpp"


class AtomicBoolLock {
private:
	std::atomic<bool>& atomicBool;

public:
	AtomicBoolLock(std::atomic<bool>& atomicBool) : atomicBool(atomicBool) {
		bool expected = false;
		while (!this->atomicBool.compare_exchange_strong(expected, true))
			expected = false;
	}

	~AtomicBoolLock() {
		this->atomicBool.store(false);
	}
};


struct OverlayData {
	const std::unique_ptr<Gdi::Brush> brush;
	const RECT rect;

	OverlayData(_In_ std::unique_ptr<Gdi::Brush> brush, _In_ const RECT rect) : brush(std::move(brush)), rect(rect) {}
};


static std::atomic<bool> isMapInUse(false);
static std::map<const byte, OverlayData*> overlays;


static void clearOverlay(const RECT rect) {
	InvalidateRect(nullptr, &rect, TRUE);
}


static void renderAllOverlays(const HDC &hdc) {
	const AtomicBoolLock lock(isMapInUse);

	for (const auto& [key, data] : overlays)
		FillRect(hdc, &data->rect, *data->brush);
}

static void renderThreadLoop(void*) {
	const HDC hdc = GetWindowDC(nullptr);

	for (;;) {
		if (overlays.size() > 0)
			renderAllOverlays(hdc);
		else
			std::this_thread::sleep_for(std::chrono::milliseconds(100));
	}

	ReleaseDC(nullptr, hdc);
}


namespace Overlay {
	void add(_In_ byte id, _In_ int x, _In_ int y, _In_ int w, _In_ int h, _In_ COLORREF color) {
		overlays[id] = new OverlayData{
			std::make_unique<Gdi::Brush>(color),
			{x, y, x + w, y + h}
		};
	}

	void remove(_In_ byte id) {
		const AtomicBoolLock lock(isMapInUse);
		auto entry = overlays.extract(id);
		if (entry) {
			clearOverlay(entry.mapped()->rect);
			delete entry.mapped();
		}
	}
}

const unsigned tid = _beginthread(renderThreadLoop, 0, nullptr);