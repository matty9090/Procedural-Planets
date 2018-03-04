cbuffer MatrixBuffer {
	matrix worldMatrix;
	matrix viewMatrix;
	matrix projectionMatrix;
	float Wiggle;
	float Pad0, Pad1, Pad2;
};

struct VertexInputType {
	float3 Pos : POSITION;
	float3 Normal : NORMAL;
	float4 Color : COLOR;
};

struct PixelInputType {
	float4 Pos : SV_POSITION;
	float4 Color : COLOR;
};

PixelInputType main(VertexInputType input) {
	PixelInputType output;

	float4 modelPos = float4(input.Pos, 1.0f);
	float4 worldPos = mul(modelPos, worldMatrix);

	float4 modelNormal = float4(input.Normal, 1.0f);
	float4 worldNormal = normalize(mul(modelNormal, worldMatrix));

	/*worldPos += worldNormal * (sin(Wiggle + 1.0f)) * 0.12f;
	worldPos.x += sin(modelPos.y + Wiggle) * 0.12f;
	worldPos.y += sin(modelPos.x + Wiggle) * 0.12f;*/

	output.Pos = mul(worldPos, viewMatrix);
	output.Pos = mul(output.Pos, projectionMatrix);
	output.Color = input.Color;

	return output;
}