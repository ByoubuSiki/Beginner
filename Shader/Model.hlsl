#include"BasicHeader.hlsli"

float4 main(BasicOutPut input) : SV_TARGET
{
	return float4(input.normal.xyz, 1.0f);
}