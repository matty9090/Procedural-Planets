#pragma once

#include <Windows.h>
#include <string>

#include "resource.h"
#include "Graphics.hpp"
#include "Input.hpp"

class Window {
	public:
		Window(HINSTANCE inst, int showCmd);
		~Window();

		void setDims(int w, int h);

		BOOL init();
		void run();

		int getWindowWidth() { return m_WindowW; }
		int getWindowHeight() { return m_WindowW; }
		HWND getHandle() { return m_Hwnd; }

	private:
		HINSTANCE m_Inst;
		HWND m_Hwnd;

		size_t m_WindowW, m_WindowH;

		char m_Title[100], m_Class[100];
		int m_ShowCmd;
		
		BOOL InitInstance();
		ATOM Register();

		Graphics *m_Graphics;

		static LRESULT CALLBACK Window::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
};
