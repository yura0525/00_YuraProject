struct VS_OUTPUT
{
	float4 p : SV_POSITION;
	float4 c : COLOR0;
};

//triangle TriangleStream, Ʈ���̾ޱ��̹Ƿ� ������ 3���̴�.
//point pointStream, line lineStream
//ó���ϰ� TriangleStream�� �����ؾ� ó���� �ȴ�.
[maxvertexcount(9)]
void GS(triangle VS_OUTPUT input[3], 
	inout TriangleStream<VS_OUTPUT> triStream)
{
	//�ﰢ���� �ٽ� ������ �������� �ﰢ�� 3���� ������,
	//triStream�� �ִ´�.
	//v0, v1, cPos
	//v1, v2, cPos
	//v2, v0, cPos
	float3 cPos = (input[0].p.xyz + input[1].p.xyz + input[2].p.xyz) / 3.0f;
	float4 cColor = (input[0].c + input[1].c + input[2].c) / 3.0f;

	VS_OUTPUT newVertex;
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
	}
	
	triStream.RestartStrip();
}