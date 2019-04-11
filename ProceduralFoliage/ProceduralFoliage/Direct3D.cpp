#include "Direct3D.h"

bool Direct3D::Init(int screenWidth, int screenHeight, bool vsync, HWND hwnd, bool fullscreen,
	float screenDepth, float screenNear)
{
	HRESULT result;
	IDXGIFactory* factory;
	IDXGIAdapter* adapter;
	IDXGIOutput* adapterOutput;
	unsigned int numModes, i, numerator, denominator;
	unsigned long long string_length;
	DXGI_MODE_DESC* display_mode_list;
	DXGI_ADAPTER_DESC adapter_desc;
	DXGI_SWAP_CHAIN_DESC swap_chain_desc;
	D3D_FEATURE_LEVEL feature_level;
	ID3D11Texture2D* back_buffer_ptr;
	D3D11_TEXTURE2D_DESC depth_buffer_desc;
	D3D11_DEPTH_STENCIL_DESC depth_stencil_desc;
	D3D11_DEPTH_STENCIL_VIEW_DESC depth_stencil_view_desc;
	D3D11_RASTERIZER_DESC raster_desc;
	D3D11_VIEWPORT viewport;
	D3D11_BLEND_DESC blend_state_desc;
	float fov, screen_aspect_ratio;


	//setup the direct3D object and display for the video card
	//vsync in enabled as movement is not done via a deltatime
	vsync_mode = vsync;
	CreateDXGIFactory(__uuidof(IDXGIFactory), (void**)&factory);
	factory->EnumAdapters(0, &adapter);
	adapter->EnumOutputs(0, &adapterOutput);
	adapterOutput->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_ENUM_MODES_INTERLACED, &numModes, NULL);
	display_mode_list = new DXGI_MODE_DESC[numModes];
	adapterOutput->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_ENUM_MODES_INTERLACED, &numModes, display_mode_list);

	for (i = 0; i < numModes; i++)
	{
		if (display_mode_list[i].Width == (unsigned int)screenWidth)
		{
			if (display_mode_list[i].Height == (unsigned int)screenHeight)
			{
				numerator = display_mode_list[i].RefreshRate.Numerator;
				denominator = display_mode_list[i].RefreshRate.Denominator;
			}
		}
	}
	adapter->GetDesc(&adapter_desc);
	video_card_memory = (int)(adapter_desc.DedicatedVideoMemory / 1024 / 1024);
	wcstombs_s(&string_length, video_card_desc, 128, adapter_desc.Description, 128);

	delete[] display_mode_list;
	display_mode_list = 0;
	adapterOutput->Release();
	adapterOutput = 0;
	adapter->Release();
	adapter = 0;
	factory->Release();
	factory = 0;
	ZeroMemory(&swap_chain_desc, sizeof(swap_chain_desc));
	swap_chain_desc.BufferCount = 1;
	swap_chain_desc.BufferDesc.Width = screenWidth;
	swap_chain_desc.BufferDesc.Height = screenHeight;
	swap_chain_desc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;

	if (vsync_mode)
	{
		swap_chain_desc.BufferDesc.RefreshRate.Numerator = numerator;
		swap_chain_desc.BufferDesc.RefreshRate.Denominator = denominator;
	}
	else
	{
		swap_chain_desc.BufferDesc.RefreshRate.Numerator = 0;
		swap_chain_desc.BufferDesc.RefreshRate.Denominator = 1;
	}

	swap_chain_desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swap_chain_desc.OutputWindow = hwnd;
	swap_chain_desc.SampleDesc.Count = 1;
	swap_chain_desc.SampleDesc.Quality = 0;
	swap_chain_desc.Windowed = true;
	swap_chain_desc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	swap_chain_desc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	swap_chain_desc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	swap_chain_desc.Flags = 0;
	feature_level = D3D_FEATURE_LEVEL_11_0;

	D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, 0, &feature_level, 1,
		D3D11_SDK_VERSION, &swap_chain_desc, &swap_chain, &device, NULL, &device_context);
	 
	swap_chain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&back_buffer_ptr);
	device->CreateRenderTargetView(back_buffer_ptr, NULL, &render_target_view);

	back_buffer_ptr->Release();
	back_buffer_ptr = 0;
	ZeroMemory(&depth_buffer_desc, sizeof(depth_buffer_desc));

	depth_buffer_desc.Width = screenWidth;
	depth_buffer_desc.Height = screenHeight;
	depth_buffer_desc.MipLevels = 1;
	depth_buffer_desc.ArraySize = 1;
	depth_buffer_desc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depth_buffer_desc.SampleDesc.Count = 1;
	depth_buffer_desc.SampleDesc.Quality = 0;
	depth_buffer_desc.Usage = D3D11_USAGE_DEFAULT;
	depth_buffer_desc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	depth_buffer_desc.CPUAccessFlags = 0;
	depth_buffer_desc.MiscFlags = 0;
	
	device->CreateTexture2D(&depth_buffer_desc, NULL, &depth_stencil_buffer);
	ZeroMemory(&depth_stencil_desc, sizeof(depth_stencil_desc));

	depth_stencil_desc.DepthEnable = true;
	depth_stencil_desc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	depth_stencil_desc.DepthFunc = D3D11_COMPARISON_LESS;
	depth_stencil_desc.StencilEnable = true;
	depth_stencil_desc.StencilReadMask = 0xFF;
	depth_stencil_desc.StencilWriteMask = 0xFF;

	// if pixel is front-facing.
	depth_stencil_desc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depth_stencil_desc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
	depth_stencil_desc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depth_stencil_desc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	// if pixel is back-facing.
	depth_stencil_desc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depth_stencil_desc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
	depth_stencil_desc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depth_stencil_desc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
	
	device->CreateDepthStencilState(&depth_stencil_desc, &depth_stencil_state);
	device_context->OMSetDepthStencilState(depth_stencil_state, 1);
	ZeroMemory(&depth_stencil_view_desc, sizeof(depth_stencil_view_desc));
	depth_stencil_view_desc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depth_stencil_view_desc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	depth_stencil_view_desc.Texture2D.MipSlice = 0;

	device->CreateDepthStencilView(depth_stencil_buffer, &depth_stencil_view_desc, &depth_stencil_view);
	device_context->OMSetRenderTargets(1, &render_target_view, depth_stencil_view);

	raster_desc.AntialiasedLineEnable = false;
	raster_desc.CullMode = D3D11_CULL_BACK;
	raster_desc.DepthBias = 0;
	raster_desc.DepthBiasClamp = 0.0f;
	raster_desc.DepthClipEnable = true;
	raster_desc.FillMode = D3D11_FILL_SOLID;
	raster_desc.FrontCounterClockwise = false;
	raster_desc.MultisampleEnable = false;
	raster_desc.ScissorEnable = false;
	raster_desc.SlopeScaledDepthBias = 0.0f;

	ZeroMemory(&blend_state_desc, sizeof(D3D11_BLEND_DESC));
	// blend state for alpha blending of models
	blend_state_desc.RenderTarget[0].BlendEnable = TRUE;
	blend_state_desc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
	blend_state_desc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
	blend_state_desc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	blend_state_desc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
	blend_state_desc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
	blend_state_desc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	blend_state_desc.RenderTarget[0].RenderTargetWriteMask = 0x0f;

	device->CreateBlendState(&blend_state_desc, &enable_alpha_blend_state);
	blend_state_desc.RenderTarget[0].BlendEnable = FALSE;
	device->CreateBlendState(&blend_state_desc, &disable_alpha_blend_state);
	device->CreateRasterizerState(&raster_desc, &raster_state);
	device_context->RSSetState(raster_state);
	
	viewport.Width = (float)screenWidth;
	viewport.Height = (float)screenHeight;
	viewport.MinDepth = 0.0f;
	viewport.MaxDepth = 1.0f;
	viewport.TopLeftX = 0.0f;
	viewport.TopLeftY = 0.0f;

	device_context->RSSetViewports(1, &viewport);
	fov = 3.141592654f / 4.0f;
	screen_aspect_ratio = (float)screenWidth / (float)screenHeight;

	projection_matrix = XMMatrixPerspectiveFovLH(fov, screen_aspect_ratio, screenNear, screenDepth);
	world_matrix = XMMatrixIdentity();
	ortho_matrix = XMMatrixOrthographicLH((float)screenWidth, (float)screenHeight, screenNear, screenDepth);

	return true;
}

void Direct3D::Shutdown()
{
	if (enable_alpha_blend_state)
	{
		enable_alpha_blend_state->Release();
		enable_alpha_blend_state = 0;
	}

	if (disable_alpha_blend_state)
	{
		disable_alpha_blend_state->Release();
		disable_alpha_blend_state = 0;
	}

	if (swap_chain)
	{
		swap_chain->SetFullscreenState(false, NULL);
	}

	if (raster_state)
	{
		raster_state->Release();
		raster_state = 0;
	}

	if (depth_stencil_view)
	{
		depth_stencil_view->Release();
		depth_stencil_view = 0;
	}

	if (depth_stencil_state)
	{
		depth_stencil_state->Release();
		depth_stencil_state = 0;
	}

	if (depth_stencil_buffer)
	{
		depth_stencil_buffer->Release();
		depth_stencil_buffer = 0;
	}

	if (render_target_view)
	{
		render_target_view->Release();
		render_target_view = 0;
	}

	if (device_context)
	{
		device_context->Release();
		device_context = 0;
	}

	if (device)
	{
		device->Release();
		device = 0;
	}

	if (swap_chain)
	{
		swap_chain->Release();
		swap_chain = 0;
	}
}

void Direct3D::StartScene(float red, float green, float blue, float alpha)
{
	float color[4];
	color[0] = red;
	color[1] = green;
	color[2] = blue;
	color[3] = alpha;

	// Clear the back buffer, assigning the color provided
	device_context->ClearRenderTargetView(render_target_view, color);
	device_context->ClearDepthStencilView(depth_stencil_view, D3D11_CLEAR_DEPTH, 1.0f, 0);
}

void Direct3D::EndScene()
{
	if (vsync_mode)
	{
		swap_chain->Present(1, 0);
	}
	else
	{
		swap_chain->Present(0, 0);
	}
}

ID3D11Device* Direct3D::GetDevice()
{
	return device;
}

ID3D11DeviceContext* Direct3D::GetDeviceContext()
{
	return device_context;
}

ID3D11DepthStencilView * Direct3D::GetDepthStencilView()
{
	return depth_stencil_view;
}

void Direct3D::GetProjectionMatrix(XMMATRIX& projectionMatrix)
{
	projectionMatrix = projection_matrix;
}


void Direct3D::GetWorldMatrix(XMMATRIX& worldMatrix)
{
	worldMatrix = world_matrix;
}


void Direct3D::GetOrthoMatrix(XMMATRIX& orthoMatrix)
{
	orthoMatrix = ortho_matrix;
}

void Direct3D::GetVideoCardInfo(char* cardName, int& memory)
{
	strcpy_s(cardName, 128, video_card_desc);
	memory = video_card_memory;
}

void Direct3D::TurnOnAlphaBlending()
{
	float blendFactor[4];
	blendFactor[0] = 0.0f;
	blendFactor[1] = 0.0f;
	blendFactor[2] = 0.0f;
	blendFactor[3] = 0.0f;

	// Turn on alpha blending mode
	device_context->OMSetBlendState(enable_alpha_blend_state, blendFactor, 0xffffffff);
}

void Direct3D::TurnOffAlphaBlending()
{
	float blendFactor[4];
	blendFactor[0] = 0.0f;
	blendFactor[1] = 0.0f;
	blendFactor[2] = 0.0f;
	blendFactor[3] = 0.0f;

	// Turn off alpha blending mode
	device_context->OMSetBlendState(disable_alpha_blend_state, blendFactor, 0xffffffff);
}

void Direct3D::SetBackBufferTarget()
{
	device_context->OMSetRenderTargets(1, &render_target_view, depth_stencil_view);
}
