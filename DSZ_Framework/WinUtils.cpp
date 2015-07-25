#include <glew.h>
#include <wglew.h>
#include "WinUtils.h"
#include "Input.h"
#include "Globals.h"
#include <gl\GL.h>
#include <gl\GLU.h>


HWND createWindow(char* title, int width, int height, int bits, bool fullscreenFlag)
{
	WNDCLASS wc;
	DWORD dwExStyle;
	DWORD dwStyle;
	HWND hWnd;

	wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;  // Redraw On Move, And Own DC For Window
	wc.lpfnWndProc = Input::WndProc;				// WndProc Handles Messages
	wc.cbClsExtra = 0;								// No Extra Window Data
	wc.cbWndExtra = 0;								// No Extra Window Data
	wc.hInstance = hInstance;						// Set The Instance
	wc.hIcon = LoadIcon(NULL, IDI_WINLOGO);			// Load The Default Icon
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);       // Load The Arrow Pointer
	wc.hbrBackground = NULL;						// No Background Required For GL
	wc.lpszMenuName = NULL;							// We Don't Want A Menu
	wc.lpszClassName = "OpenGL";					// Set The Class Name

	if (!RegisterClass(&wc))
	{
		MessageBox(NULL, "Failed To Register The Window Class.", "ERROR", MB_OK | MB_ICONEXCLAMATION);
		return NULL;
	}

	if (fullscreenFlag)
	{
		DEVMODE dmScreenSettings;
		memset(&dmScreenSettings, 0, sizeof(dmScreenSettings));
		dmScreenSettings.dmSize = sizeof(dmScreenSettings);
		dmScreenSettings.dmPelsWidth = width;            // Selected Screen Width
		dmScreenSettings.dmPelsHeight = height;           // Selected Screen Height
		dmScreenSettings.dmBitsPerPel = bits;             // Selected Bits Per Pixel
		dmScreenSettings.dmFields = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;

		if (ChangeDisplaySettings(&dmScreenSettings, CDS_FULLSCREEN) != DISP_CHANGE_SUCCESSFUL)
		{

			if (MessageBox(NULL, "The Requested Fullscreen Mode Is Not Supported By\nYour Video Card. Use Windowed Mode Instead?", "ERROR", MB_YESNO | MB_ICONEXCLAMATION) == IDYES)
			{
				fullscreenFlag = false;
			}
			else
			{
				MessageBox(NULL, "Program Will Now Close.", "ERROR", MB_OK | MB_ICONSTOP);
				return NULL;
			}
		}
	}


	if (fullscreenFlag)
	{
		dwExStyle = WS_EX_APPWINDOW;
		dwStyle = WS_POPUP;
		//ShowCursor(FALSE);
	}
	else
	{
		dwExStyle = WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;
		dwStyle = dwStyle = (WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX | WS_MAXIMIZEBOX);
	}

	if (!(hWnd = CreateWindowEx(dwExStyle,  // Extended Style For The Window
		"OpenGL",							// Class Name
		title,								// Window Title
		WS_CLIPSIBLINGS |					// Required Window Style
		WS_CLIPCHILDREN |					// Required Window Style
		dwStyle,							// Selected Window Style
		0, 0,								// Window Position
		width,								// Calculate Adjusted Window Width
		height,								// Calculate Adjusted Window Height
		NULL,								// No Parent Window
		NULL,								// No Menu
		hInstance,							// Instance
		NULL)))
	{
		destroyWindow(hWnd);
		MessageBox(NULL, "Window Creation Error.", "ERROR", MB_OK | MB_ICONEXCLAMATION);
		return NULL;
	}

	return hWnd;
}

bool destroyWindow(HWND hwnd)
{
	HDC hDC = GetDC(hwnd);
	if (hDC && !ReleaseDC(hwnd, hDC))
	{
		MessageBox(NULL, "Release Device Context Failed.", "SHUTDOWN ERROR", MB_OK | MB_ICONINFORMATION);
		hDC = NULL;
	}

	if (hwnd && !DestroyWindow(hwnd))
	{
		MessageBox(NULL, "Could Not Release hWnd.", "SHUTDOWN ERROR", MB_OK | MB_ICONINFORMATION);
		hwnd = NULL;
	}

	if (!UnregisterClass("OpenGL", hInstance))
	{
		MessageBox(NULL, "Could Not Unregister Class.", "SHUTDOWN ERROR", MB_OK | MB_ICONINFORMATION);
		hInstance = NULL;
	}
	return true;
}

bool initExtensions()
{
	GLuint PixelFormat;

	HWND hwnd = createWindow("temp", 1, 1, 32, false);
	HDC hDC;
	HGLRC hRC;

	static  PIXELFORMATDESCRIPTOR pfd = // pfd Tells Windows How We Want Things To Be
	{
		sizeof(PIXELFORMATDESCRIPTOR),  // Size Of This Pixel Format Descriptor
		1,                              // Version Number
		PFD_DRAW_TO_WINDOW |            // Format Must Support Window
		PFD_SUPPORT_OPENGL |            // Format Must Support OpenGL
		PFD_DOUBLEBUFFER,               // Must Support Double Buffering
		PFD_TYPE_RGBA,                  // Request An RGBA Format
		32,                           // Select Our Color Depth
		0, 0, 0, 0, 0, 0,               // Color Bits Ignored
		0,                              // No Alpha Buffer
		0,                              // Shift Bit Ignored
		0,                              // No Accumulation Buffer
		0, 0, 0, 0,                     // Accumulation Bits Ignored
		24,                             // 16Bit Z-Buffer (Depth Buffer)
		0,                              // No Stencil Buffer
		0,                              // No Auxiliary Buffer
		PFD_MAIN_PLANE,                 // Main Drawing Layer
		0,                              // Reserved
		0, 0, 0                         // Layer Masks Ignored
	};

	if (!(hDC = GetDC(hwnd)))
	{
		destroyWindow(hwnd);
		MessageBox(NULL, "Can't Create A GL Device Context.", "ERROR", MB_OK | MB_ICONEXCLAMATION);
		return FALSE;
	}

	if (!(PixelFormat = ChoosePixelFormat(hDC, &pfd)))
	{
		destroyWindow(hwnd);
		MessageBox(NULL, "Can't Find A Suitable PixelFormat.", "ERROR", MB_OK | MB_ICONEXCLAMATION);
		return FALSE;
	}

	if (!SetPixelFormat(hDC, PixelFormat, &pfd))
	{
		destroyWindow(hwnd);
		MessageBox(NULL, "Can't Set The PixelFormat.", "ERROR", MB_OK | MB_ICONEXCLAMATION);
		return FALSE;
	}

	if (!(hRC = wglCreateContext(hDC)))
	{
		destroyWindow(hwnd);
		MessageBox(NULL, "Can't Create A GL Rendering Context.", "ERROR", MB_OK | MB_ICONEXCLAMATION);
		return FALSE;
	}

	if (!wglMakeCurrent(hDC, hRC))
	{
		destroyWindow(hwnd);
		MessageBox(NULL, "Can't Activate The GL Rendering Context.", "ERROR", MB_OK | MB_ICONEXCLAMATION);
		return FALSE;
	}

	glewInit();

	wglChoosePixelFormatARB = (PFNWGLCHOOSEPIXELFORMATARBPROC)wglGetProcAddress("wglChoosePixelFormatARB");
	wglCreateContextAttribsARB = (PFNWGLCREATECONTEXTATTRIBSARBPROC)wglGetProcAddress("wglCreateContextAttribsARB");

	destroyWindow(hwnd);

	return true;
}

HGLRC createContext(HWND hwnd, int major, int minor)
{
	PIXELFORMATDESCRIPTOR pfd;
	HGLRC hRC;
	HDC hDC = GetDC(hwnd);

	const int attribList[] =
	{
		WGL_DRAW_TO_WINDOW_ARB, GL_TRUE,
		WGL_SUPPORT_OPENGL_ARB, GL_TRUE,
		WGL_DOUBLE_BUFFER_ARB, GL_TRUE,
		WGL_PIXEL_TYPE_ARB, WGL_TYPE_RGBA_ARB,
		WGL_COLOR_BITS_ARB, 32,
		WGL_DEPTH_BITS_ARB, 24,
		WGL_STENCIL_BITS_ARB, 8,
		0,        //End
	};

	int pixelFormat;
	UINT numFormats;

	wglChoosePixelFormatARB(hDC, attribList, NULL, 1, &pixelFormat, &numFormats);

	if (!SetPixelFormat(hDC, pixelFormat, &pfd))
	{
		MessageBox(NULL, "Can't Set The PixelFormat.", "ERROR", MB_OK | MB_ICONEXCLAMATION);
		return 0;
	}

	int attribListCtx[] = {
		WGL_CONTEXT_MAJOR_VERSION_ARB, major,
		WGL_CONTEXT_MINOR_VERSION_ARB, minor,
		0
	};

	hRC = wglCreateContextAttribsARB(hDC, NULL, attribListCtx);

	if (!wglMakeCurrent(hDC, hRC))
	{
		destroyContext(hRC);
		MessageBox(NULL, "Can't Activate The GL Rendering Context.", "ERROR", MB_OK | MB_ICONEXCLAMATION);
		return 0;
	}

	ShowWindow(hwnd, SW_SHOW);
	SetForegroundWindow(hwnd);
	SetFocus(hwnd);

	return hRC;
}

bool destroyContext(HGLRC hRC)
{
	if (hRC)
	{
		if (!wglMakeCurrent(NULL, NULL))
		{
			MessageBox(NULL, "Release Of DC And RC Failed.", "SHUTDOWN ERROR", MB_OK | MB_ICONINFORMATION);
			return false;
		}

		if (!wglDeleteContext(hRC))
		{
			MessageBox(NULL, "Release Rendering Context Failed.", "SHUTDOWN ERROR", MB_OK | MB_ICONINFORMATION);
			return false;
		}
	}

	return true;
}