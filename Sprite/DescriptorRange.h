#pragma once

#include"..\Base\Pch.h"

namespace Beginner
{
	class DescriptorRange
	{
	private:
		std::vector<D3D12_DESCRIPTOR_RANGE> descRange;//DescriptorRange�̉��ϔz��

	public:
		DescriptorRange() :descRange() {}
		~DescriptorRange() {}

		size_t GetSize()const;//DescriptorRange�z��̒���
		D3D12_DESCRIPTOR_RANGE* GetDescRange(const size_t);//Getter
		void PushBack(const D3D12_DESCRIPTOR_RANGE);//�z��ɒǉ�
		void PushBack(const UINT, const D3D12_DESCRIPTOR_RANGE_TYPE);//�z��ɒǉ�
	};
}
