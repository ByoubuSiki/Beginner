#include"BasicHeader.hlsli"

cbuffer cbuff0 : register(b0)
{
    matrix transform;//ïœä∑çsóÒ
}

BasicOutPut main(
float4 pos : POSITION,float4 normal : NORMAL,
float2 uv : TEXCOORD,min16uint2 bone:BONE,
min16uint weight : WEIGHT)
{
    BasicOutPut output;
    output.svPosition = mul(transform, pos);
    output.svUV = uv;
    output.normal = normal;
    
    return output;
}