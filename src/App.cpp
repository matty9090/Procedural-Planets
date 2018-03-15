#include "App.hpp"
#include "Input.hpp"

#include <sstream>

App::App(HWND hwnd, Graphics *graphics) : m_Hwnd(hwnd), m_Graphics(graphics), m_Speed(2.0f), m_RotSpeed(0.5f), m_Frames(0), m_FrameTimer(0.5f) {
	m_Timer.start();

	m_Camera = new Camera();
	m_Shader = new Shader();
	m_Terrain = new Terrain(graphics->getDevice(), graphics->getDeviceContext(), m_Graphics, m_Shader, m_Camera, 200.0f);

	m_Camera->setPosition(D3DXVECTOR3(0, 0, -400.0f));

	m_LightPos	= D3DXVECTOR3(-40.0f, 0.0f, -800.0f);
	m_LightCol	= D3DXVECTOR3(0.8f, 0.8f, 0.8f);
	m_Ambient	= D3DXVECTOR3(0.1f, 0.1f, 0.1f);

	if (!m_Shader->init(graphics->getDevice(), hwnd)) exit(1);
	if (!m_Terrain->init()) exit(1);
}

void App::run() {
	dt = m_Timer.getLapTime();
	m_Frames++;
	m_FrameTimer -= dt;

	if (m_FrameTimer < 0.0f) {
		m_FPS = m_Frames / 0.5f;
		m_Frames = 0, m_FrameTimer = 0.5f;
	}

	handleInput();
	update();

	m_Graphics->beginScene();
	render();
	m_Graphics->endScene();
}

void App::update() {
	//m_LightPos = m_Camera->getDxPosition();
	m_Terrain->update();

	m_Speed = log((m_Camera->getPosition().distance(Vec3<float>(0.0f, 0.0f, 0.0f)) + 1.0f) / 200.0f) * 200.0f;
}

void App::render() {
	D3DXMATRIX viewMatrix;
	m_Camera->render();
	m_Camera->getViewMatrix(viewMatrix);

	D3DXVECTOR3 camPos = m_Camera->getDxPosition();
	m_Terrain->render(viewMatrix, m_Graphics->getProjectionMatrix(), camPos, m_LightPos, m_LightCol, m_Ambient);

	m_Graphics->drawTextValue(10, 10, "Camera", m_Camera->getDxPosition());
	m_Graphics->drawTextValue(10, 100, "FPS", m_FPS);
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
