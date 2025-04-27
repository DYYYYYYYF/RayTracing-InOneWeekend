#pragma once

#include "IPlatform.hpp"

#ifdef DPLATFORM_WINDOWS

#include <windows.h>
#include <windowsx.h>

#ifdef VULKAN_ENABLED
#include <vulkan/vulkan.hpp>
#include <vulkan/vulkan_win32.h>
#endif

class Windows32 : public IPlatform{
public:
	virtual bool PlatformStartup(const PlatformInfo& info) override;
	virtual void PlatformShutdown() override;
	virtual int GetProcessorCount() override;
	virtual void PlatformConsoleWrite(const char* message, unsigned char color) override;
	virtual void PlatformConsoleWriteError(const char* message, unsigned char color) override;
	virtual double PlatformGetAbsoluteTime() override;
	virtual bool PlatformPumpMessage() override;
	virtual void PlatformSleep(size_t ms) override;

	bool InitOpenGLContext();
	void SwapBuffer() { SwapBuffers(m_hDC); }
	HWND GetHWND() { return hwnd; }

private:
	HINSTANCE h_instance;
	HWND hwnd;

#ifdef VULKAN_ENABLED
	// Vulkan handle
	vk::SurfaceKHR surface;
#endif

	// OpenGL handle
	HGLRC m_hRC;
	HDC m_hDC;

};

#endif