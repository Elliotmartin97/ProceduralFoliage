#pragma once

#include "Model.h"

class LOD
{
public:
	LOD() = default;
	~LOD();
	void Init(ID3D11Device* device, ID3D11DeviceContext* deviceContext, int textureWidth, int textureHeight);
	void SetRenderTarget(ID3D11DeviceContext* deviceContext, ID3D11DepthStencilView* depthStencilView);
	void ClearRenderTarget(ID3D11DeviceContext* deviceContext, ID3D11DepthStencilView* depthStencilView, float red, float green, float blue, float alpha);
	ID3D11ShaderResourceView* GetShaderResourceView();
	std::vector<Model*> GetModel();
	void SetQuadScale(float x, float y, float z);
	void SetQuadPosition(float x, float y, float z);
	void SetLODBounds(std::vector<Model*> models);
private:
	std::vector<Model*> quads;

	ID3D11Texture2D* renderTargetTextureMap;
	ID3D11RenderTargetView* renderTargetViewMap;
	ID3D11ShaderResourceView* shaderResourceViewMap;

	XMMATRIX quad_view;
	XMMATRIX quad_projection;
	XMVECTOR cam_position;
	XMFLOAT3 lod_bounds[2];
};