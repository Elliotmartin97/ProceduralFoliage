#include "System.h"

bool System::Init()
{
	int screen_width = 0, screen_height = 0;
	InitializeWindows(screen_height, screen_width);
	key_input.reset(new Input);
	key_input->Init();
	graphics.reset(new Graphics);
	graphics->Init(screen_width, screen_height, hwnd);

	return true;
}

void System::Run()
{
	//runs the loop, checking for inputs, loops continues until exit input

	MSG msg;
	bool done, result;

	ZeroMemory(&msg, sizeof(MSG));

	done = false;
	while (!done)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		if (msg.message == WM_QUIT)
		{
			done = true;
		}
		else
		{
			done = Update();
		}
	}
}

bool System::Update()
{
	if (key_input->IsKeyDown(VK_ESCAPE))
	{
		return true;
	}
	graphics->Update();
	return false;
}

LRESULT CALLBACK System::MessageHandler(HWND hwnd, UINT umsg, WPARAM wparam, LPARAM lparam)
{
	//callback for inputs as well as anttweakbar input
	if (TwEventWin(hwnd, umsg, wparam, lparam))
	{
		return 0;
	}
	switch (umsg)
	{
		case WM_KEYDOWN:
		{
			key_input->KeyDown((unsigned int)wparam);
			return 0;
		}
		case WM_KEYUP:
		{
			key_input->KeyUp((unsigned int)wparam);
			return 0;
		}
		default:
		{
			return DefWindowProc(hwnd, umsg, wparam, lparam);
		}
	}
}

void System::InitializeWindows(int& window_height, int& window_width)
{
	WNDCLASSEX wc;
	DEVMODE screen_settings;
	int pos_width, pos_height;

	app_handle = this;
	h_instance = GetModuleHandle(NULL);
	app_name = L"Procedural Foliage";

	// Settings of the window for the program, borderless and windows
	wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	wc.lpfnWndProc = WndProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = h_instance;
	wc.hIcon = LoadIcon(NULL, IDI_WINLOGO);
	wc.hIconSm = wc.hIcon;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	wc.lpszMenuName = NULL;
	wc.lpszClassName = app_name;
	wc.cbSize = sizeof(WNDCLASSEX);
	RegisterClassEx(&wc);
	window_width = 1280;
	window_height = 960;
	pos_width = (GetSystemMetrics(SM_CXSCREEN) - window_width) / 2;
	pos_height = (GetSystemMetrics(SM_CYSCREEN) - window_height) / 2;
	
	hwnd = CreateWindowEx(WS_EX_APPWINDOW, app_name, app_name,
		WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_POPUP,
		pos_width, pos_height, window_width, window_height, NULL, NULL, h_instance, NULL);

	ShowWindow(hwnd, SW_SHOW);
	SetForegroundWindow(hwnd);
	SetFocus(hwnd);
	ShowCursor(true);
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT umessage, WPARAM wparam, LPARAM lparam)
{
	switch (umessage)
	{
		case WM_DESTROY:
		{
			PostQuitMessage(0);
			return 0;
		}
		case WM_CLOSE:
		{
			PostQuitMessage(0);
			return 0;
		}
		default:
		{
			return app_handle->MessageHandler(hwnd, umessage, wparam, lparam);
		}
	}
}

void System::Shutdown()
{
	DestroyWindow(hwnd);
	hwnd = NULL;
	UnregisterClass(app_name, h_instance);
	h_instance = NULL;
	app_handle = NULL;
}