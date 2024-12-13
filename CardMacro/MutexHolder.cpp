#include "MutexHolder.hpp"


MutexHolder::MutexHolder(_In_ LPCWSTR lpName, _In_ bool initiallyHeld) : mutex(CreateMutex(NULL, initiallyHeld, lpName)) {
	this->held = initiallyHeld && GetLastError() == 0;
}

MutexHolder::~MutexHolder() {
	CloseHandle(this->mutex);
}

MutexHolder::operator bool() const {
	return this->mutex != NULL;
}

void MutexHolder::free() {
	if (this->held)
		ReleaseMutex(this->mutex);
}

bool MutexHolder::isHeld() const {
	return this->held;
}

bool MutexHolder::lock() {
	if (this->held)
		return true;

	const bool res = WaitForSingleObject(this->mutex, 0) == WAIT_OBJECT_0;
	if (res)
		this->held = true;
	return res;
}

bool MutexHolder::lockWithRetries(_In_ uint8_t tries) {
	for (int attempt = 0; attempt < tries && !this->lock(); ++attempt);
	return this->held;
}