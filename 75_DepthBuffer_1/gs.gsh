struct VS_OUTPUT
{
	float4 p : SV_POSITION;
	float4 c : COLOR;
};

struct GS_OUTPUT
{
	float4 p : SV_POSITION;
	float4 c : COLOR;
	//uint vp : SV_ViewportArrayIndex;
};

//cbuffer cb0
//{
//	matrix	g_matWorld: packoffset(c0);
//	matrix	g_matView: packoffset(c4);
//	matrix	g_matProj: packoffset(c8);
//
//	float g_fTimer : packoffset(c12.x);
//	float active : packoffset(c12.y);
//	float z : packoffset(c12.z);
//	float w : packoffset(c12.w);
//	float4 g_Color : packoffset(c13);
//};

//triangle TriangleStream, 트라이앵글이므로 정점이 3개이다.
//point pointStream, line lineStream
//처리하고 TriangleStream에 저장해야 처리가 된다.
[maxvertexcount(9)]
void GS(triangle VS_OUTPUT input[3], 
	inout TriangleStream<GS_OUTPUT> triStream)
{
	//삼각형을 다시 중점을 기준으로 삼각형 3개로 나누고,
	//triStream에 넣는다.
	//v0, v1, cPos
	//v1, v2, cPos
	//v2, v0, cPos
	float3 cPos = (input[0].p.xyz + input[1].p.xyz + input[2].p.xyz) / 3.0f;
	float4 cColor = (input[0].c + input[1].c + input[2].c) / 3.0f;

	GS_OUTPUT newVertex;
	//newVertex.vp = active;

	for (int i = 0; i < 3; i++)
	{
		newVertex.p = input[i].p;
		newVertex.c = input[i].c;
		triStream.Append(newVertex);

		int iNext = (i + 1) % 3;
		newVertex.p = input[iNext].p;
		newVertex.c = input[iNext].c;
		triStream.Append(newVertex);

		newVertex.p = float4(cPos, 1.0f);
		newVertex.c = float4(0,0,0,1);
		triStream.Append(newVertex);
		triStream.RestartStrip();
	}
	
	triStream.RestartStrip();
}