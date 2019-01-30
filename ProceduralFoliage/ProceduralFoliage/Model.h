#pragma once

struct ModelType
{
	float x, y, z;
	float tu, tv;
	float nx, ny, nz;
};

#ifndef _MODELCLASS_H_
#define _MODELCLASS_H_

#include "GameObject.h"
#include <d3d11.h>
#include <directxmath.h>
#include "Texture.h"
#include <fstream>
#include <vector>

using namespace std;
using namespace DirectX;

class Model : public GameObject
{
private:
	struct VertexType
	{
		XMFLOAT3 position;
		XMFLOAT2 texture;
		XMFLOAT3 normal;
	};

public:

	Model();
	~Model();

	bool Init(ID3D11Device* device, ID3D11DeviceContext* deviceContext, char* model_filename, char* texture_filename, char* mettalic_filename, char* roughness_filename);
	void Shutdown();
	void Render(ID3D11DeviceContext*);

	float GetBlendAmount();
	void SetBlendAmount(float amount);
	int GetIndexCount();
	int GetVertexCount();
	char* GetTextureFileName();
	std::string GetTextureName();
	void SetTextureName(char* file_name);
	ID3D11ShaderResourceView* GetTexture();
	ID3D11ShaderResourceView* GetMetallic();
	ID3D11ShaderResourceView* GetRoughness();
	ModelType* GetModelType();

private:
	bool InitializeBuffers(ID3D11Device*);
	void ShutdownBuffers();
	void RenderBuffers(ID3D11DeviceContext*);

	bool LoadTextures(ID3D11Device*, ID3D11DeviceContext*, char*, char*, char*);
	void ReleaseTexture();

	bool LoadModel(char*);
	void ReleaseModel();

private:
	ID3D11Device* m_device;
	ModelType* m_model;
	ID3D11Buffer *m_vertexBuffer, *m_indexBuffer;
	int m_vertexCount, m_indexCount;
	Texture* m_Texture;
	char* texture_file_name;
	std::string texture_name;
	Texture* m_metallic_texture;
	Texture* m_roughness_texture;
	float blend_amount;
};

#endif