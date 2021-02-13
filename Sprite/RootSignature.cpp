#include"RootSignature.h"

namespace Beginner
{

	//Getter
	D3D12_ROOT_SIGNATURE_DESC* RootSignature::GetRootSignature()
	{
		return &signatureDesc;
	}

	//Parametor�ݒ�
	void RootSignature::SetParametor(const UINT num, const D3D12_ROOT_PARAMETER* address)
	{
		signatureDesc.NumParameters = num;
		signatureDesc.pParameters = address;
	}

	//Sampler�ݒ�
	void RootSignature::SetStaticSamper(const UINT num, const D3D12_STATIC_SAMPLER_DESC* address)
	{
		signatureDesc.NumStaticSamplers = num;
		signatureDesc.pStaticSamplers = address;
	}
}