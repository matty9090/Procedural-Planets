#include "App.hpp"
#include "Input.hpp"

App::App(HWND hwnd, Graphics *graphics) : m_Hwnd(hwnd), m_Graphics(graphics), m_Speed(2.0f), m_RotSpeed(0.5f) {
	m_Timer.start();

	m_Camera = new Camera();
	m_Shader = new Shader();
	m_Terrain = new Terrain(graphics->getDevice(), graphics->getDeviceContext(), m_Shader, m_Camera, 10.0f);

	m_Camera->setPosition(D3DXVECTOR3(0, 0, -20.0f));

	if (!m_Shader->init(graphics->getDevice(), hwnd)) exit(1);
	if (!m_Terrain->init()) exit(1);
}

void App::run() {
	dt = m_Timer.getLapTime();

	handleInput();
	update();

	m_Graphics->beginScene();
	render();
	m_Graphics->endScene();
}

void App::update() {
	m_Terrain->update();

	m_Speed = log(m_Camera->getPosition().distance(Vec3<float>(0.0f, 0.0f, 0.0f)) / 10.0f + 0.01f) * 20.0f;
}

void App::render() {
	D3DXMATRIX viewMatrix;

	m_Camera->render();
	m_Camera->getViewMatrix(viewMatrix);

	m_Terrain->render(viewMatrix, m_Graphics->getProjectionMatrix());
}

void App::handleInput() {
	if (Input::KeyHit(Input::Key_Escape)) DestroyWindow(m_Hwnd);
	if (Input::KeyHit(Input::Key_F3))     m_Graphics->toggleWireframe();

	if (Input::KeyHeld(Input::Key_W))     m_Camera->moveZ(dt * m_Speed);
	if (Input::KeyHeld(Input::Key_S))     m_Camera->moveZ(-dt * m_Speed);
	if (Input::KeyHeld(Input::Key_D))     m_Camera->moveX(dt * m_Speed);
	if (Input::KeyHeld(Input::Key_A))     m_Camera->moveX(-dt * m_Speed);

	if (Input::KeyHeld(Input::Key_Up))    m_Camera->rotate(D3DXVECTOR3(-dt * m_RotSpeed, 0.0f, 0.0f));
	if (Input::KeyHeld(Input::Key_Down))  m_Camera->rotate(D3DXVECTOR3(dt * m_RotSpeed, 0.0f, 0.0f));
	if (Input::KeyHeld(Input::Key_Left))  m_Camera->rotate(D3DXVECTOR3(0.0f, -dt * m_RotSpeed, 0.0f));
	if (Input::KeyHeld(Input::Key_Right)) m_Camera->rotate(D3DXVECTOR3(0.0f, dt * m_RotSpeed, 0.0f));
}

App::~App() {
	m_Terrain->cleanup();
	m_Shader->cleanup();

	delete m_Camera;
	delete m_Shader;
	delete m_Terrain;
}
