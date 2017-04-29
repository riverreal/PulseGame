cbuffer Texture2DBuffer
{
	float4 gTransform;
	float4 gColor;
};

struct VertexInput
{
	float4 position: POSITION;
	float2 tex: TEXCOORD0;
};

struct PixelInput
{
	float4 position: SV_POSITION;
	float2 tex: TEXCOORD0;
	float4 color: COLOR;
};

PixelInput Texture2DVS(uint vI: SV_VERTEXID)
{
	PixelInput output;

	output.position.x = (float)(vI / 2) * 4.0 - 1.0;
	output.position.y = (float)(vI % 2) * 4.0 - 1.0;
	output.position.z = 0.0;
	output.position.w = 1.0;

	output.tex.x = (float)(vI / 2) * 2.0;
	output.tex.y = 1.0 - (float)(vI % 2) * 2.0;

	output.position.xy *= gTransform.zw;
	output.position.xy += gTransform.xy;

	output.color = gColor;

	return output;
}