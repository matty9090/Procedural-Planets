#pragma once

#include "Graphics.hpp"
#include "Timer.h"

class App {
	public:
		App(HWND hwnd, Graphics *graphics);
		~App();

		void run();

	private:
		HWND m_Hwnd;
		Timer m_Timer;
		Graphics *m_Graphics;
};

