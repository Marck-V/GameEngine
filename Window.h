#pragma once
#include <Window.h>


class Window
{
	public:
	Window();
	// Initialize the window
	bool init();
	// Release the window
	bool release();
	~Window();
};

