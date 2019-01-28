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
#include "AntTweakBar.h"
#include "Turtle.h"
#include "LSystem.h"

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


	std::string GetFileName() { return load_file_name; };
	bool Init(int screen_width, int screen_height, HWND hwnd);
	void LoadTypeFile(std::string file_name);
	void Refresh();
	bool Frame();
	bool Render();
private:

	Direct3D* m_Direct3D;
	Camera* m_camera;
	Turtle* turtle;
	DefaultShader* m_default_shader;
	DiffuseLight* m_Light;
	TwBar* loader_bar;
	std::string load_file_name;
};

#endif