#pragma once
#ifndef _GRAPHICS_H_
#define _GRAPHICS_H_

#include <windows.h>

#include "Direct3D.h"
#include "Camera.h"
#include "Model.h"
#include "Input.h"
#include "DefaultShader.h"
#include "DiffuseLight.h"
#include "Turtle.h"

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
	bool Render();
private:

	Direct3D* m_Direct3D;
	Camera* m_camera;
	Turtle* turtle;
	DefaultShader* m_default_shader;
	DiffuseLight* m_Light;
};

#endif