#include "App.hpp"
#include "Input.hpp"

App::App(HWND hwnd, Graphics *graphics) : m_Hwnd(hwnd), m_Graphics(graphics) {
	m_Timer.start();
}

void App::run() {
	float dt = m_Timer.getLapTime();

	if (Input::KeyHit(Input::Key_Escape)) DestroyWindow(m_Hwnd);
	if (Input::KeyHit(Input::Key_F3)) m_Graphics->toggleWireframe();

	if (Input::KeyHeld(Input::Key_W)) m_Graphics->getCamera()->move(dt * 1.0f);
	if (Input::KeyHeld(Input::Key_S)) m_Graphics->getCamera()->move(-dt * 1.0f);

	if (Input::KeyHeld(Input::Key_Up)) m_Graphics->getCamera()->rotate(D3DXVECTOR3(-dt * 1.0f, 0.0f, 0.0f));
	if (Input::KeyHeld(Input::Key_Down)) m_Graphics->getCamera()->rotate(D3DXVECTOR3(dt * 1.0f, 0.0f, 0.0f));
	if (Input::KeyHeld(Input::Key_Left)) m_Graphics->getCamera()->rotate(D3DXVECTOR3(0.0f, -dt * 1.0f, 0.0f));
	if (Input::KeyHeld(Input::Key_Right)) m_Graphics->getCamera()->rotate(D3DXVECTOR3(0.0f, dt * 1.0f, 0.0f));

	m_Graphics->render();
}

App::~App() {

}
