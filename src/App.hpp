#pragma once

#include "Graphics.hpp"
#include "Timer.h"

#include "Camera.hpp"
#include "Shader.hpp"
#include "Primitive.hpp"
#include "Sphere.hpp"
#include "Terrain.hpp"

class App {
	public:
		App(HWND hwnd, Graphics *graphics);
		~App();

		void run();

	private:
		HWND m_Hwnd;
		Timer m_Timer;

		Graphics *m_Graphics;
		Camera *m_Camera;
		Shader *m_Shader;
		Terrain *m_Terrain;

		D3DXVECTOR3 m_LightPos, m_LightCol, m_Ambient;

		float dt;
		float m_Speed, m_RotSpeed;

		void update();
		void render();
		void handleInput();
};

