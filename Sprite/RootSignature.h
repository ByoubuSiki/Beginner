#pragma once

#include"..\Base\Pch.h"

namespace Beginner
{

	class RootSignature
	{
	private:
		D3D12_ROOT_SIGNATURE_DESC signatureDesc;

	public:
		RootSignature() :signatureDesc() { signatureDesc.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT; }
		~RootSignature() {}

		D3D12_ROOT_SIGNATURE_DESC* GetRootSignature();//Getter
		void SetParametor(const UINT, const D3D12_ROOT_PARAMETER*);//Parametorê›íË
		void SetStaticSamper(const UINT, const D3D12_STATIC_SAMPLER_DESC*);//Samplerê›íË
	};
}