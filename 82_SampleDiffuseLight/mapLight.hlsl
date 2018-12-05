struct VS_IN
{
	float3 p : POSITION;
	float3 n : NORMAL;
	float4 c : COLOR;
	float2 t : TEXCOORD;
};
struct VS_OUTPUT
{
	float4 p : SV_POSITION;
	float3 n : NORMAL;
	float4 c : COLOR0;
	float2 t : TEXCOORD0;
	float4 l : TEXCOORD1;
};
struct GS_OUTPUT
{
	float4 p : SV_POSITION;
	float3 n : NORMAL;
	float4 c : COLOR0;
	float2 t : TEXCOORD0;
};
cbuffer cb0 : register(b0)
{
	float4x4  g_matWorld : packoffset(c0);
	matrix  g_matView : packoffset(c4);
	matrix  g_matProj : packoffset(c8);

	float  g_fTimer : packoffset(c12.x);
	float  y : packoffset(c12.y);
	float  z : packoffset(c12.z);
	float  w : packoffset(c12.w);

	float4 g_Color : packoffset(c13);
};

cbuffer cb1 : register(b1)
{
	matrix g_matInvWorld : packoffset(c0);
	float4 g_vLightDir : packoffset(c4);
	float4 g_vLightPos : packoffset(c5);
	float4 g_vEyeDir : packoffset(c6);
	float4 g_vEyePos : packoffset(c7);
};
Texture2D     g_txDiffuse : register(t0);
SamplerState  g_samLinear : register(s0);

VS_OUTPUT VS(VS_IN v)
{
	VS_OUTPUT vOut = (VS_OUTPUT)0;
	float4 vWorld = mul(float4(v.p,1.0f), g_matWorld);
	float4 vView = mul(vWorld, g_matView);
	float4 vProj = mul(vView, g_matProj);
	vOut.p = vProj;
	vOut.n = mul(v.n, (float3x3)g_matInvWorld);
	vOut.c = v.c;// g_Color;

	//버텍스셰이더에 조명을 준거.
	/*float fDot = dot(vOut.n, -g_vLightDir.xyz);
	vOut.l = float4(fDot, fDot, fDot, 1.0f);*/

	vOut.l.xyz = -g_vLightDir.xyz;
	vOut.t = v.t;
	return vOut;
}
VS_OUTPUT VS_NoMatrix(VS_IN v)
{
	VS_OUTPUT vOut = (VS_OUTPUT)0;	
	vOut.p = float4(v.p, 1.0f);
	vOut.n = v.n;
	vOut.c = v.c;// g_Color;
	vOut.t = v.t;
	return vOut;
}

//--------------------------------------------------------------------------------------
// Pixel Shader
//--------------------------------------------------------------------------------------
float4 Diffuse(float3 vNormal, float3 vWolrd, uint iNumLight)
{
	float fIntensity = 0.0f;
	float fLuminance = 0.0f;

	float4 cLightColor = float4(0.0f, 0.0f, 0.0f, 1.0f);
	for (int iLight = 0; iLight < iNumLight; iLight++)
	{
		float3 vLight = normalize(vWolrd - g_vLightPos);
		float fLength = distance(vWolrd, g_vLightPos.xyz);

		fIntensity = saturate(dot(-vLight, vNormal));
		fLuminance = smoothstep(fLength - 15, fLength, g_vLightPos.w);
		cLightColor += float4(1, 1, 1, 1) * fLuminance * fIntensity;
	}
	cLightColor.a = 1.0f;
	return cLightColor;
}
float4 Specular(float3 vNormal, float3 vWolrd, uint iNumLight)
{
	float fIntensity = 0.0f;
	float fLuminance = 0.0f;

	float4 cLightColor = float4(0.0f, 0.0f, 0.0f, 1.0f);
	for (int iLight = 0; iLight < iNumLight; iLight++)
	{
		float3 vLight = normalize(vWolrd - g_vLightPos);
		float fLength = distance(vWolrd, g_vLightPos.xyz);
#ifndef HALF_VECTOR
		float3 vReflect = reflect(g_vEyeDir.xyz, vNormal);
		fIntensity = saturate(dot(-vLight, vReflect));
		float fPower = pow(fIntensity, g_vEyeDir.w);
#else
#endif
		fLuminance = smoothstep(fLength - 15, fLength, g_vLightPos.w);
		cLightColor += float4(1, 1, 1, 1) * fLuminance * fIntensity;
	}
	cLightColor.a = 1.0f;
	return cLightColor;
}

float4 PS(VS_OUTPUT v) : SV_Target
{
	float4 cAmbient = float4(0.2f, 0.2f, 0.2f, 1.0f);
	float4 cDiffuse = Diffuse(v.l.xyz, v.n, 1);
	float4 cSpecular = Specular(v.l.xyz, v.n, 1);
	float4 vTexColor = g_txDiffuse.Sample(g_samLinear, v.t);
	vTexColor = vTexColor * (cDiffuse + cSpecular + cAmbient);
	vTexColor.a = 1.0f;
	return vTexColor;
	//return vTexColor * v.c * v.l;
	//return v.c;

	//조명 적용.
	/*float3 vLight = normalize(float3(100.0f, 100.0f, 100.0f));
	float fDot = dot(vLight, v.n);
	v.c.x = fDot;
	v.c.y = fDot;
	v.c.z = fDot;
	v.c.w = 1.0f;
	float4 fAmbient = float4(0.3f, 0.3f, 0.3f, 0.0f);
	return vTexColor * (v.c + fAmbient);*/
}
float4 PSLine(VS_OUTPUT v) : SV_Target
{
	return v.c;
}
float4 PSColor(VS_OUTPUT v) : SV_Target
{
	return g_Color;
}