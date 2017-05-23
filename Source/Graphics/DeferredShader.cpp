#include "DeferredShader.h"
#include "Shaders/Compiled/DeferredVertexShader.h"
#include "Shaders/Compiled/DeferredPixelShader.h"
#include "../Helper/GeneralHelper.h"

using namespace DirectX;
using namespace Elixir;

DeferredShader::DeferredShader()
	:m_vertexShader(0),
	m_pixelShader(0),
	m_layout(0),
	m_sampleStateWrap(0),
	m_matrixBuffer(0)
{
}

DeferredShader::~DeferredShader()
{
}

bool DeferredShader::Initialize(ID3D11Device* device, HWND window)
{
	bool result;

	result = InitializeShader(device, window);
	if (!result)
	{
		MessageBox(0, L"Couldnt initialize shaders", 0, 0);
		return false;
	}

	return true;
}

void DeferredShader::Shutdown()
{
	ShutdownShader();
}

bool DeferredShader::Render(ID3D11DeviceContext * deviceContext, GameObject* object, Camera* camera, TextureManager* texManager)
{
	bool result;
	if (object->GetRenderer() != nullptr)
	{
		auto albedo = texManager->GetTexture(object->GetRenderer()->Material.albedo);
		auto normal = texManager->GetTexture(object->GetRenderer()->Material.normal);
		auto roughness = texManager->GetTexture(object->GetRenderer()->Material.roughness);
		auto metallic = texManager->GetTexture(object->GetRenderer()->Material.metallic);
		auto emissive = texManager->GetTexture(object->GetRenderer()->Material.emissive);
		auto transMatrix = XMLoadFloat4x4(&object->GetTransform()->TextureTransform4x4);
		auto worldMatrix = XMLoadFloat4x4(&object->GetTransform()->World4x4);

		result = SetShaderParameters(deviceContext, transMatrix, worldMatrix, 
			camera->GetViewMatrix(), camera->GetProjectionMatrix(), albedo, roughness, metallic, normal, emissive);

		if (!result)
		{
			return false;
		}
	
		if(object->GetRenderer()->Enabled)
			RenderShader(deviceContext, object->GetRenderer()->Model);
	}

	for(auto &child : object->GetChildren())
	{
		if (child->GetRenderer() != nullptr)
		{
			if (child->GetRenderer()->Enabled)
			{
				Render(deviceContext, child, camera, texManager);
			}
		}
	}

	return true;
}

bool DeferredShader::InitializeShader(ID3D11Device * device, HWND window)
{
	HRESULT result;
	D3D11_INPUT_ELEMENT_DESC layout[4];
	unsigned int numElements;
	D3D11_SAMPLER_DESC samplerDesc;
	D3D11_BUFFER_DESC matrixBufferDesc;

	result = device->CreateVertexShader(DVS, sizeof(DVS), nullptr, &m_vertexShader);
	if (FAILED(result))
	{
		//err msg here
		MessageBox(0, L"Could not create vertex shader.", 0, MB_OK);
		return false;
	}

	result = device->CreatePixelShader(DPS, sizeof(DPS), nullptr, &m_pixelShader);
	if (FAILED(result))
	{
		MessageBox(0, L"Could not create pixel shader.", 0, MB_OK);
		return false;
	}

	layout[0] = {"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0}; //format float3
	layout[1] = {"NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 }; //format float3
	layout[2] = {"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 }; //format float2
	layout[3] = {"TANGENT", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0}; //format float3

	numElements = sizeof(layout) / sizeof(layout[0]);

	result = device->CreateInputLayout(layout, numElements, DVS, sizeof(DVS), &m_layout);
	if (FAILED(result))
	{
		MessageBox(0, L"Could not create input layout.", 0, MB_OK);
		return false;
	}

	samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.MipLODBias = 0.0f;
	samplerDesc.MaxAnisotropy = 1;
	samplerDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
	samplerDesc.BorderColor[0] = 0;
	samplerDesc.BorderColor[1] = 0;
	samplerDesc.BorderColor[2] = 0;
	samplerDesc.BorderColor[3] = 0;
	samplerDesc.MinLOD = 0;
	samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

	result = device->CreateSamplerState(&samplerDesc, &m_sampleStateWrap);
	if (FAILED(result))
	{
		MessageBox(0, L"Can't create sampler state.", L"Error", MB_OK);
		return false;
	}

	matrixBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	matrixBufferDesc.ByteWidth = sizeof(MatrixBufferType);
	matrixBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	matrixBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	matrixBufferDesc.MiscFlags = 0;
	matrixBufferDesc.StructureByteStride = 0;

	result = device->CreateBuffer(&matrixBufferDesc, nullptr, &m_matrixBuffer);
	if (FAILED(result))
	{
		MessageBox(0, L"Could not create matrix buffer.", 0, MB_OK);
		return false;
	}

	return true;
}

void DeferredShader::ShutdownShader()
{
	ReleaseCOM(m_matrixBuffer);
	ReleaseCOM(m_sampleStateWrap);
	ReleaseCOM(m_layout);
	ReleaseCOM(m_pixelShader);
	ReleaseCOM(m_vertexShader);
}

bool DeferredShader::SetShaderParameters(ID3D11DeviceContext * deviceContext, DirectX::XMMATRIX& texTransf, XMMATRIX & world, XMMATRIX & view, XMMATRIX & projection, ID3D11ShaderResourceView * albedoTexture, ID3D11ShaderResourceView * roughnessTexture, ID3D11ShaderResourceView * metallicTexture, ID3D11ShaderResourceView* normalTexture, ID3D11ShaderResourceView* emissiveTexture)
{
	HRESULT result;
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	unsigned int bufferNumber;
	MatrixBufferType* dataPtr;

	XMMATRIX worldCpy, viewCpy, projCpy, texTransCpy;

	worldCpy = XMMatrixTranspose(world);
	viewCpy = XMMatrixTranspose(view);
	projCpy = XMMatrixTranspose(projection);
	texTransCpy = XMMatrixTranspose(texTransf);
	

	result = deviceContext->Map(m_matrixBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	if (FAILED(result))
	{
		return false;
	}

	dataPtr = (MatrixBufferType*)mappedResource.pData;
	dataPtr->world = worldCpy;
	dataPtr->view = viewCpy;
	dataPtr->projection = projCpy;
	dataPtr->texTrans = texTransCpy;

	deviceContext->Unmap(m_matrixBuffer, 0);

	bufferNumber = 0;

	deviceContext->VSSetConstantBuffers(bufferNumber, 1, &m_matrixBuffer);
	//set albedo
	deviceContext->PSSetShaderResources(0, 1, &albedoTexture);
	deviceContext->PSSetShaderResources(1, 1, &roughnessTexture);
	deviceContext->PSSetShaderResources(2, 1, &metallicTexture);
	deviceContext->PSSetShaderResources(3, 1, &normalTexture);
	deviceContext->PSSetShaderResources(4, 1, &emissiveTexture);

	return true;
}

void DeferredShader::RenderShader(ID3D11DeviceContext * deviceContext, offsetData offset)
{
	deviceContext->IASetInputLayout(m_layout);
	deviceContext->VSSetShader(m_vertexShader, nullptr, 0);
	deviceContext->PSSetShader(m_pixelShader, nullptr, 0);

	deviceContext->PSSetSamplers(0, 1, &m_sampleStateWrap);

	deviceContext->DrawIndexed(offset.indexCount, offset.indexOffset, offset.vertexOffset);
}
