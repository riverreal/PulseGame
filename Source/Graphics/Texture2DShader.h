#pragma once

#include <d3d11.h>
#include <d3dcompiler.h>
#include <DirectXMath.h>
#include <fstream>
#include "../Helper/TypeHelper.h"
#include "../System/GameObject.h"
#include "CameraManager.h"
#include "TextureManager.h"

using namespace std;
namespace Elixir
{
	class Texture2DShader
	{
	public:
		Texture2DShader();
		~Texture2DShader();

		bool Initialize(ID3D11Device* device, HWND hWnd);
		void Shutdown();
		bool Render(ID3D11DeviceContext* deviceContext, Elixir::GameObject* obj, Camera* camera, TextureManager* texManger);
	private:
		bool InitializeShader(ID3D11Device* device, HWND hWnd);
		void ShutdownShader();

		bool SetShaderParameters(ID3D11DeviceContext* deviceContext, Elixir::GameObject* obj, ID3D11ShaderResourceView* tex, Camera* camera);
		void RenderShader(ID3D11DeviceContext* deviceContext);

		struct Texture2DBuffer
		{
			DirectX::XMFLOAT4 transform;
			DirectX::XMFLOAT4 color;
		};

	private:
		float m_screenWidth;
		float m_screenHeight;
		//HWND m_hWnd;
		ID3D11VertexShader* m_vertexShader;
		ID3D11PixelShader* m_pixelShader;
		ID3D11InputLayout* m_layout;
		ID3D11SamplerState* m_samplerState;
		ID3D11Buffer* m_tex2dBuffer;
	};
}
