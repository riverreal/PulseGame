#pragma once

#include <Windows.h>
#include <DirectXMath.h>
#include <vector>

using namespace DirectX;

namespace Elixir
{
	struct Vec3f;
	struct Vec4f;
	struct CatmullPoint;
}

class MathHelper
{
public:
	template<typename T>
	static T Min(const T& a, const T& b)
	{
		return a < b ? a : b;
	}

	template<typename T>
	static T Max(const T& a, const T& b)
	{
		return a > b ? a : b;
	}

	static float Deg2Rad(float degree);

	static float clamp(float x, float min, float max);

	static float AngleFromXY(float x, float y);

	static XMMATRIX WorldToBillboard(const XMMATRIX &source, XMFLOAT3 eyePos);

	static const float Pi;

	static float lerp(float a, float b, float x);
	static XMFLOAT3 lerp(XMFLOAT3 a, XMFLOAT3 b, float x);

	static XMFLOAT3 F3DotScalar(XMFLOAT3 x, float s);
	static XMFLOAT3 F3MinusF3(XMFLOAT3 a, XMFLOAT3 b);

	static XMFLOAT3 Vec3f2XMF3(Elixir::Vec3f vec3);

	//Rotation from axis and angle
	static Elixir::Vec4f QuaternionAxisAngle(Elixir::Vec3f axis, float radians);
	static Elixir::Vec3f Quaternion2Euler(Elixir::Vec4f quat);

	static float GetT(float t, Elixir::Vec3f p0, Elixir::Vec3f p1, float alpha);

	//Needs a set of points (minimum of 4 points) as parameters.
	//Returns a smooth curve passing through each point. It returns the tangent of it's points if @param tanget is true
	static std::vector<Elixir::Vec3f> CatmullromSpline(std::vector<Elixir::Vec3f> points, int subdivision, bool tangent);
	static std::vector<Elixir::Vec3f> cmrSpline(std::vector<Elixir::Vec3f> points, int subdivision, bool tangent);

	//Returns a point of a CatmullSpline curve by a t [0, 1]
	static Elixir::CatmullPoint GetPointInCMSpline(Elixir::Vec3f P0, Elixir::Vec3f P1, Elixir::Vec3f P2, Elixir::Vec3f P3, float t);
};