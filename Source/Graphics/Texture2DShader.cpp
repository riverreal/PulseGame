#include "Texture2DShader.h"
#include "Shaders\Compiled\Texture2DVS.h"
#include "Shaders\Compiled\Texture2DPS.h"

using namespace Elixir;

Texture2DShader::Texture2DShader()
	:m_vertexShader(0),
	m_pixelShader(0),
	m_layout(0),
	m_samplerState(0),
	m_tex2dBuffer(0)
{
}

Texture2DShader::~Texture2DShader()
{
}

bool Texture2DShader::Initialize(ID3D11Device * device, HWND hWnd)
{
	bool result;

	result = InitializeShader(device, hWnd);
	if (!result)
	{
		MessageBox(0, L"Can't InitializeShader in Texture2DShader", L"Error", MB_OK);
		return false;
	}

	RECT rect;
	GetClientRect(hWnd, &rect);
	m_screenWidth = abs(rect.left - rect.right);
	m_screenHeight = abs(rect.top - rect.bottom);

	return true;
}

void Texture2DShader::Shutdown()
{
	ShutdownShader();
}

bool Texture2DShader::Render(ID3D11DeviceContext * deviceContext, Elixir::GameObject * obj, Camera * camera, TextureManager* texManager)
{
	bool  result;

	result = SetShaderParameters(deviceContext, obj, texManager->GetTexture(obj->Get2DRenderer()->Texture), camera);
	if (!result)
	{
		MessageBox(0, L"Can't set shader parameters for Texture2DShader", L"Error", MB_OK);
		return false;
	}

	RenderShader(deviceContext);

	return true;
}

bool Texture2DShader::InitializeShader(ID3D11Device * device, HWND hWnd)
{
	HRESULT result;
	D3D11_INPUT_ELEMENT_DESC layout[2];
	UINT numElements;
	D3D11_SAMPLER_DESC sampDesc;
	D3D11_BUFFER_DESC tex2dBufferDesc;

	result = device->CreateVertexShader(T2DVS, sizeof(T2DVS), NULL, &m_vertexShader);
	if (FAILED(result))
	{
		MessageBox(0, L"Can't create vertex shader", L"Error", MB_OK);
		return false;
	}

	result = device->CreatePixelShader(T2DPS, sizeof(T2DPS), NULL, &m_pixelShader);
	if (FAILED(result))
	{
		MessageBox(0, L"Can't create pixel shader", L"Error", MB_OK);
		return false;
	}

	layout[0].SemanticName = "POSITION";
	layout[0].SemanticIndex = 0;
	layout[0].Format = DXGI_FORMAT_R32G32B32_FLOAT;
	layout[0].InputSlot = 0;
	layout[0].AlignedByteOffset = 0;
	layout[0].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	layout[0].InstanceDataStepRate = 0;

	layout[1].SemanticName = "TEXCOORD";
	layout[1].SemanticIndex = 0;
	layout[1].Format = DXGI_FORMAT_R32G32_FLOAT;
	layout[1].InputSlot = 0;
	layout[1].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
	layout[1].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	layout[1].InstanceDataStepRate = 0;

	numElements = sizeof(layout) / sizeof(layout[0]);

	result = device->CreateInputLayout(layout, numElements, T2DVS, sizeof(T2DVS), &m_layout);
	if (FAILED(result))
	{
		MessageBox(0, L"Can't create input layout", L"Error", MB_OK);
		return false;
	}

	sampDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_BORDER;
	sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_BORDER;
	sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_BORDER;
	sampDesc.MipLODBias = 0.0f;
	sampDesc.MaxAnisotropy = 1;
	sampDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
	sampDesc.BorderColor[0] = 0;
	sampDesc.BorderColor[1] = 0;
	sampDesc.BorderColor[2] = 0;
	sampDesc.BorderColor[3] = 0;
	sampDesc.MinLOD = 0;
	sampDesc.MaxLOD = D3D11_FLOAT32_MAX;

	result = device->CreateSamplerState(&sampDesc, &m_samplerState);
	if (FAILED(result))
	{
		MessageBox(0, L"Can't create Sampler State", L"Error", MB_OK);
		return false;
	}

	tex2dBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	tex2dBufferDesc.ByteWidth = sizeof(Texture2DBuffer);
	tex2dBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	tex2dBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	tex2dBufferDesc.MiscFlags = 0;
	tex2dBufferDesc.StructureByteStride = 0;

	result = device->CreateBuffer(&tex2dBufferDesc, nullptr, &m_tex2dBuffer);
	if (FAILED(result))
	{
		MessageBox(0, L"Can't create buffer", L"Error", MB_OK);
		return false;
	}

	return true;
}

void Texture2DShader::ShutdownShader()
{
	ReleaseCOM(m_tex2dBuffer);
	ReleaseCOM(m_samplerState);
	ReleaseCOM(m_layout);
	ReleaseCOM(m_pixelShader);
	ReleaseCOM(m_vertexShader);
}

bool Texture2DShader::SetShaderParameters(ID3D11DeviceContext * deviceContext, GameObject* obj, ID3D11ShaderResourceView* tex, Camera * camera)
{
	HRESULT result;

	D3D11_MAPPED_SUBRESOURCE mappedResource;
	ZeroMemory(&mappedResource, sizeof(D3D11_MAPPED_SUBRESOURCE));
	UINT bufferNum;
	Texture2DBuffer* dataPtr;

	deviceContext->PSSetShaderResources(0, 1, &tex);

	result = deviceContext->Map(m_tex2dBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	if (FAILED(result))
	{
		ElixirLog("Failed at Tex2dBuffer update");
		return false;
	}

	dataPtr = (Texture2DBuffer*)mappedResource.pData;
	//x = position on X
	//y = position on Y
	//z = scale on X
	//w = scale on Y
	DirectX::XMFLOAT4 transf, col;

	ID3D11Resource* res;
	tex->GetResource(&res);
	ID3D11Texture2D* texture;
	res->QueryInterface(&texture);
	D3D11_TEXTURE2D_DESC desc;
	texture->GetDesc(&desc);
	auto texW = desc.Width;
	auto texH = desc.Height;
	auto baseScaleX = texW / m_screenWidth;
	auto baseScaleY = texH / m_screenHeight;

	transf.x = obj->GetTransform()->Position.x / (m_screenWidth / 2.0f);
	transf.y = obj->GetTransform()->Position.y / (m_screenHeight / 2.0f);
	transf.z = baseScaleX * obj->GetTransform()->Scale.x;
	transf.w = baseScaleY * obj->GetTransform()->Scale.y;
	auto color = obj->Get2DRenderer()->Color;
	col.x = color.r;
	col.y = color.g;
	col.z = color.b;
	col.w = color.a;

	dataPtr->transform = transf;
	dataPtr->color = col;

	deviceContext->Unmap(m_tex2dBuffer, 0);
	deviceContext->VSSetConstantBuffers(0, 1, &m_tex2dBuffer);

	return true;
}

void Texture2DShader::RenderShader(ID3D11DeviceContext * deviceContext)
{
	deviceContext->IASetInputLayout(m_layout);
	deviceContext->VSSetShader(m_vertexShader, nullptr, 0);
	deviceContext->PSSetShader(m_pixelShader, nullptr, 0);
	deviceContext->PSSetSamplers(0, 1, &m_samplerState);

	deviceContext->Draw(3, 0);
}