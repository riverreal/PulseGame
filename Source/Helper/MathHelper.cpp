#include "MathHelper.h"
#include "TypeHelper.h"
#include "GeneralHelper.h"
#include <cmath>

const float MathHelper::Pi = 3.1415926535f;


float MathHelper::clamp(float x, float min, float max)
{
	x = Max(x, min);
	x = Min(x, max);
	return x;
}

float MathHelper::AngleFromXY(float x, float y)
{
	float theta = 0.0f;

	if (x >= 0.0f)
	{
		theta = atanf(y / x);

		if (theta < 0.0f)
		{
			theta += 2.0f*Pi;
		}
	}
	else
	{
		theta = atanf(y / x) + Pi;
	}

	return theta;
}

XMMATRIX MathHelper::WorldToBillboard(const XMMATRIX &source, XMFLOAT3 eyePos)
{
	XMMATRIX billboardMatrix;
	float angle;
	XMFLOAT3 pos;
	XMStoreFloat3(&pos, source.r[3]);

	angle = atan2(pos.x - eyePos.x, pos.z - eyePos.z);
	billboardMatrix = source;
	billboardMatrix = XMMatrixMultiply(billboardMatrix, XMMatrixRotationY(XMConvertToRadians( angle)));
	return billboardMatrix;
}

float MathHelper::lerp(float a, float b, float x)
{
	return a + x * (b - a);
}

XMFLOAT3 MathHelper::lerp(XMFLOAT3 a, XMFLOAT3 b, float x)
{
	return XMFLOAT3(lerp(a.x, b.x, x), lerp(a.y, b.y, x), lerp(a.z, b.z, x));
}

XMFLOAT3 MathHelper::F3DotScalar(XMFLOAT3 x, float s)
{
	return XMFLOAT3(x.x*s, x.y*s, x.z*s);
}

XMFLOAT3 MathHelper::F3MinusF3(XMFLOAT3 a, XMFLOAT3 b)
{
	return XMFLOAT3(a.x-b.x, a.y-b.y, a.z-b.z);
}

XMFLOAT3 MathHelper::Vec3f2XMF3(Elixir::Vec3f vec3)
{
	return XMFLOAT3(vec3.x, vec3.y, vec3.z);
}

Elixir::Vec4f MathHelper::QuaternionAxisAngle(Elixir::Vec3f axis, float radians)
{
	F32 s = sinf(radians / 2);
	Elixir::Vec4f q(0);
	q.x = axis.x * s;
	q.y = axis.y * s;
	q.z = axis.z * s;
	q.w = cosf(radians / 2);

	return q;
}

Elixir::Vec3f MathHelper::Quaternion2Euler(Elixir::Vec4f q)
{
	Elixir::Vec3f euler;

	F32 ysqr = q.y * q.y;

	//x
	F32 t0 = 2.0 * (q.w * q.x + q.y * q.z);
	F32 t1 = 1.0f - 2.0f * (q.x * q.x + ysqr);
	euler.x = atan2f(t0, t1);

	//y
	F32 t2 = 2.0 * (q.w * q.y - q.z * q.x);
	t2 = clamp(t2, -1.0f, 1.0f);
	euler.y = asinf(t2);

	//z
	F32 t3 = 2.0f * (q.w * q.z + q.x * q.y);
	F32 t4 = 1.0f - 2.0f * (ysqr + q.z * q.z);
	euler.z = atan2(t3, t4);

	return euler;
}

float MathHelper::GetT(float t, Elixir::Vec3f p0, Elixir::Vec3f p1, float alpha)
{
	//Uniform		alpha = 0
	//Centripetal	alpha = 0.5
	//Chordal		alpha = 1

	//float alpha = 0.0f;

	float a = pow((p1.x - p0.x), 2.0f) + pow((p1.y - p0.y), 2.0f) + pow((p1.z - p0.z), 2.0f);
	float b = pow(a, 0.5f);
	float c = pow(b, alpha);

	return (c + t);
}

std::vector<Elixir::Vec3f> MathHelper::CatmullromSpline(std::vector<Elixir::Vec3f> line, int subdivision, bool tangent)
{
	std::vector<Elixir::Vec3f> smoothCurve;

	float alpha = 0.0f;

	for (int i = 0; i < line.size() - 1; ++i)
	{
		if (i == 0 || i + 2 == line.size())
		{
			continue;
		}

		Elixir::Vec3f P0, P1, P2, P3;
		P0 = i == 0 ? line[i] : line[i - 1];
		P1 = line[i];
		P2 = line[i + 1];
		P3 = (i + 2) == line.size() ? line[i + 1] : line[i + 2];

		float t0 = 0.0f;
		float t1 = GetT(t0, P0, P1, alpha);
		float t2 = GetT(t1, P1, P2, alpha);
		float t3 = GetT(t2, P2, P3, alpha);

		for (float t = t1; t < t2; t += ((t2 - t1) / subdivision))
		{
			Elixir::Vec3f A1, A2, A3, B1, B2, C;

			A1 = P0 * ((t1 - t) / (t1 - t0)) + P1 * ((t - t0) / (t1 - t0));
			A2 = P1 * ((t2 - t) / (t2 - t1)) + P2 * ((t - t1) / (t2 - t1));
			A3 = P2 * ((t3 - t) / (t3 - t2)) + P3 * ((t - t2) / (t3 - t2));

			B1 = A1 * ((t2 - t) / (t2 - t0)) + A2 * ((t - t0) / (t2 - t0));
			B2 = A2 * ((t3 - t) / (t3 - t1)) + A3 * ((t - t1) / (t3 - t1));

			if (!tangent)
			{
				C = B1 * ((t2 - t) / (t2 - t1)) + B2 * ((t - t1) / (t2 - t1));

				//skip duplicates
				if (!smoothCurve.empty() && smoothCurve.back() == C)
				{
					continue;
				}

				smoothCurve.push_back(C);

				continue;
			}
			
			//Calculate derivatives--------------------

			auto DA1 = (P1 - P0) * (1 / (t1 - t0));
			auto DA2 = (P2 - P1) * (1 / (t2 - t1));
			auto DA3 = (P3 - P2) * (1 / (t3 - t2));

			auto DB1 = (A2 - A1) * (1 / (t2 - t0)) + DA1 * ((t2 - t) / (t2 - t0)) + DA2 * ((t - t0) / (t2 - t0));
			auto DB2 = (A3 - A2) * (1 / (t3 - t1)) + DA2 * ((t3 - t) / (t3 - t1)) + DA3 * ((t - t1) / (t3 - t1));

			auto DC = (B2 - B1) * (1 / (t2 - t1)) + DB1 * t * ((t2 - t) / (t2 - t1)) + DB2 * t * ((t - t1) / (t2 - t1));

			//skip duplicates
			if (!smoothCurve.empty() && smoothCurve.back() == DC)
			{
				continue;
			}

			smoothCurve.push_back(DC);
		}
	}

	return smoothCurve;
}

std::vector<Elixir::Vec3f> MathHelper::cmrSpline(std::vector<Elixir::Vec3f> points, int subdivision, bool tangent)
{
	std::vector<Elixir::Vec3f> smoothCurve;

	float alpha = 1.0f;

	for (int i = 0; i < points.size() - 1; ++i)
	{
		if (i == 0 || i + 2 == points.size())
		{
			continue;
		}

		Elixir::Vec3f P0, P1, P2, P3;
		P0 = i == 0 ? points[i] : points[i - 1];
		P1 = points[i];
		P2 = points[i + 1];
		P3 = (i + 2) == points.size() ? points[i + 1] : points[i + 2];

		float t0 = 0.0f;
		float t1 = GetT(t0, P0, P1, alpha);
		float t2 = GetT(t1, P1, P2, alpha);
		float t3 = GetT(t2, P2, P3, alpha);

		for (float t = t1; t < t2; t += ((t2 - t1) / subdivision))
		{
			Elixir::Vec3f A1, A2, A3, B1, B2, C;

			A1 = P0 * ((t1 - t) / (t1 - t0)) + P1 * ((t - t0) / (t1 - t0));
			A2 = P1 * ((t2 - t) / (t2 - t1)) + P2 * ((t - t1) / (t2 - t1));
			A3 = P2 * ((t3 - t) / (t3 - t2)) + P3 * ((t - t2) / (t3 - t2));

			B1 = A1 * ((t2 - t) / (t2 - t0)) + A2 * ((t - t0) / (t2 - t0));
			B2 = A2 * ((t3 - t) / (t3 - t1)) + A3 * ((t - t1) / (t3 - t1));

			if (!tangent)
			{
				C = B1 * ((t2 - t) / (t2 - t1)) + B2 * ((t - t1) / (t2 - t1));

				//skip duplicates
				if (!smoothCurve.empty() && smoothCurve.back() == C)
				{
					continue;
				}

				smoothCurve.push_back(C);

				continue;
			}
			
			//Calculate derivatives--------------------

			auto DA1 = (P1 - P0) * (1 / (t1 - t0));
			auto DA2 = (P2 - P1) * (1 / (t2 - t1));
			auto DA3 = (P3 - P2) * (1 / (t3 - t2));

			auto DB1 = (A2 - A1) * (1 / (t2 - t0)) + DA1 * ((t2 - t) / (t2 - t0)) + DA2 * ((t - t0) / (t2 - t0));
			auto DB2 = (A3 - A2) * (1 / (t3 - t1)) + DA2 * ((t3 - t) / (t3 - t1)) + DA3 * ((t - t1) / (t3 - t1));

			auto DC = (B2 - B1) * (1 / (t2 - t1)) + DB1 * t * ((t2 - t) / (t2 - t1)) + DB2 * t * ((t - t1) / (t2 - t1));

			//skip duplicates
			if (!smoothCurve.empty() && smoothCurve.back() == DC)
			{
				continue;
			}

			smoothCurve.push_back(DC);
		}
	}

	return smoothCurve;
}

Elixir::CatmullPoint MathHelper::GetPointInCMSpline(Elixir::Vec3f P0, Elixir::Vec3f P1, Elixir::Vec3f P2, Elixir::Vec3f P3, float t)
{
	Elixir::CatmullPoint cmp;

	t = clamp(t, 0.0f, 1.0f);

	float t0 = 0.0f;
	float t1 = GetT(t0, P0, P1, 0.0f);
	float t2 = GetT(t1, P1, P2, 0.0f);
	float t3 = GetT(t2, P2, P3, 0.0f);

	Elixir::Vec3f A1, A2, A3, B1, B2, C;

	//reparametarize t
	//t = (t2 - t1) * t
	t *= t2;

	A1 = P0 * ((t1 - t) / (t1 - t0)) + P1 * ((t - t0) / (t1 - t0));
	A2 = P1 * ((t2 - t) / (t2 - t1)) + P2 * ((t - t1) / (t2 - t1));
	A3 = P2 * ((t3 - t) / (t3 - t2)) + P3 * ((t - t2) / (t3 - t2));

	B1 = A1 * ((t2 - t) / (t2 - t0)) + A2 * ((t - t0) / (t2 - t0));
	B2 = A2 * ((t3 - t) / (t3 - t1)) + A3 * ((t - t1) / (t3 - t1));

	cmp.Position = B1 * ((t2 - t) / (t2 - t1)) + B2 * ((t - t1) / (t2 - t1));

	auto DA1 = (P1 - P0) * (1 / (t1 - t0));
	auto DA2 = (P2 - P1) * (1 / (t2 - t1));
	auto DA3 = (P3 - P2) * (1 / (t3 - t2));

	auto DB1 = (A2 - A1) * (1 / (t2 - t0)) + DA1 * ((t2 - t) / (t2 - t0)) + DA2 * ((t - t0) / (t2 - t0));
	auto DB2 = (A3 - A2) * (1 / (t3 - t1)) + DA2 * ((t3 - t) / (t3 - t1)) + DA3 * ((t - t1) / (t3 - t1));

	cmp.Tangent = (B2 - B1) * (1 / (t2 - t1)) + DB1 * t * ((t2 - t) / (t2 - t1)) + DB2 * t * ((t - t1) / (t2 - t1));
	cmp.Tangent = cmp.Tangent.FastNormalize();
	return cmp;
}

