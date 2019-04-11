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
	void StartScene(float, float, float, float);
	void EndScene();
	void Shutdown();

	ID3D11Device* GetDevice();
	ID3D11DeviceContext* GetDeviceContext();
	ID3D11DepthStencilView* GetDepthStencilView();

	void GetProjectionMatrix(XMMATRIX&);
	void GetWorldMatrix(XMMATRIX&);
	void GetOrthoMatrix(XMMATRIX&);
	void GetVideoCardInfo(char*, int&);
	void TurnOnAlphaBlending();
	void TurnOffAlphaBlending();
	void SetBackBufferTarget();

private:

	bool vsync_mode;
	int video_card_memory;
	char video_card_desc[128];
	IDXGISwapChain* swap_chain;
	ID3D11Device* device;
	ID3D11DeviceContext* device_context;
	ID3D11RenderTargetView* render_target_view;
	ID3D11Texture2D* depth_stencil_buffer;
	ID3D11DepthStencilState* depth_stencil_state;
	ID3D11DepthStencilView* depth_stencil_view;
	ID3D11RasterizerState* raster_state;
	XMMATRIX projection_matrix;
	XMMATRIX world_matrix;
	XMMATRIX ortho_matrix;
	ID3D11BlendState* enable_alpha_blend_state;
	ID3D11BlendState* disable_alpha_blend_state;
};

#endif