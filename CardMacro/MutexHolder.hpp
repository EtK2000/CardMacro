#pragma once


#include <stdint.h>
#include <windows.h>


class MutexHolder {
private:
	const HANDLE mutex;
	bool held;

public:
	MutexHolder(_In_ LPCWSTR lpName, _In_ bool initiallyHeld);
	~MutexHolder();

	operator bool() const;

	void free();
	bool isHeld() const;
	bool lock();
	bool lockWithRetries(_In_ uint8_t tries);
};