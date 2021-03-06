#pragma once

#include <Windows.h>
#include <string>

#include "resource.h"
#include "App.hpp"
#include "Input.hpp"

class Window {
	public:
		Window(HINSTANCE inst, int showCmd);
		~Window();

		void setDims(int w, int h);

		BOOL init();
		void run();

		size_t getWindowWidth() { return m_WindowW; }
		size_t getWindowHeight() { return m_WindowH; }
		HWND getHandle() { return m_Hwnd; }

	private:
		HINSTANCE m_Inst;
		HWND m_Hwnd;
		Graphics *m_Graphics;

		size_t m_WindowW, m_WindowH;

		char m_Title[100], m_Class[100];
		int m_ShowCmd;
		
		BOOL InitInstance();
		ATOM Register();

		static LRESULT CALLBACK Window::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
};
