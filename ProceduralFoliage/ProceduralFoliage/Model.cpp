#include "Model.h"

Model::Model()
{
	m_vertexBuffer = 0;
	m_indexBuffer = 0;
	m_Texture = 0;
	texture_file_name = 0;
	blend_amount = 0.0f;
}
Model::~Model()
{
}

bool Model::Init(ID3D11Device* device, ID3D11DeviceContext* deviceContext, char* model_filename, char* texture_filename)
{
	bool result;

	// Load in the model data,
	result = LoadModel(model_filename);
	if (!result)
	{
		return false;
	}

	// Initialize the vertex and index buffers.
	result = InitializeBuffers(device);
	if (!result)
	{
		return false;
	}

	// Load the texture for this model.
	result = LoadTexture(device, deviceContext, texture_filename);
	if (!result)
	{
		return false;
	}
	texture_file_name = texture_filename;

	return true;
}

void Model::Shutdown()
{
	ReleaseTexture();
	// Shutdown the vertex and index buffers.
	ShutdownBuffers();
	// Release the model data.
	ReleaseModel();
}

void Model::Render(ID3D11DeviceContext* deviceContext)
{
	// Put the vertex and index buffers on the graphics pipeline to prepare them for drawing.
	RenderBuffers(deviceContext);
}

ID3D11ShaderResourceView* Model::GetTexture()
{
	return m_Texture->GetTexture();
}

int Model::GetIndexCount()
{
	return m_indexCount;
}

int Model::GetVertexCount()
{
	return m_vertexCount;
}

bool Model::InitializeBuffers(ID3D11Device* device)
{
	VertexType* vertices;
	unsigned long* indices;
	D3D11_BUFFER_DESC vertexBufferDesc, indexBufferDesc;
	D3D11_SUBRESOURCE_DATA vertexData, indexData;
	HRESULT result;


	// Create the vertex array.
	vertices = new VertexType[m_vertexCount];
	if (!vertices)
	{
		return false;
	}

	// Create the index array.
	indices = new unsigned long[m_indexCount];
	if (!indices)
	{
		return false;
	}

	// Load the vertex array and index array with data.
	for (int i = 0; i < m_vertexCount; i++)
	{
		vertices[i].position = XMFLOAT3(m_model[i].x, m_model[i].y, m_model[i].z);
		vertices[i].texture = XMFLOAT2(m_model[i].tu, m_model[i].tv);
		vertices[i].normal = XMFLOAT3(m_model[i].nx, m_model[i].ny, m_model[i].nz);

		indices[i] = i;
	}

	// Set up the description of the static vertex buffer.
	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufferDesc.ByteWidth = sizeof(VertexType) * m_vertexCount;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = 0;
	vertexBufferDesc.MiscFlags = 0;
	vertexBufferDesc.StructureByteStride = 0;

	// Give the subresource structure a pointer to the vertex data.
	vertexData.pSysMem = vertices;
	vertexData.SysMemPitch = 0;
	vertexData.SysMemSlicePitch = 0;

	// Now create the vertex buffer.
	result = device->CreateBuffer(&vertexBufferDesc, &vertexData, &m_vertexBuffer);
	if (FAILED(result))
	{
		return false;
	}

	// Set up the description of the static index buffer.
	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = sizeof(unsigned long) * m_indexCount;
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;
	indexBufferDesc.StructureByteStride = 0;

	// Give the subresource structure a pointer to the index data.
	indexData.pSysMem = indices;
	indexData.SysMemPitch = 0;
	indexData.SysMemSlicePitch = 0;

	// Create the index buffer.
	result = device->CreateBuffer(&indexBufferDesc, &indexData, &m_indexBuffer);
	if (FAILED(result))
	{
		return false;
	}
	m_device = device;
	// Release the arrays now that the vertex and index buffers have been created and loaded.
	delete[] vertices;
	vertices = 0;

	delete[] indices;
	indices = 0;

	return true;
}

void Model::ShutdownBuffers()
{
	// Release the index buffer.
	if (m_indexBuffer)
	{
		m_indexBuffer->Release();
		m_indexBuffer = 0;
	}

	// Release the vertex buffer.
	if (m_vertexBuffer)
	{
		m_vertexBuffer->Release();
		m_vertexBuffer = 0;
	}
}

void Model::RenderBuffers(ID3D11DeviceContext* deviceContext)
{
	unsigned int stride;
	unsigned int offset;

	// Set vertex buffer stride and offset.
	stride = sizeof(VertexType);
	offset = 0;

	// Set the vertex buffer to active in the input assembler so it can be rendered.
	deviceContext->IASetVertexBuffers(0, 1, &m_vertexBuffer, &stride, &offset);

	// Set the index buffer to active in the input assembler so it can be rendered.
	deviceContext->IASetIndexBuffer(m_indexBuffer, DXGI_FORMAT_R32_UINT, 0);

	// Set the type of primitive that should be rendered from this vertex buffer, in this case triangles.
	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}

bool Model::LoadTexture(ID3D11Device* device, ID3D11DeviceContext* deviceContext, char* filename)
{
	bool result;


	// Create the texture object.
	m_Texture = new Texture;
	if (!m_Texture)
	{
		return false;
	}

	// Initialize the texture object.
	result = m_Texture->Initialize(device, deviceContext, filename);
	if (!result)
	{
		return false;
	}
	texture_file_name = filename;
	SetTextureName(filename);
	return true;
}

void Model::ReleaseTexture()
{
	// Release the texture object.
	if (m_Texture)
	{
		m_Texture->Shutdown();
		delete m_Texture;
		m_Texture = 0;
	}

	return;
}

bool Model::LoadModel(char* filename)
{
	ifstream file;
	char input;


	// Open the model file.
	file.open(filename);

	// If it could not open the file then exit.
	if (file.fail())
	{
		return false;
	}

	// Read up to the value of vertex count.
	file.get(input);
	while (input != ':')
	{
		file.get(input);
	}

	// Read in the vertex count.
	file >> m_vertexCount;

	// Set the number of indices to be the same as the vertex count.
	m_indexCount = m_vertexCount;

	// Create the model using the vertex count that was read in.
	m_model = new ModelType[m_vertexCount];
	if (!m_model)
	{
		return false;
	}

	// Read up to the beginning of the data.
	file.get(input);
	while (input != ':')
	{
		file.get(input);
	}

	// Read in the vertex data.
	for (int i = 0; i < m_vertexCount; i++)
	{
		file >> m_model[i].x >> m_model[i].y >> m_model[i].z;
		file >> m_model[i].tu >> m_model[i].tv;
		file >> m_model[i].nx >> m_model[i].ny >> m_model[i].nz;

		//flip texture y because it reads upside down
		m_model[i].tv = 1 - m_model[i].tv;
	}

	// Close the model file.
	file.close();

	return true;
}

void Model::ReleaseModel()
{
	if (m_model)
	{
		delete[] m_model;
		m_model = 0;
	}
}

ModelType* Model::GetModelType()
{
	return m_model;
}

char* Model::GetTextureFileName()
{
	return texture_file_name;
}

std::string Model::GetTextureName()
{
	return texture_name;
}

void Model::SetTextureName(char* file_name)
{
	std::string name_part;

	for (int i = 0; i < file_name[i] != '\0'; i++)
	{
		if (i > 18)
		{
			if (file_name[i] == '.')
			{
				break;
			}
			name_part += file_name[i];
		}
	}
	texture_name = name_part;
}

float Model::GetBlendAmount()
{
	return blend_amount;
}

void Model::SetBlendAmount(float amount)
{
	blend_amount = amount;
}