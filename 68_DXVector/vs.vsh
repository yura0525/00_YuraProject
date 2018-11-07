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

cbuffer cb0
{
	float g_fTimer : packoffset(c0.x);
	float y : packoffset(c0.y);
	float z : packoffset(c0.z);
	float w : packoffset(c0.w);
};

VS_OUTPUT VS(VS_IN v)
{
	VS_OUTPUT vOut = (VS_OUTPUT)0;
	vOut.p = float4(v.p, 1.0f)  * g_fTimer;

	//원래는 g_fTimer값에 따라 중점으로 갔다가 늘어났다 해야하는데,
	//vOut.p.w = 1.0f;을 해줘야 정상 작동한다.
	vOut.p.w = 1.0f;
	vOut.c = v.c * g_fTimer;
	return vOut;
}