#pragma once


#include <windows.h>


namespace Overlay {
	void add(_In_ byte id, _In_ int x, _In_ int y, _In_ int w, _In_ int h, _In_ COLORREF color);
	void remove(_In_ byte id);


	class Overlay {
	private:
		const byte id;

	public:
		Overlay(_In_ byte id, _In_ int x, _In_ int y, _In_ int w, _In_ int h, _In_ COLORREF color) : id(id) {
			add(id, x, y, w, h, color);
		}

		~Overlay() {
			remove(id);
		}
	};
}