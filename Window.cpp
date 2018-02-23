#include "Window.hpp"
#include "Timer.h"

Window::Window(HINSTANCE inst, int showCmd) : m_Inst(inst), m_ShowCmd(showCmd) {
	m_WindowW = 1024;
	m_WindowH = 768;

	Input::init();
}

Window::~Window() {

}

void Window::setDims(int w, int h) {
	m_WindowW = w;
	m_WindowH = h;
}

BOOL Window::init() {
	LoadStringA(m_Inst, IDS_APP_TITLE, m_Title, 100);
	LoadStringA(m_Inst, IDC_PROCEDURALPLANETS, m_Class, 100);

	Register();

	if (!InitInstance())
		return FALSE;

	m_Graphics = new Graphics(this);

	if (!m_Graphics->init())
		return FALSE;

	return TRUE;
}

void Window::run() {
	HACCEL hAccelTable = LoadAcceleratorsA(m_Inst, MAKEINTRESOURCEA(IDC_PROCEDURALPLANETS));

	MSG msg = {0};

	Timer timer;
	timer.start();

	while (WM_QUIT != msg.message) {
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		} else {
			if (Input::KeyHit(Input::Key_Escape))
				DestroyWindow(m_Hwnd);

			float dt = timer.getLapTime();

			m_Graphics->beginScene();
			m_Graphics->endScene();
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
		case WM_PAINT: {
			PAINTSTRUCT ps;
			HDC hdc = BeginPaint(hWnd, &ps);
			EndPaint(hWnd, &ps);
		}
		break;

		case WM_DESTROY:
			PostQuitMessage(0);
			break;

		case WM_KEYDOWN:
			Input::KeyDownEvent(static_cast<Input::EKeyCode>(wParam));
			break;

		case WM_KEYUP:
			Input::KeyUpEvent(static_cast<Input::EKeyCode>(wParam));
			break;

		case WM_LBUTTONDOWN:
			Input::KeyDownEvent(Input::Mouse_LButton);
			break;
		case WM_LBUTTONUP:
			Input::KeyUpEvent(Input::Mouse_LButton);
			break;

		case WM_MBUTTONDOWN:
			Input::KeyDownEvent(Input::Mouse_MButton);
			break;
		case WM_MBUTTONUP:
			Input::KeyUpEvent(Input::Mouse_MButton);
			break;

		case WM_RBUTTONDOWN:
			Input::KeyDownEvent(Input::Mouse_RButton);
			break;
		case WM_RBUTTONUP:
			Input::KeyUpEvent(Input::Mouse_RButton);
			break;

		default:
			return DefWindowProcA(hWnd, message, wParam, lParam);
	}

	return 0;
}
