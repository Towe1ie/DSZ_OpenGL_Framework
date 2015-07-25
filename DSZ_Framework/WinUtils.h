#pragma once

#include <Windows.h>
#include <gl\GL.h>

HWND createWindow(char* title, int width, int height, int bits, bool fullscreenFlag);
bool destroyWindow(HWND hwnd);

bool initExtensions();

HGLRC createContext(HWND hwnd, int major, int minor);
bool destroyContext(HGLRC hRC);