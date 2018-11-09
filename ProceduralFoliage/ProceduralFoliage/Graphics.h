#pragma once
#ifndef _GRAPHICS_H_
#define _GRAPHICS_H_

#include <windows.h>

//global values
const bool FULL_SCREEN = false;
const bool VSYNC_ENABLED = true;
const float SCREEN_DEPTH = 1000.0f;
const float SCREEN_NEAR = 0.1f;

class Graphics
{
public:
	Graphics() = default;
	~Graphics() = default;

	bool Init(int screen_width, int screen_height, HWND hwnd);
	bool Frame();

private:
	bool Render();

};

#endif