#pragma once
#ifndef _Direct3D_H_
#define _Direct3D_H_

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3dcompiler.lib")

#include <d3d11.h>
#include <directxmath.h>
using namespace DirectX;

class Direct3D
{
public:
	Direct3D() = default;
	~Direct3D() = default;

	bool Init(int, int, bool, HWND, bool, float, float);

	void BeginScene(float, float, float, float);
	void EndScene();
	void Shutdown();

	ID3D11Device* GetDevice();
	ID3D11DeviceContext* GetDeviceContext();

	void GetProjectionMatrix(XMMATRIX&);
	void GetWorldMatrix(XMMATRIX&);
	void GetOrthoMatrix(XMMATRIX&);

	void GetVideoCardInfo(char*, int&);

	void TurnOnAlphaBlending();
	void TurnOffAlphaBlending();

private:

	bool m_vsync_enabled;
	int m_videoCardMemory;
	char m_videoCardDescription[128];
	IDXGISwapChain* m_swap_chain;
	ID3D11Device* m_device;
	ID3D11DeviceContext* device_context;
	ID3D11RenderTargetView* m_render_target_view;
	ID3D11Texture2D* m_depth_stencil_buffer;
	ID3D11DepthStencilState* m_depth_stencil_state;
	ID3D11DepthStencilView* m_depth_stencil_view;
	ID3D11RasterizerState* m_raster_state;
	XMMATRIX m_projectionMatrix;
	XMMATRIX m_worldMatrix;
	XMMATRIX m_orthoMatrix;
	ID3D11BlendState* m_alphaEnableBlendingState;
	ID3D11BlendState* m_alphaDisableBlendingState;
};

#endif