#include "stdafx.h"

Window::Window(HINSTANCE inst, int showCmd) : m_Inst(inst), m_ShowCmd(showCmd) {
	m_WindowW = 1280;
	m_WindowH = 960;
}

Window::~Window() {

}

BOOL Window::init() {
	LoadStringA(m_Inst, IDS_APP_TITLE, m_Title, 100);
	LoadStringA(m_Inst, IDC_PROCEDURALPLANETS, m_Class, 100);

	Register();

	if (!InitInstance())
		return FALSE;

	return TRUE;
}

void Window::run() {
	HACCEL hAccelTable = LoadAcceleratorsA(m_Inst, MAKEINTRESOURCEA(IDC_PROCEDURALPLANETS));

	MSG msg;

	while (GetMessageA(&msg, nullptr, 0, 0))
	{
		if (!TranslateAcceleratorA(msg.hwnd, hAccelTable, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessageA(&msg);
		}
	}
}

BOOL Window::InitInstance() {
	m_Hwnd = CreateWindowA(m_Class, m_Title, WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, 0, m_WindowW, m_WindowH, nullptr, nullptr, m_Inst, nullptr);

	if (!m_Hwnd)
		return FALSE;

	ShowWindow(m_Hwnd, m_ShowCmd);
	UpdateWindow(m_Hwnd);

	return TRUE;
}

ATOM Window::Register() {
	WNDCLASSEXA wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = Window::WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = m_Inst;
	wcex.hIcon = LoadIconA(m_Inst, MAKEINTRESOURCEA(IDI_PROCEDURALPLANETS));
	wcex.hCursor = LoadCursorW(nullptr, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = nullptr;
	wcex.lpszClassName = m_Class;
	wcex.hIconSm = LoadIconA(wcex.hInstance, MAKEINTRESOURCEA(IDI_SMALL));

	return RegisterClassExA(&wcex);
}

LRESULT CALLBACK Window::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
	switch (message) {
		case WM_COMMAND: {
			int wmId = LOWORD(wParam);

			switch (wmId) {
				default:
					return DefWindowProcA(hWnd, message, wParam, lParam);
			}
		}
		break;

		case WM_PAINT: {
			PAINTSTRUCT ps;
			HDC hdc = BeginPaint(hWnd, &ps);
			EndPaint(hWnd, &ps);
		}
		break;

		case WM_DESTROY:
			PostQuitMessage(0);
			break;

		default:
			return DefWindowProcA(hWnd, message, wParam, lParam);
	}
	return 0;
}
