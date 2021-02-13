#include"DescriptorRange.h"

namespace Beginner
{

	//DescriptorRange�z��̒���
	size_t DescriptorRange::GetSize()const
	{
		return descRange.size();
	}

	//Getter
	D3D12_DESCRIPTOR_RANGE* DescriptorRange::GetDescRange(const size_t index)
	{
		return &descRange[index];
	}

	//�z��ɒǉ�
	void DescriptorRange::PushBack(const D3D12_DESCRIPTOR_RANGE pushDesc)
	{
		descRange.push_back(pushDesc);
	}

	//�z��ɒǉ�
	void DescriptorRange::PushBack(const UINT numDescriptor, const D3D12_DESCRIPTOR_RANGE_TYPE rangeType)
	{
		D3D12_DESCRIPTOR_RANGE pushDescRange = {};
		{
			pushDescRange.NumDescriptors = numDescriptor;
			pushDescRange.RangeType = rangeType;
			pushDescRange.BaseShaderRegister = 0;
			pushDescRange.OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;
		}

		PushBack(pushDescRange);
	}
}