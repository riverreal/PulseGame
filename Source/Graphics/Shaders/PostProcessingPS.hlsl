#include "PostProcessing.hlsli"

Texture2D gImg : register(t0);
Texture2D gAdaptedLum : register(t1);
Texture2D gBloom : register(t2);
Texture2D gAnaLensFlare : register(t3);

SamplerState samplerLinear;
//true = 1
//false = 0
#define TONEMAPPING 0
#define EYE_ADAPTATION 0
#define FXAA 0
#define HDR_BLOOM 0
#define ANAMORPHIC_LENS_FLARE 0

cbuffer PostProcessParam : register(b0)
{
	uint gEnabled;
	float3 pad;
};

struct VertexOut
{
	float4 PosL : SV_POSITION;
	float2 tex : TEXCOORD0;
};

struct PixelOut
{
	float4 color : SV_Target0;
};

PixelOut PostProcessingPS(VertexOut input) : SV_TARGET
{
	PixelOut output;
	output.color = gImg.Sample(samplerLinear, input.tex);

	if (gEnabled > 0)
	{
#if FXAA
		uint dx, dy;
		gImg.GetDimensions(dx, dy);
		float2 rcpro = rcp(float2(dx, dy));

		FxaaTex t;
		t.smpl = samplerLinear;
		t.tex = gImg;

		//FXAA function samples the texture
		output.color = FxaaPixelShader(input.tex, 0, t, t, t, rcpro, 0, 0, 0, 1.0, 0.166, 0.0312, 0, 0, 0, 0);
#endif

#if TONEMAPPING
#if HDR_BLOOM
		output.color += float4(gBloom.Sample(samplerLinear, input.tex).rgb, 1.0f);
#endif //HDR_BLOOM

#if ANAMORPHIC_LENS_FLARE
		output.color += float4(gAnaLensFlare.Sample(samplerLinear, input.tex).rgb, 1.0f);
#endif //ANAMORPHIC_LENS_FLARE

		float exposure = 0.5f;
#if EYE_ADAPTATION
		float4 adaptedLum = gAdaptedLum.Sample(samplerLinear, input.tex);

		exposure = GetExposure(adaptedLum.r, 0.05, 0.998, 0.001f);
#endif
		output.color *= exposure;
		output.color = FilmicTonemap(output.color);
#else
		output.color.rgb = pow(output.color.rgb, 1 / 2.2);

#endif //TONEMAPPING



	}

	return output;
}