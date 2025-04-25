#pragma once

#include "Defines.hpp"
#include <string>
#include <vector>

class IPlatform {
public:
	struct PlatformInfo {
		const std::string& application_name;
		int x, y, width, height;
	};

protected:
	IPlatform() : EnableRender(true) {};

public:
	static IPlatform* GetInstance() { return SingleInstance; }

public:
	virtual bool PlatformStartup(const PlatformInfo& info) = 0;
	virtual void PlatformShutdown() = 0;
	virtual int GetProcessorCount() = 0;
	virtual void PlatformConsoleWrite(const char* message, unsigned char color) = 0;
	virtual void PlatformConsoleWriteError(const char* message, unsigned char color) = 0;
	virtual double PlatformGetAbsoluteTime() = 0;
	virtual bool PlatformPumpMessage() = 0;
	virtual void PlatformSleep(size_t ms) = 0;

public:
	bool GetWindowStatus() const { return EnableRender; }
	void SetWindowStatus(bool status) { EnableRender = status; }

public:
	static void* PlatformAllocate(size_t size, bool aligned) { return malloc(size); }
	static void PlatformFree(void* block, bool aligned) { free(block); }
	static void* PlatformZeroMemory(void* block, size_t size) { return memset(block, 0, size); }
	static void* PlatformCopyMemory(void* dst, const void* src, size_t size) { return memcpy(dst, src, size); }
	static void* PlatformSetMemory(void* dst, int val, size_t size) { return memset(dst, val, size); }

private:
	bool EnableRender;
	static IPlatform* SingleInstance;

};

