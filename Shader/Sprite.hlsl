#include"BasicHeader.hlsli"

Texture2D<float4> tex : register(t0); //0�ԃX���b�g�ɒǉ����ꂽ�e�N�X�`��
SamplerState smp : register(s0); //0�ԃX�g�b�g�ɐݒ肳�ꂽ�T���v���[


float4 main(BasicOutPut input) : SV_TARGET
{
    return float4(tex.Sample(smp, input.svUV));
}