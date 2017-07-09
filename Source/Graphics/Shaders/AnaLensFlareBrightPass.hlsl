#include "EyeAdaptation.hlsli"

Texture2D gHDRImage : register(t0);
Texture2D gAvrgLum : register(t1);

SamplerState samplerLinear;

struct VertexOut
{
	float4 PosL : SV_POSITION;
	float2 tex : TEXCOORD0;
};

float4 AnaLensFlareBrightPass(VertexOut input) : SV_TARGET
{
	float3 color = gHDRImage.Sample(samplerLinear, input.tex).rgb;
	float avgLum = gAvrgLum.Sample(samplerLinear, float2(0.5f, 0.5f)).r;

	//color.b *= 1.5f;

	color *= GetExposure(avgLum, 1.0, 3, 0.001);
	color -= 0.5f;
	color = max(color, 0.0f);
	color /= (10.0f + color);

	return float4(color, 1.0f);
}