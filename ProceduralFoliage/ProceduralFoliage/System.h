#pragma once
#ifndef _SYSTEM_H_
#define _SYSTEM_H_

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <memory>

#include "Input.h"
#include "Graphics.h"
class System {
public:
	System() = default;
	~System() = default;

	bool Init();
	void Run();
	void ShutdownWindows();
	LRESULT CALLBACK MessageHandler(HWND, UINT, WPARAM, LPARAM);

private:
	bool Frame();
	void InitializeWindows(int&, int&);
private:
	LPCWSTR app_name;
	HINSTANCE h_instance;
	HWND hwnd;

	std::unique_ptr<Input> input;
	std::unique_ptr<Graphics> graphics;
};

static LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
static System* app_handle = 0;
#endif