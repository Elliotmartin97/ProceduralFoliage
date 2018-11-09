#include "Graphics.h"

bool Graphics::Init(int screen_width, int screen_height, HWND hwnd)
{
	bool result;


	// Create the Direct3D object.
	m_Direct3D = new Direct3D;
	if (!m_Direct3D)
	{
		return false;
	}

	// Initialize the Direct3D object.
	result = m_Direct3D->Init(screen_width, screen_height, VSYNC_ENABLED, hwnd, FULL_SCREEN, SCREEN_DEPTH, SCREEN_NEAR);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize Direct3D", L"Error", MB_OK);
		return false;
	}
	return true;
}

bool Graphics::Frame()
{
	Render();
	return true;
}

bool Graphics::Render()
{

	m_Direct3D->BeginScene(0.5f, 0.5f, 0.5f, 1.0f);
	m_Direct3D->EndScene();

	return true;
}