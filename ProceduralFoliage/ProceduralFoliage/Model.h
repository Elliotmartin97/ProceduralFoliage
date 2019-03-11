#pragma once

struct ModelType
{
	float x, y, z;
	float tu, tv;
	float nx, ny, nz;
};

enum CubePoint
{
	TOPRIGHTFRONT,
	TOPRIGHTBACK,
	TOPLEFTFRONT,
	TOPLEFTBACK,
	BOTRIGHTFRONT,
	BOTRIGHTBACK,
	BOTLEFTFRONT,
	BOTLEFTBACK
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
	std::vector<XMFLOAT3> GetTransformedVertexData();
	void TransformVertexData();
	void SetTransformedVertexData(std::vector<XMFLOAT3> new_data);
	std::vector<int> GetBotIndexes() { return bot_indexes; };
	std::vector<int> GetTopIndexes() { return top_indexes; };
	XMFLOAT3 GetPointPosition(CubePoint point);
	void LinkBotPosition(Model* model, int index);
	int GetBotCount() { return bot_indexes.size(); };

	void SetBufferToTransformedVertices(ID3D11Device* device);

private:
	bool InitializeBuffers(ID3D11Device*);
	void ShutdownBuffers();
	void RenderBuffers(ID3D11DeviceContext*);

	bool LoadTextures(ID3D11Device*, ID3D11DeviceContext*, char*, char*, char*);
	void ReleaseTexture();

	bool LoadModel(char*);
	void ReleaseModel();

private:
	VertexType* vertices;
	ID3D11Device* m_device;
	ModelType* m_model;
	std::vector<XMFLOAT3> transformed_vertex_data;
	ID3D11Buffer *m_vertexBuffer, *m_indexBuffer;
	int m_vertexCount, m_indexCount;
	Texture* m_Texture;
	char* texture_file_name;
	std::string texture_name;
	Texture* m_metallic_texture;
	Texture* m_roughness_texture;
	float blend_amount;
	std::vector<int> top_indexes;
	std::vector<int> bot_indexes;
};

#endif