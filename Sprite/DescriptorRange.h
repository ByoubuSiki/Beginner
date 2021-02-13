#pragma once

#include"..\Base\Pch.h"

namespace Beginner
{
	class DescriptorRange
	{
	private:
		std::vector<D3D12_DESCRIPTOR_RANGE> descRange;//DescriptorRangeの可長変配列

	public:
		DescriptorRange() :descRange() {}
		~DescriptorRange() {}

		size_t GetSize()const;//DescriptorRange配列の長さ
		D3D12_DESCRIPTOR_RANGE* GetDescRange(const size_t);//Getter
		void PushBack(const D3D12_DESCRIPTOR_RANGE);//配列に追加
		void PushBack(const UINT, const D3D12_DESCRIPTOR_RANGE_TYPE);//配列に追加
	};
}
