#pragma once

#include <DirectXMath.h>
#include <vector>

typedef unsigned int UINT;
typedef unsigned long DWORD;
typedef unsigned int U32;
typedef int I32;
//32-bit float
typedef float F32;

const F32 E_PI = 3.141592741f;

enum BasicModel
{
	MODEL_TYPE_CUBE,
	MODEL_TYPE_SPHERE,
	MODEL_TYPE_GEOSPHERE,
	MODEL_TYPE_CYLINDER,
	MODEL_TYPE_PLAIN,
	MODEL_TYPE_SCREEN_LAYER
};

namespace Elixir
{
	struct Vec3f;
	struct Vec4f;

	//A simple Color struct
	struct Color
	{
		Color()
			:r(0.0f), g(0.0f), b(0.0f), a(1.0f)
		{};

		Color(F32 r, F32 g, F32 b, F32 a)
			:r(r), g(g), b(b), a(a)
		{};

		Color(F32 rgb)
			:r(rgb), g(rgb), b(rgb), a(1.0f)
		{};

		Color(F32 rgb, F32 a)
			:r(rgb), g(rgb), b(rgb), a(a)
		{};

		static Color Black()
		{
			return Color();
		}

		static Color Green()
		{
			return Color(0.0f, 1.0f, 0.0f, 1.0f);
		}

		static Color Red()
		{
			return Color(1.0f, 0.0f, 0.0f, 1.0f);
		}

		static Color Blue()
		{
			return Color(0.0f, 0.0f, 1.0f, 1.0f);
		}

		static Color White()
		{
			return Color(1.0f);
		}

		F32 r;
		F32 g;
		F32 b;
		F32 a;
	};

	//A Vector storing 4 floats
	struct Vec4f
	{
		Vec4f()
			:x(0.0f), y(0.0f), z(0.0f), w(0.0f)
		{};

		Vec4f(F32 x, F32 y, F32 z, F32 w)
			:x(x), y(y), z(z), w(w)
		{};

		Vec4f(F32 xyzw)
			:x(xyzw), y(xyzw), z(xyzw), w(xyzw)
		{};

		Vec4f operator* (Vec4f q) const
		{
			return Vec4f(
				w * q.w - x * q.x - y * q.y - z * q.z,
				w * q.x + x * q.w + y * q.z - z * q.y,
				w * q.y + y * q.w + z * q.x - x * q.z,
				w * q.z + z * q.w + x * q.y - y * q.x
				);
		}

		Vec4f FastNormalize() const
		{
			float lengSqr = x * x + y * y + z * z + w * w;

			int leng = *(int*)&lengSqr;

			if (leng == 0)
			{
				return 0.0f;
			}
			else
			{
				float xhalf = 0.5f * lengSqr;
				leng = 0x5f375a86 - (leng >> 1);
				lengSqr = *(float*)&leng;
				lengSqr = lengSqr * (1.5f - xhalf * lengSqr * lengSqr);
			}


			Vec4f normalized(x, y, z, w);

			normalized.x *= lengSqr;
			normalized.y *= lengSqr;
			normalized.z *= lengSqr;
			normalized.w *= lengSqr;

			return normalized;
		}

		static Vec4f Identity()
		{
			return Vec4f(0, 0, 0, 1);
		}

		F32 x;
		F32 y;
		F32 z;
		F32 w;
	};

	//A Vector storing 3 floats
	struct Vec3f
	{
		Vec3f()
			:x(0.0f), y(0.0f), z(0.0f)
		{};

		Vec3f(F32 x, F32 y, F32 z)
			:x(x), y(y), z(z)
		{};

		Vec3f(F32 xyz)
			:x(xyz), y(xyz), z(xyz)
		{};

		Vec3f(DirectX::XMFLOAT3 xmf3)
			:x(xmf3.x), y(xmf3.y), z(xmf3.z)
		{};

		inline Vec3f operator* (const float scalar) const
		{
			return Vec3f(x*scalar, y*scalar, z*scalar);
		}

		inline Vec3f operator+ (const Vec3f& sum) const
		{
			return Vec3f(x + sum.x, y + sum.y, z + sum.z);
		}

		inline Vec3f operator- (const Vec3f& sum) const
		{
			return Vec3f(x - sum.x, y - sum.y, z - sum.z);
		}

		inline bool operator== (const Vec3f& comp) const
		{
			return (x == comp.x && y == comp.y && z == comp.z);
		}

		inline bool operator!= (const Vec3f& comp) const
		{
			return !(*this == comp);
		}

		inline F32 Length() const
		{
			return sqrt(x*x + y*y + z*z);
		}

		Vec3f FastNormalize() const
		{
			float lengSqr = x * x + y * y + z * z;

			int leng = *(int*)&lengSqr;

			if (leng == 0)
			{
				return 0.0f;
			}
			else
			{
				float xhalf = 0.5f * lengSqr;
				leng = 0x5f375a86 - (leng >> 1);
				lengSqr = *(float*)&leng;
				lengSqr = lengSqr * (1.5f - xhalf * lengSqr * lengSqr);
			}


			Vec3f normalized(x, y, z);

			normalized.x *= lengSqr;
			normalized.y *= lengSqr;
			normalized.z *= lengSqr;

			return normalized;
		}

		Vec3f Cross(const Vec3f& v) const
		{
			Vec3f result;
			result.x = y * v.z - v.y * z;
			result.y = v.x * z - x * v.z;
			result.z = x * v.y - v.x * y;

			return result;
		}

		F32 Dot(const Vec3f& d) const
		{
			F32 product = 0;
			product += x*d.x;
			product += y*d.y;
			product += z*d.z;

			return product;
		}

		static Vec3f Zero()
		{
			return Vec3f(0, 0, 0);
		}

		inline bool isZeroLength() const
		{
			F32 sqlen = (x * x) + (y * y) + (z * z);
			return (sqlen < (1e-06 * 1e-06));
		}

		static Vec4f QuaternionFromToRotation(const Vec3f& v1, const Vec3f& v2)
		{
			Vec4f q;
			auto a = v1.Cross(v2);
			q.x = a.x;
			q.y = a.y;
			q.z = a.z;
			q.w = sqrt((v1.Length() * v1.Length()) * (v2.Length() * v2.Length())) + v1.Dot(v2);
			return q.FastNormalize();
		}

		Vec4f QuaternionLookRotation(const Vec3f& dir, const Vec3f& up = Vec3f(0.0f, 1.0f, 0.0f))
		{
			if (dir == Vec3f::Zero())
			{
				return Vec4f::Identity();
			}

			if (up != dir)
			{
				auto cUp = up.FastNormalize();
				auto v = dir + cUp * -cUp.Dot(dir);
				v = v.FastNormalize();
				auto q = Vec3f(0, 0, 1).QuaternionLookAt(v);
				return v.QuaternionLookAt(dir) * q;
			}
			else
			{
				return Vec3f(0, 0, 1).QuaternionLookAt(dir);
			}
		}

		static Vec4f QLR(Vec3f forward, Vec3f up)
		{
			const Vec3f f = forward.FastNormalize();
			const Vec3f s = up.Cross(f).FastNormalize();
			const Vec3f u = f.Cross(s);

			float z = 1 + s.x + u.y + f.z;
			float fd = 2 * sqrt(z);

			Vec4f q;

			if (z > 0.00001f)
			{
				q.w = 0.25f * fd;
				q.x = (f.y - u.z) / fd;
				q.y = (s.z / f.x) / fd;
				q.z = (u.x - s.y) / fd;
			}
			else if (s.x > u.y && s.x > f.z)
			{
				fd = 2 * sqrt(1 + s.x - u.y - f.z);
				q.w = (f.y - u.z) / fd;
				q.x = 0.25f;
				q.y = (u.x + s.y) / fd;
				q.z = (s.z + f.x) / fd;
			}
			else if (u.y > f.z)
			{
				fd = 2 * sqrt(1 + u.y - s.x - f.z);
				q.w = (s.z - f.x) / fd;
				q.x = (u.x + s.y) / fd;
				q.y = 0.25f * fd;
				q.z = (f.y + u.z) / fd;
			}
			else
			{
				fd = 2 * sqrt(1 + f.z - s.x - u.y);
				q.w = (u.x - s.y) / fd;
				q.x = (s.z + f.x) / fd;
				q.y = (f.y + u.z) / fd;
				q.z = 0.25f;
			}

			return q;
		}

		static Vec4f QLookRotation(Vec3f forward, Vec3f up)
		{
			forward = forward.FastNormalize();

			Vec3f vector = forward.FastNormalize();
			Vec3f vector2 = up.Cross(vector).FastNormalize();
			Vec3f vector3 = vector.Cross(vector2);

			auto m00 = vector2.x;
			auto m01 = vector2.y;
			auto m02 = vector2.z;
			auto m10 = vector3.x;
			auto m11 = vector3.y;
			auto m12 = vector3.z;
			auto m20 = vector.x;
			auto m21 = vector.y;
			auto m22 = vector.z;

			float num8 = (m00 + m11) + m22;
			Vec4f q;

			if (num8 > 0.0f)
			{
				auto num = (float)sqrt(num8 + 1.0f);
				q.w = num * 0.5f;

				num = 0.5f / num;
				q.x = (m12 - m21) * num;
				q.y = (m20 - m02) * num;
				q.z = (m01 - m10) * num;
				return q;
			}
			if ((m00 >= m11) && (m00 >= m22))
			{
				auto num7 = (float)sqrt(((1.0f + m00) - m11) - m22);
				auto num4 = 0.5f / num7;
				q.x = 0.5f * num7;
				q.y = (m01 + m10) * num4;
				q.z = (m02 + m20) * num4;
				q.w = (m12 - m21) * num4;
				return q;
			}
			if (m11 > m22)
			{
				auto num6 = (float)sqrt(((1.0f + m11) - m00) - m22);
				auto  num3 = 0.5f / num6;
				q.x = (m10 + m01) * num3;
				q.y = 0.5f * num6;
				q.z = (m21 + m12) * num3;
				q.w = (m20 - m02) * num3;
				return q;
			}

			auto num5 = (float)sqrt(((1.0f + m22) - m00) - m11);

			auto num2 = 0.5f / num5;
			q.x = (m20 + m02) * num2;
			q.y = (m21 + m12) * num2;
			q.z = 0.5f * num5;
			q.w = (m01 - m10) * num2;
			return q;
		}

		Vec4f QuaternionLookAt(const Vec3f& dest, const Vec3f& fallbackAxis = Vec3f::Zero()) const
		{
			//Taken from Ogre3D (OgreVector3)

			Vec4f q;

			Vec3f v0 = *this;
			Vec3f v1 = dest;

			v0 = v0.FastNormalize();
			v1 = v1.FastNormalize();

			F32 d = v0.Dot(v1);

			if (d >= 1.0f)
			{
				return Vec4f::Identity();
			}

			if (d < (1e-6f - 1.0f))
			{
				if (fallbackAxis != Vec3f::Zero())
				{
					F32 s = sinf(E_PI / 2);
					Elixir::Vec4f q(0);
					q.x = fallbackAxis.x * s;
					q.y = fallbackAxis.y * s;
					q.z = fallbackAxis.z * s;
					q.w = cosf(E_PI / 2);
					//q = q.FastNormalize();
				}
				else
				{
					Vec3f axis(1.0f, 0.0f, 0.0f);
					axis = axis.Cross(*this);
					if (axis.isZeroLength())
					{
						axis = Vec3f(0.0f, 1.0f, 0.0f).Cross(*this);
					}

					axis = axis.FastNormalize();

					F32 s = sinf(E_PI / 2);
					Elixir::Vec4f q(0);
					q.x = axis.x * s;
					q.y = axis.y * s;
					q.z = axis.z * s;
					q.w = cosf(E_PI / 2);
					//q = q.FastNormalize();
				}
			}
			else
			{
				F32 s = sqrt((1 + d) * 2);
				F32 invs = 1 / s;

				Vec3f c = v0.Cross(v1);
				q.x = c.x * invs;
				q.y = c.y * invs;
				q.z = c.z * invs;
				q.w = s * 0.5f;
				q = q.FastNormalize();
			}

			return q;
		}

		F32 x;
		F32 y;
		F32 z;
	};

	struct CatmullPoint
	{
		CatmullPoint()
			:Position(0),
			Tangent(0)
		{};

		Vec3f Position;
		Vec3f Tangent;
	};

	struct Quaternion
	{
		
	};

	//An element with a string and an ID
	struct StringID
	{
		StringID() {};

		StringID(std::string n, U32 i)
			:
			name(n), ID(i)
		{}

		std::string name;
		U32 ID;
	};

	struct TransformData
	{
		TransformData()
			:exists(false)
		{};

		bool exists;

		bool dynamic;
		Vec3f pos;
		Vec3f rot;
		Vec3f sca;
		Vec3f tpos;
		Vec3f trot;
		Vec3f tsca;
	};

	struct RendererData
	{
		RendererData()
			:exists(false)
		{};

		bool exists;

		bool enabled;
		bool backface;
		bool castShadow;

		//IDs
		U32 alb;
		U32 norm;
		U32 rou;
		U32 met;

		std::string modelPath;
		bool modelTypePrimitive;
		U32 primitiveType;
	};

	struct ObjectID
	{
		U32 ID;
		TransformData transform;
		RendererData renderer;
	};

	struct ProjectData
	{
		std::vector<StringID> TextureData;
		std::vector<StringID> MaterialData;
		std::vector<StringID> SceneData;
	};

	struct SceneData
	{
		std::string name;
		std::vector<ObjectID> GameObjects;
	};

	enum OBJECT_PRESET
	{
		//Empty object. With no components added.
		OBJECT_EMPTY,
		//Object with Transform component added.
		OBJECT_TRANSFORM,
		//Object with Transform/Renderer3D components added.
		OBJECT_RENDER,
		//Object with Transform/Renderer2D components added.
		OBJECT_2D
	};
	

	struct Material
	{
		Material()
			:albedo(1), normal(2), roughness(3), metallic(4), name("NewMaterial")
		{};

		//Set material
		Material(U32 alb, U32 norm, U32 rough, U32 metal)
			:albedo(alb), normal(norm), roughness(rough), metallic(metal), name("NewMaterial")
		{};

		//Copy material
		Material(const Material &cpy)
			:albedo(cpy.albedo), normal(cpy.normal), roughness(cpy.roughness), metallic(cpy.metallic), name(cpy.name)
		{};

		// 0 means unset
		U32 albedo;
		U32 normal;
		U32 roughness;
		U32 metallic;

		U32 materialID;

		std::string name;
	};
}

struct Vertex2
{
	Vertex2() {}
	Vertex2(const DirectX::XMFLOAT3& p, const DirectX::XMFLOAT3& n, const DirectX::XMFLOAT3& t, const DirectX::XMFLOAT2& uv)
		:Position(p), Normal(n), TangentU(t), TexC(uv) {}
	Vertex2(
		float px, float py, float pz,
		float nx, float ny, float nz,
		float tx, float ty, float tz,
		float u, float v)
		:Position(px, py, pz), Normal(nx, ny, nz),
		TangentU(tx, ty, tz), TexC(u, v) {}

	DirectX::XMFLOAT3 Position;
	DirectX::XMFLOAT3 Normal;
	DirectX::XMFLOAT2 TexC;
	DirectX::XMFLOAT3 TangentU;
	
};

struct Vertex
{
	Vertex() {}
	Vertex(const DirectX::XMFLOAT3& p, const DirectX::XMFLOAT3& n, const DirectX::XMFLOAT3& t, const DirectX::XMFLOAT2& uv)
		:Position(p), Normal(n), TangentU(t), Tex(uv) {}
	Vertex(
		float px, float py, float pz,
		float nx, float ny, float nz,
		float tx, float ty, float tz,
		float u, float v)
		:Position(px, py, pz), Normal(nx, ny, nz),
		TangentU(tx, ty, tz), Tex(u, v) {}

	DirectX::XMFLOAT3 Position;
	DirectX::XMFLOAT3 Normal;
	DirectX::XMFLOAT2 Tex;
	DirectX::XMFLOAT3 TangentU;

};

struct SpriteVertex
{
	DirectX::XMFLOAT3 Position;
	DirectX::XMFLOAT2 Texture;
};

struct MeshData
{
	std::vector<Vertex> Vertices;
	std::vector<unsigned int> Indices;
};

struct offsetData
{
	offsetData() 
		:vertexOffset(0), indexCount(0), indexOffset(0)
	{}
	offsetData(UINT iCount, UINT iOffset, UINT vOffset)
		:indexCount(iCount), indexOffset(iOffset), vertexOffset(vOffset) {}
	UINT indexCount;
	UINT indexOffset;
	UINT vertexOffset;
};

struct BoundingSphere
{
	BoundingSphere()
		:Center(0.0f, 0.0f, 0.0f),
		Radius(0.0f)
	{}

	BoundingSphere(float x, float y, float z, float r)
		:Center(x, y, z),
		Radius(r)
	{}

	DirectX::XMFLOAT3 Center;
	float Radius;
};

enum RENDER_MODE
{
	DEFERRED_RENDERING,
	FOREWARD_RENDERING
};

enum TEXTURE_TYPE
{
	ALBEDO,
	IRRADIANCE,
	ENV_MAP,
	ROUGHNESS,
	METALLIC,
	NORMAL
};