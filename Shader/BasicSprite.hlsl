#include"BasicHeader.hlsli"

Texture2D<float4> tex : register(t0); //0番スロットに追加されたテクスチャ
SamplerState smp : register(s0); //0番ストットに設定されたサンプラー


float4 main(BasicOutPut input) : SV_TARGET
{
    return float4(tex.Sample(smp, input.svUV));
}