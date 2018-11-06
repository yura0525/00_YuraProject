struct VS_IN
{
	float3 p : POSITION;
	float4 c : COLOR0;
};

struct VS_OUTPUT
{
	float4 p : SV_POSITION;
	float4 c : COLOR0;
};

VS_OUTPUT VS(VS_IN v)
{
	VS_OUTPUT vOut = (VS_OUTPUT)0;
	vOut.p = float4(v.p, 1.0f);
	vOut.c = v.c;
	return vOut;
}