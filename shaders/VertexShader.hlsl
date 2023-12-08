struct VSOut {
	float4 color : COLOR;
	float4 position : SV_POSITION;
};

// Catching the constant buffer.

cbuffer CBF {
	matrix transform;
};

VSOut SDAH(float3 position : POSITION, float4 color : COLOR) {
	VSOut vso;
	vso.position = mul(float4(position.x, position.y, position.z, 1.0f), transform);
	vso.color = color;
	
	return vso;
}