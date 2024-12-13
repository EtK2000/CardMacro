#pragma once


#include <utility>
#include <Windows.h>


namespace Gdi {

	class Brush {
	private:
		const HBRUSH brush;

	public:
		Brush(_In_ const COLORREF color) : brush(std::move(CreateSolidBrush(color))) {}

		~Brush() {
			DeleteObject(this->brush);
		}

		operator const HBRUSH() const {
			return this->brush;
		}
	};

}