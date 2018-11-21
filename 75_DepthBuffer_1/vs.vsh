struct VS_IN
{
	float3 p : POSITION;
	float4 c : COLOR;
};

struct VS_OUTPUT
{
	float4 p : SV_POSITION;
	float4 c : COLOR;
};

cbuffer cb0
{
	matrix	g_matWorld: packoffset(c0);
	matrix	g_matView: packoffset(c4);
	matrix	g_matProj: packoffset(c8);

	float g_fTimer : packoffset(c12.x);
	float active : packoffset(c12.y);
	float z : packoffset(c12.z);
	float w : packoffset(c12.w);
	float4 g_Color : packoffset(c13);
};

VS_OUTPUT VS(VS_IN v)
{
	VS_OUTPUT vOut = (VS_OUTPUT)0;
	float4 vPos = float4(v.p, 1.0f);
	float4 vWorld = mul(vPos, g_matWorld);
	float4 vView = mul(vWorld, g_matView);
	float4 vProj = mul(vView, g_matProj);

	vOut.p = vProj;
	vOut.c = g_Color;
	//원래는 g_fTimer값에 따라 중점으로 갔다가 늘어났다 해야하는데,
	//vOut.p.w = 1.0f;을 해줘야 정상 작동한다.
	//vOut.p.w = 1.0f;
	//vOut.c = v.c * g_fTimer;
	return vOut;
}