#include "LOD.h"


LOD::~LOD()
{
	for (int i = 0; i < quads.size(); i++)
	{
		if (quads[i])
		{
			quads[i]->Shutdown();
			delete quads[i];
			quads[i] = nullptr;
		}
	}
}

void LOD::Init(ID3D11Device * device, ID3D11DeviceContext* device_context, int textureWidth, int textureHeight)
{
	quads.resize(2);
	for (int i = 0; i < quads.size(); i++)
	{
		quads[i] = new Model;
		quads[i]->Init(device, device_context, (char*)"../Engine/Models/Quad.txt", (char*)"../Engine/Textures/Alpha.tga", (char*)"../Engine/Textures/Alpha.tga", (char*)"../Engine/Textures/Alpha.tga");
		quads[i]->SetPosition(0.0f, 0.0f, 0.0f);
		quads[i]->SetScale(1.0f, 1.0f, 1.0f);
	}
	D3D11_TEXTURE2D_DESC textureDesc;
	D3D11_RENDER_TARGET_VIEW_DESC renderTargetViewDesc;
	D3D11_SHADER_RESOURCE_VIEW_DESC shaderResourceViewDesc;

	ZeroMemory(&textureDesc, sizeof(textureDesc));

	textureDesc.Width = textureWidth;
	textureDesc.Height = textureHeight;
	textureDesc.MipLevels = 1;
	textureDesc.ArraySize = 1;
	textureDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	textureDesc.SampleDesc.Count = 1;
	textureDesc.Usage = D3D11_USAGE_DEFAULT;
	textureDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
	textureDesc.CPUAccessFlags = 0;
	textureDesc.MiscFlags = 0;

	device->CreateTexture2D(&textureDesc, NULL, &renderTargetTextureMap);

	renderTargetViewDesc.Format = textureDesc.Format;
	renderTargetViewDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
	renderTargetViewDesc.Texture2D.MipSlice = 0;

	device->CreateRenderTargetView(renderTargetTextureMap, &renderTargetViewDesc, &renderTargetViewMap);

	shaderResourceViewDesc.Format = textureDesc.Format;
	shaderResourceViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	shaderResourceViewDesc.Texture2D.MostDetailedMip = 0;
	shaderResourceViewDesc.Texture2D.MipLevels = 1;

	device->CreateShaderResourceView(renderTargetTextureMap, &shaderResourceViewDesc, &shaderResourceViewMap);


}

void LOD::ClearRenderTarget(ID3D11DeviceContext* deviceContext, ID3D11DepthStencilView* depthStencilView, float red, float green, float blue, float alpha)
{
	float color[4];

	color[0] = red;
	color[1] = green;
	color[2] = blue;
	color[3] = alpha;

	deviceContext->ClearRenderTargetView(renderTargetViewMap, color);

	deviceContext->ClearDepthStencilView(depthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);
}

ID3D11ShaderResourceView * LOD::GetShaderResourceView()
{
	return shaderResourceViewMap;
}

void LOD::SetRenderTarget(ID3D11DeviceContext* deviceContext, ID3D11DepthStencilView* depthStencilView)
{
	deviceContext->OMSetRenderTargets(1, &renderTargetViewMap, depthStencilView);
}

void LOD::SetQuadScale(float x, float y, float z)
{
	for (int i = 0; i < quads.size(); i++)
	{
		quads[i]->SetScale(x, y, z);
	}
}

void LOD::SetQuadPosition(float x, float y, float z)
{
	for (int i = 0; i < quads.size(); i++)
	{
		quads[i]->SetPosition(x, y, z);
	}
}

void LOD::SetLODBounds(std::vector<Model*> models)
{
	for (int i = 0; i < models.size(); i++)
	{
		XMFLOAT3 min = models[i]->GetMinBounds();
		XMFLOAT3 max = models[i]->GetMaxBounds();

		if (min.x < lod_bounds[0].x) lod_bounds[0].x = min.x;
		if (min.y < lod_bounds[0].y) lod_bounds[0].y = min.y;
		if (min.z < lod_bounds[0].z) lod_bounds[0].z = min.z;

		if (max.x > lod_bounds[0].x) lod_bounds[0].x = max.x;
		if (max.y > lod_bounds[0].y) lod_bounds[0].y = max.y;
		if (max.z > lod_bounds[0].z) lod_bounds[0].z = max.z;
		
	}
}

std::vector<Model*> LOD::GetModel()
{
	return quads;
}
