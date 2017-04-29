Texture2D albedoTex : register(t0);
SamplerState sampler2d : register(s0);

struct PixelInput
{
	float4 position: SV_POSITION;
	float2 tex: TEXCOORD0;
	float4 color: COLOR;
};

float4 Texture2DPS(PixelInput input) : SV_TARGET
{
	float4 finalColor;
	finalColor = albedoTex.Sample(sampler2d, input.tex);
	finalColor *= input.color;

	return finalColor;
}