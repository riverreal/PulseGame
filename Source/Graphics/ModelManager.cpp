#include "ModelManager.h"
#include "../Helper/GeneralHelper.h"
#include "../Helper/MathHelper.h"
#include <fstream>
#include <string>
#include <iostream>
#include "../System/GameManager.h"
#include "../../jsoncpp/json/json.h"

using namespace DirectX;

using namespace Elixir;

Model::Model()
	:m_vertexBuffer(0),
	m_indexBuffer(0),
	m_totalVertexCount(0),
	m_totalIndexCount(0),
	m_lastVertexOffset(0),
	m_lastVertexSize(0),
	m_lastIndexOffset(0),
	m_lastIndexSize(0)
{
}

Model::Model(const Model& other)
{
}

Model::~Model()
{
}

offsetData Model::AddGeometry(int modelType)
{
	offsetData offset;
	Terrain plainTerrain;
	BasicShapes shapes;
	MeshData meshData;
	UINT currentVertexCount;
	UINT currentIndexCount;

	switch (modelType)
	{
	case MODEL_TYPE_CUBE:
		shapes.CreateCube(1.0f, 1.0f, 1.0f, meshData);
		break;
	case MODEL_TYPE_CYLINDER:
		shapes.CreateCylinder(0.5f, 0.5f, 1.0f, 10, 10, meshData);
		break;
	case MODEL_TYPE_GEOSPHERE:
		shapes.CreateGeosphere(0.5f, 2, meshData);
		break;
	case MODEL_TYPE_PLAIN:
		plainTerrain.CreatePlane(1.0f, 1.0f, 20, 20, meshData);
		break;
	case MODEL_TYPE_SCREEN_LAYER:
		shapes.CreateScreenLayer(meshData);
		break;
	case MODEL_TYPE_SPHERE:
		shapes.CreateSphere(0.5f, 20, 20, meshData);
		break;
	default:
		shapes.CreateCube(1.0f, 1.0f, 1.0f, meshData);
		break;
	}

	currentVertexCount = meshData.Vertices.size();
	currentIndexCount = meshData.Indices.size();
	m_totalVertexCount += meshData.Vertices.size();
	m_totalIndexCount += meshData.Indices.size();

	std::vector<Vertex> vertices(currentVertexCount);
	for (size_t i = 0; i < currentVertexCount; ++i)
	{
		XMFLOAT3 position = meshData.Vertices[i].Position;
		vertices[i].Position = position;
		vertices[i].Normal = meshData.Vertices[i].Normal;
		vertices[i].Tex = meshData.Vertices[i].Tex;
		vertices[i].TangentU = meshData.Vertices[i].TangentU;
		m_totalVertex.push_back(vertices[i]);
	}

	std::vector<UINT> indices(currentIndexCount);
	for (size_t i = 0; i < currentIndexCount; ++i)
	{
		indices[i] = meshData.Indices[i];
		m_totalIndex.push_back(indices[i]);
	}

	//get the index offset
	m_lastIndexOffset += m_lastIndexSize;
	m_lastIndexSize = indices.size();

	//get the vertex offset
	m_lastVertexOffset += m_lastVertexSize;
	m_lastVertexSize = vertices.size();

	offset.indexCount = currentIndexCount;
	offset.indexOffset = m_lastIndexOffset;
	offset.vertexOffset = m_lastVertexOffset;

	return offset;
}

offsetData Model::AddCustomGeometry(std::wstring fileName)
{
	offsetData offset;
	UINT vertexCount = 0;
	UINT trianglesCount = 0;
	std::string ignore;
	std::ifstream fin;
	UINT indexCount;

	std::string str_fileName(fileName.begin(), fileName.end());
	//Get Data from File
	fin.open(str_fileName);
	LPCWSTR errorMsg = L"Not Found Custom Geometry";

	if (!fin)
	{
		MessageBox(0, fileName.c_str(), errorMsg, 0);
		return offset;
	}

	fin >> ignore >> vertexCount;
	fin >> ignore >> trianglesCount;
	fin >> ignore >> ignore >> ignore >> ignore;

	std::cout << vertexCount;

	float nx, ny, nz;
	XMFLOAT4 black(0.0f, 0.0f, 0.0f, 1.0f);

	std::vector<Vertex> vertices(vertexCount);
	for (UINT i = 0; i < vertexCount; ++i)
	{
		fin >> vertices[i].Position.x >> vertices[i].Position.y >> vertices[i].Position.z;

		fin >> nx >> ny >> nz; //normals

		vertices[i].Normal = XMFLOAT3(nx,ny,nz);

		m_totalVertex.push_back(vertices[i]);
	}

	fin >> ignore;
	fin >> ignore;
	fin >> ignore;

	indexCount = 3 * trianglesCount;
	std::vector<UINT> indices(indexCount);
	for (UINT i = 0; i < trianglesCount; ++i)
	{
		fin >> indices[i * 3 + 0] >> indices[i * 3 + 1] >> indices[i * 3 + 2];
	}

	for (UINT i = 0; i < indexCount; ++i)
	{
		m_totalIndex.push_back(indices[i]);
	}

	fin.close();

	//get the vertex and index count
	m_totalVertexCount += vertexCount;
	m_totalIndexCount += indexCount;

	//get the index offset
	m_lastIndexOffset += m_lastIndexSize;
	m_lastIndexSize = indices.size();

	//get the vertex offset
	m_lastVertexOffset += m_lastVertexSize;
	m_lastVertexSize = vertices.size();

	//Calculate Offset
	offset.indexCount = indexCount;
	offset.indexOffset = m_lastIndexOffset;
	offset.vertexOffset = m_lastVertexOffset;

	return offset;
}

offsetData Model::AddModelFromFile(std::string fileName)
{
	offsetData offset;
	unsigned int numberOfMeshes;
	Assimp::Importer imp;

	auto scene = imp.ReadFile(fileName, aiProcess_CalcTangentSpace |
		aiProcess_Triangulate |
		aiProcess_GenSmoothNormals |
		aiProcess_SplitLargeMeshes |
		aiProcess_ConvertToLeftHanded |
		aiProcess_SortByPType |
		aiProcess_PreTransformVertices);

	if (scene == NULL)
	{
		MessageBox(0, L"Failed to Import Scene with Assimp", 0, MB_OK);
		return offset;
	}

	numberOfMeshes = scene->mNumMeshes;

	std::vector<Vertex> vertices;
	std::vector<UINT> indices;

	UINT vertexCount = 0;
	UINT indexCount = 0;

	for (UINT i = 0; i < numberOfMeshes; ++i)
	{
		auto mesh = scene->mMeshes[i];
		if (mesh)
		{
			for (UINT j = 0; j < mesh->mNumVertices; ++j)
			{
				Vertex v;
				vertexCount++;
				v.Position.x = mesh->mVertices[j].x;
				v.Position.y = mesh->mVertices[j].y;
				v.Position.z = mesh->mVertices[j].z;

				v.Normal.x = mesh->mNormals[j].x;
				v.Normal.y = mesh->mNormals[j].y;
				v.Normal.z = mesh->mNormals[j].z;

				v.TangentU.x = mesh->mTangents[j].x;
				v.TangentU.y = mesh->mTangents[j].y;
				v.TangentU.z = mesh->mTangents[j].z;

				if (mesh->HasTextureCoords(0))
				{
					v.Tex.x = mesh->mTextureCoords[0][j].x;
					v.Tex.y = mesh->mTextureCoords[0][j].y;
				}

				vertices.push_back(v);
				m_totalVertex.push_back(v);
			}

			for (UINT c = 0; c < mesh->mNumFaces; ++c)
			{
				for (UINT e = 0; e < mesh->mFaces[c].mNumIndices; ++e)
				{
					indexCount++;
					indices.push_back(mesh->mFaces[c].mIndices[e]);
					m_totalIndex.push_back(mesh->mFaces[c].mIndices[e]);
				}
			}
			std::vector<XMFLOAT3> tangents;
			for (UINT k = 0; k < mesh->mNumVertices; k += 3)
			{
				XMFLOAT3 v0;
				v0.x = mesh->mVertices[k + 0].x;
				v0.y = mesh->mVertices[k + 0].y;
				v0.z = mesh->mVertices[k + 0].z;
				
				XMFLOAT3 v1;
				v1.x = mesh->mVertices[k + 1].x;
				v1.y = mesh->mVertices[k + 1].y;
				v1.z = mesh->mVertices[k + 1].z;

				XMFLOAT3 v2;
				v2.x = mesh->mVertices[k + 2].x;
				v2.y = mesh->mVertices[k + 2].y;
				v2.z = mesh->mVertices[k + 2].z;

				XMFLOAT2 uv0;
				uv0.x = mesh->mTextureCoords[0][k + 0].x;
				uv0.y = mesh->mTextureCoords[0][k + 0].y;

				XMFLOAT2 uv1;
				uv1.x = mesh->mTextureCoords[0][k + 1].x;
				uv1.y = mesh->mTextureCoords[0][k + 1].y;

				XMFLOAT2 uv2;
				uv2.x = mesh->mTextureCoords[0][k + 2].x;
				uv2.y = mesh->mTextureCoords[0][k + 2].y;

				XMFLOAT3 deltaPos1;
				deltaPos1.x = v1.x - v0.x;
				deltaPos1.y = v1.y - v0.y;
				deltaPos1.z = v1.z- v0.z;

				XMFLOAT3 deltaPos2;
				deltaPos2.x = v2.x - v0.x;
				deltaPos2.y = v2.y - v0.y;
				deltaPos2.z = v2.z - v0.z;

				XMFLOAT2 deltaUV1;
				deltaUV1.x = uv1.x - uv0.x;
				deltaUV1.y = uv1.y - uv0.y;

				XMFLOAT2 deltaUV2;
				deltaUV2.x = uv2.x - uv0.x;
				deltaUV2.y = uv2.y - uv0.y;

				float r = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV1.y * deltaUV2.x);
				XMFLOAT3 a = MathHelper::F3DotScalar(deltaPos1, deltaUV2.y);
				XMFLOAT3 b = MathHelper::F3DotScalar(deltaPos2, deltaUV1.y);
				XMFLOAT3 tangent = MathHelper::F3DotScalar(MathHelper::F3MinusF3(a, b),r);

				tangents.push_back(tangent);
				tangents.push_back(tangent);
				tangents.push_back(tangent);
			}

			int index = 0;
			for (auto &v: vertices)
			{	
				v.TangentU = tangents[index];
				index++;
			}
		}

		
	}

	if (vertexCount == 0)
	{
		return offset;
	}

	//get the vertex and index count
	m_totalVertexCount += vertexCount;
	m_totalIndexCount += indexCount;

	//get the index offset
	m_lastIndexOffset += m_lastIndexSize;
	m_lastIndexSize = indices.size();

	//get the vertex offset
	m_lastVertexOffset += m_lastVertexSize;
	m_lastVertexSize = vertices.size();

	//Calculate Offset
	offset.indexCount = indexCount;
	offset.indexOffset = m_lastIndexOffset;
	offset.vertexOffset = m_lastVertexOffset;

	return offset;
}

offsetData Model::AddTubeFromLineData(std::vector<Elixir::Vec3f> lines, std::vector<Elixir::Vec3f> tangents, float radius, std::vector<Elixir::Vec3f> outputLine)
{
	MeshData meshData;
	offsetData offset;

	UINT currentVertexCount;
	UINT currentIndexCount;

	int subdivision = 10;

	//loop through the lines
	for (int i = 0; i < lines.size() - 1; ++i)
	{
		//skip the first and last point (cannot get curve for those)
		if (i == 0 || i + 2 >= lines.size())
			continue;

		Vec3f tangent = tangents[i].FastNormalize();

		int maxAxis;

		if (std::abs(tangent.x) > std::abs(tangent.y))
		{
			if (std::abs(tangent.x) > std::abs(tangent.z))
			{
				maxAxis = 0;
			}
			else
			{
				maxAxis = 2;
			}
		}
		else if (std::abs(tangent.y) > std::abs(tangent.z))
		{
			maxAxis = 1;
		}
		else
		{
			maxAxis = 2;
		}

		Vec3f up, forward, right;

		switch (maxAxis)
		{
		case 0:
		case 1:
			up = tangent;
			forward = Vec3f(0, 0, 1);
			right = up.Cross(forward);
			forward = right.Cross(up);
			break;

		case 2:
			up = tangent;
			right = Vec3f(0, 0, 1);
			forward = right.Cross(up);
			right = up.Cross(forward);
			break;

		default:
			break;
		}

		for (int j = 0; j < subdivision; ++j)
		{
			float sNorm = (i * subdivision + j) / float(subdivision * lines.size());
			//float rad = 0.1f * (1 - sNorm) * radius;

			for (int k = 0; k <= subdivision; ++k)
			{
				Vertex vertex;

				//[0 , 1]
				float t = k / (float)subdivision;

				float theta = t * 2 * XM_PI;

				Vec3f pc(cos(theta) * radius, 0, sin(theta) * radius);
				float x = pc.x * right.x + pc.y * up.x + pc.z * forward.x;
				float y = pc.x * right.y + pc.y * up.y + pc.z * forward.y;
				float z = pc.x * right.z + pc.y * up.z + pc.z * forward.z;

				vertex.Position = XMFLOAT3(lines[i].x + x, lines[i].y + y, lines[i].z + z);
				auto normal = Vec3f(x, y, z).FastNormalize();
				vertex.Normal = XMFLOAT3(normal.x, normal.y, normal.z);
				vertex.TangentU = XMFLOAT3(tangent.x, tangent.y, tangent.z);
				vertex.Tex = XMFLOAT2(sNorm, t);

				meshData.Vertices.push_back(vertex);
			}
		}
	}

	int nf = 0;

	for (int k = 0; k < lines.size() - 1; ++k)
	{
		if (k == 0 || k + 2 >= lines.size())
			continue;

		for (int j = 0; j < subdivision; ++j)
		{
			for (int i = 0; i < subdivision; ++i)
			{
				meshData.Indices.push_back(nf);
				meshData.Indices.push_back(nf + (subdivision + 1));
				meshData.Indices.push_back(nf + (subdivision + 1) + 1);
				meshData.Indices.push_back(nf + 1);

				++nf;
			}
			nf++;
		}
	}

	for (int i = 0; i < subdivision; ++i)
	{
		meshData.Indices.push_back(nf);
		meshData.Indices.push_back((nf + 1) * subdivision * lines.size());
		meshData.Indices.push_back(nf + 1);

		nf++;
	}

	//Apply to buffers count

	currentVertexCount = meshData.Vertices.size();
	currentIndexCount = meshData.Indices.size();
	m_totalVertexCount += meshData.Vertices.size();
	m_totalIndexCount += meshData.Indices.size();

	std::vector<Vertex> vertices(currentVertexCount);
	for (size_t i = 0; i < currentVertexCount; ++i)
	{
		XMFLOAT3 position = meshData.Vertices[i].Position;
		vertices[i].Position = position;
		vertices[i].Normal = meshData.Vertices[i].Normal;
		vertices[i].Tex = meshData.Vertices[i].Tex;
		vertices[i].TangentU = meshData.Vertices[i].TangentU;
		m_totalVertex.push_back(vertices[i]);
	}

	std::vector<UINT> indices(currentIndexCount);
	for (size_t i = 0; i < currentIndexCount; ++i)
	{
		indices[i] = meshData.Indices[i];
		m_totalIndex.push_back(indices[i]);
	}

	//get the index offset
	m_lastIndexOffset += m_lastIndexSize;
	m_lastIndexSize = indices.size();

	//get the vertex offset
	m_lastVertexOffset += m_lastVertexSize;
	m_lastVertexSize = vertices.size();

	offset.indexCount = currentIndexCount;
	offset.indexOffset = m_lastIndexOffset;
	offset.vertexOffset = m_lastVertexOffset;

	return offset;
}

bool Model::Initialize(ID3D11Device* device)
{
	if (!InitializeBuffers(device))
	{
		return false;
	}

	return true;
}

void Model::Shutdown()
{
	ShutdownBuffers();
}

void Model::Render(ID3D11DeviceContext* deviceContext)
{
	RenderBuffers(deviceContext);
}

bool Model::InitializeBuffers(ID3D11Device* device)
{
	//-----------------------------------------------------
	//Needed Variables
	//-----------------------------------------------------
	D3D11_BUFFER_DESC vertexBufferDesc;
	D3D11_BUFFER_DESC indexBufferDesc;
	D3D11_SUBRESOURCE_DATA vertexData;
	D3D11_SUBRESOURCE_DATA indexData;

	HRESULT result;

	vertexBufferDesc.Usage = D3D11_USAGE_IMMUTABLE;
	vertexBufferDesc.ByteWidth = sizeof(Vertex)* m_totalVertexCount;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = 0;
	vertexBufferDesc.MiscFlags = 0;
	vertexBufferDesc.StructureByteStride = 0;

	vertexData.pSysMem = &m_totalVertex[0];
	vertexData.SysMemPitch = 0;
	vertexData.SysMemSlicePitch = 0;

	result = device->CreateBuffer(&vertexBufferDesc, &vertexData, &m_vertexBuffer);
	if (FAILED(result))
	{
		return false;
	}

	indexBufferDesc.Usage = D3D11_USAGE_IMMUTABLE;
	indexBufferDesc.ByteWidth = sizeof(unsigned int) * m_totalIndexCount;
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;
	indexBufferDesc.StructureByteStride = 0;

	indexData.pSysMem = &m_totalIndex[0];
	indexData.SysMemPitch = 0;
	indexData.SysMemSlicePitch = 0;

	result = device->CreateBuffer(&indexBufferDesc, &indexData, &m_indexBuffer);
	if (FAILED(result))
	{
		return false;
	}
	
	return true;
}

void Model::ShutdownBuffers()
{
	ReleaseCOM(m_vertexBuffer);
	ReleaseCOM(m_indexBuffer);
}

void Model::RenderBuffers(ID3D11DeviceContext* deviceContext)
{
	UINT stride;
	UINT offset;

	stride = sizeof(Vertex);
	offset = 0;

	deviceContext->IASetVertexBuffers(0, 1, &m_vertexBuffer, &stride, &offset);

	deviceContext->IASetIndexBuffer(m_indexBuffer, DXGI_FORMAT_R32_UINT, 0);

	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}