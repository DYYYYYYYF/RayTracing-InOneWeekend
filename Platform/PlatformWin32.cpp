#include "PlatformWin32.hpp"

#if defined(DPLATFORM_WINDOWS)
#include <GL/gl.h>
#include "wglext.h"
#include <imgui.h>
#include <imgui_impl_win32.h>

IPlatform* IPlatform::SingleInstance = new Windows32();
PFNWGLCREATECONTEXTATTRIBSARBPROC wglCreateContextAttribsARB = nullptr;

static double ClockFrequency;
static LARGE_INTEGER StartTime;

LRESULT CALLBACK win32_process_message(HWND hwnd, UINT32 msg, WPARAM w_param, LPARAM l_param);

bool Windows32::PlatformStartup(const PlatformInfo& info) {
	
	h_instance = GetModuleHandleA(0);

	// Setup & register window class
	HICON icon = LoadIcon(h_instance, IDI_APPLICATION);
	WNDCLASSA wc;
	memset(&wc, 0, sizeof(wc));
	wc.style = CS_DBLCLKS;						// Get double clicks
	wc.lpfnWndProc = win32_process_message;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = h_instance;
	wc.hIcon = icon;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);	// NULL; // Mange cursor
	wc.hbrBackground = NULL;					// Transparent
	wc.lpszClassName = "dimension_window_class";

	if (!RegisterClassA(&wc)) {
		MessageBoxA(0, "Register window failed", "Error", MB_ICONEXCLAMATION | MB_OK);
		return false;
	}

	// Create window
	unsigned int ClientX = info.x;
	unsigned int ClientY = info.y;
	unsigned int ClientWidth = info.width;
	unsigned int ClientHeight = info.height;

	unsigned int WindowX = ClientX;
	unsigned int WindowY = ClientY;
	unsigned int WindowWidth = ClientWidth;
	unsigned int WindowHeight = ClientHeight;

	unsigned int WindowStyle = WS_OVERLAPPED | WS_SYSMENU | WS_CAPTION;
	unsigned int WindowExStyle = WS_EX_APPWINDOW;

	WindowStyle |= WS_MAXIMIZEBOX;
	WindowStyle |= WS_MINIMIZEBOX;
	WindowStyle |= WS_THICKFRAME;

	// Obtain the size of border
	RECT BorderRect = { 0, 0, 0, 0 };
	AdjustWindowRectEx(&BorderRect, WindowStyle, 0, WindowExStyle);

	WindowX += BorderRect.left;
	WindowY += BorderRect.top;

	WindowWidth += BorderRect.right - BorderRect.left;
	WindowHeight += BorderRect.bottom - BorderRect.top;

	HWND Handle = CreateWindowExA(WindowExStyle, "dimension_window_class", info.application_name.c_str(),
		WindowStyle, WindowX, WindowY, WindowWidth, WindowHeight, 0, 0, h_instance, 0);

	if (Handle == 0) {
		MessageBoxA(NULL, "Create window failed!", "Error", MB_ICONEXCLAMATION | MB_OK);
		return false;
	}
	else {
		hwnd = Handle;
	}

	// Display
	bool ShouldActivate = true;	// TODO: it will should not accept input if false
	int DisplayWindowCommandFlags = ShouldActivate ? SW_SHOW : SW_SHOWNOACTIVATE;
	//int DisplayWindowCommandFlags = ShouldActivate ? SW_SHOWMAXIMIZED : SW_MAXIMIZE;
	ShowWindow(hwnd, DisplayWindowCommandFlags);

	// Clock setup
	LARGE_INTEGER Frequency;
	QueryPerformanceFrequency(&Frequency);
	ClockFrequency = 1.0 / (double)Frequency.QuadPart;
	QueryPerformanceCounter(&StartTime);

	return true;
}

void Windows32::PlatformShutdown() {
	DestroyWindow(hwnd);
	hwnd = 0;

	wglDeleteContext(m_hRC);
	ReleaseDC(hwnd, m_hDC);
}

bool Windows32::PlatformPumpMessage() {
	MSG message;
	while (PeekMessageA(&message, NULL, 0, 0, PM_REMOVE)) {
		TranslateMessage(&message);
		DispatchMessageA(&message);
	}

	return true;
}

void Windows32::PlatformConsoleWrite(const char* message, unsigned char color) {
	HANDLE ConsoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
	static unsigned char levels[6] = { 64, 4, 6, 2, 1, 8 };
	SetConsoleTextAttribute(ConsoleHandle, levels[color]);
	OutputDebugStringA(message);
	unsigned long long Length = strlen(message);
	LPDWORD NumberWritten = 0;
	WriteConsoleA(GetStdHandle(STD_OUTPUT_HANDLE), message, (DWORD)Length, NumberWritten, 0);
	SetConsoleTextAttribute(ConsoleHandle, 8);
}

void Windows32::PlatformConsoleWriteError(const char* message, unsigned char color) {
	HANDLE ConsoleHandle = GetStdHandle(STD_ERROR_HANDLE);
	static unsigned char levels[6] = { 64, 4, 6, 2, 1, 8 };
	SetConsoleTextAttribute(ConsoleHandle, levels[color]);
	OutputDebugStringA(message);
	unsigned long long Length = strlen(message);
	LPDWORD NumberWritten = 0;
	WriteConsoleA(GetStdHandle(STD_ERROR_HANDLE), message, (DWORD)Length, NumberWritten, 0);
	SetConsoleTextAttribute(ConsoleHandle, 8);
}

double Windows32::PlatformGetAbsoluteTime() {
	LARGE_INTEGER CurrentTime;
	QueryPerformanceCounter(&CurrentTime);
	return (double)CurrentTime.QuadPart * ClockFrequency;
}

void Windows32::PlatformSleep(size_t ms) {
	Sleep((DWORD)ms);
}

int Windows32::GetProcessorCount() {
	SYSTEM_INFO SystemInfo;
	GetSystemInfo(&SystemInfo);
	return SystemInfo.dwNumberOfProcessors;
}

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

LRESULT CALLBACK win32_process_message(HWND hwnd, UINT32 msg, WPARAM w_param, LPARAM l_param) {
	if (ImGui_ImplWin32_WndProcHandler(hwnd, msg, w_param, l_param)) {return true;}

	switch (msg) {
		case WM_ERASEBKGND:
			return 1;
		case WM_CLOSE:
			IPlatform::GetInstance()->SetWindowStatus(false);
			return 1;
		case WM_DESTROY:
			PostQuitMessage(0);
			return 0;
		case WM_SIZE: {
			RECT Rect;
			GetClientRect(hwnd, &Rect);
			unsigned int Width = Rect.right - Rect.left;
			unsigned int Height = Rect.bottom - Rect.top;

			break;
		}
		case WM_KEYDOWN:
		case WM_SYSKEYDOWN:
		case WM_KEYUP:
		case WM_SYSKEYUP: {
			// Key pressed/released
			bool pressed = (msg == WM_KEYDOWN || msg == WM_SYSKEYDOWN);
			if (w_param == VK_ESCAPE) {
				IPlatform::GetInstance()->SetWindowStatus(false);
			}

		} break;
		case WM_MOUSEMOVE: {
			int PositionX = GET_X_LPARAM(l_param);
			int PositionY = GET_Y_LPARAM(l_param);

		}break;
		case WM_MOUSEWHEEL: {
			int DeltaZ = GET_WHEEL_DELTA_WPARAM(w_param);
			if (DeltaZ != 0) {
				DeltaZ = (DeltaZ < 0) ? -1 : 1;
			}
		}break;
		case WM_LBUTTONDOWN:
		case WM_MBUTTONDOWN:
		case WM_RBUTTONDOWN:
		case WM_LBUTTONUP:
		case WM_MBUTTONUP:
		case WM_RBUTTONUP: {


		}break;
	} 

	return DefWindowProc(hwnd, msg, w_param, l_param);
}


/**
 * OpenGL platform
 */
bool Windows32::InitOpenGLContext() {
	m_hDC = GetDC(hwnd);

	//初始化像素格式
	static    PIXELFORMATDESCRIPTOR pfd =
	{
		sizeof(PIXELFORMATDESCRIPTOR),    //上述格式描述符的大小
		1,                                //版本号
		PFD_DRAW_TO_WINDOW |              //格式支持窗口
		PFD_SUPPORT_OPENGL |              //格式必须支持OpenGL
		PFD_DOUBLEBUFFER,                 //必须支持双缓冲
		PFD_TYPE_RGBA,                    //申请RGBA 格式
		24,                               //选定色彩深度
		0, 0, 0, 0, 0, 0, 0, 0,           //忽略RGBA
		0,                                //无累加缓存
		0, 0, 0, 0,                       //忽略聚集位
		32,                               //32位Z-缓存(深度缓存)
		0,                                //无蒙板缓存
		0,                                //无辅助缓存
		PFD_MAIN_PLANE,                   //主绘图层
		0,                                //Reserved
		0, 0, 0                           //忽略层遮罩
	};

	//选择一个最适合pfd描述的像素格式索引值
	int nPixelFormat = ChoosePixelFormat(m_hDC, &pfd);

	//为DC设置像素格式
	SetPixelFormat(m_hDC, nPixelFormat, &pfd);

	//创建RC
	HGLRC tempContext = wglCreateContext(m_hDC);
	wglMakeCurrent(m_hDC, tempContext);

	// 2. 加载 wglCreateContextAttribsARB
	wglCreateContextAttribsARB = (PFNWGLCREATECONTEXTATTRIBSARBPROC)wglGetProcAddress("wglCreateContextAttribsARB");

	// 3. 创建真正的现代 OpenGL 上下文
	int attribs[] = {
		WGL_CONTEXT_MAJOR_VERSION_ARB, 4, // OpenGL 4.x
		WGL_CONTEXT_MINOR_VERSION_ARB, 6,
		WGL_CONTEXT_PROFILE_MASK_ARB, WGL_CONTEXT_CORE_PROFILE_BIT_ARB, // Core Profile
		WGL_CONTEXT_FLAGS_ARB, WGL_CONTEXT_DEBUG_BIT_ARB, // 可选：启用调试
		0
	};

	m_hRC = wglCreateContextAttribsARB(m_hDC, 0, attribs);
	wglMakeCurrent(m_hDC, m_hRC);
	wglDeleteContext(tempContext); // 删除临时上下文

	return true;
}

#endif	//DPLATFORM_WINDOWS
