#include "Texture.h"

Texture::Texture()
{
	targa_data = 0;
	texture = 0;
	texture_view = 0;
}

Texture::~Texture()
{
}

bool Texture::Initialize(ID3D11Device* device, ID3D11DeviceContext* deviceContext, char* filename)
{
	bool result;
	int height, width;
	D3D11_TEXTURE2D_DESC textureDesc;
	HRESULT hResult;
	unsigned int rowPitch;
	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
	
	LoadTGA(filename, height, width);


	textureDesc.Height = height;
	textureDesc.Width = width;
	textureDesc.MipLevels = 1;
	textureDesc.ArraySize = 1;
	textureDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	textureDesc.SampleDesc.Count = 1;
	textureDesc.SampleDesc.Quality = 0;
	textureDesc.Usage = D3D11_USAGE_DEFAULT;
	textureDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET;
	textureDesc.CPUAccessFlags = 0;
	textureDesc.MiscFlags = D3D11_RESOURCE_MISC_GENERATE_MIPS;
	device->CreateTexture2D(&textureDesc, NULL, &texture);

	rowPitch = (width * 4) * sizeof(unsigned char);
	deviceContext->UpdateSubresource(texture, 0, NULL, targa_data, rowPitch, 0);
	srvDesc.Format = textureDesc.Format;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MostDetailedMip = 0;
	srvDesc.Texture2D.MipLevels = -1;
	device->CreateShaderResourceView(texture, &srvDesc, &texture_view);
	deviceContext->GenerateMips(texture_view);

	delete[] targa_data;
	targa_data = 0;

	return true;
}

void Texture::Shutdown()
{
	if (texture_view)
	{
		texture_view->Release();
		texture_view = 0;
	}

	if (texture)
	{
		texture->Release();
		texture = 0;
	}

	if (targa_data)
	{
		delete[] targa_data;
		targa_data = 0;
	}
}

ID3D11ShaderResourceView* Texture::GetTexture()
{
	return texture_view;
}

bool Texture::LoadTGA(char* filename, int& height, int& width)
{
	int error, bpp, imageSize, index, i, j, k;
	FILE* filePtr;
	unsigned int count;
	TargaHeader targaFileHeader;
	unsigned char* targaImage;

	error = fopen_s(&filePtr, filename, "rb");
	if (error != 0)
	{
		return false;
	}

	count = (unsigned int)fread(&targaFileHeader, sizeof(TargaHeader), 1, filePtr);
	if (count != 1)
	{
		return false;
	}

	height = (int)targaFileHeader.height;
	width = (int)targaFileHeader.width;
	bpp = (int)targaFileHeader.bpp;

	if (bpp != 32)
	{
		return false;
	}

	imageSize = width * height * 4;

	targaImage = new unsigned char[imageSize];
	if (!targaImage)
	{
		return false;
	}

	count = (unsigned int)fread(targaImage, 1, imageSize, filePtr);
	if (count != imageSize)
	{
		return false;
	}

	error = fclose(filePtr);
	if (error != 0)
	{
		return false;
	}

	targa_data = new unsigned char[imageSize];
	if (!targa_data)
	{
		return false;
	}
	index = 0;
	k = (width * height * 4) - (width * 4);

	for (j = 0; j < height; j++)
	{
		for (i = 0; i < width; i++)
		{
			targa_data[index + 0] = targaImage[k + 2];  // Red.
			targa_data[index + 1] = targaImage[k + 1];  // Green.
			targa_data[index + 2] = targaImage[k + 0];  // Blue
			targa_data[index + 3] = targaImage[k + 3];  // Alpha

			k += 4;
			index += 4;
		}
		k -= (width * 8);
	}

	delete[] targaImage;
	targaImage = 0;

	return true;
}