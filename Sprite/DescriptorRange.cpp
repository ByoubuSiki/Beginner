#include"DescriptorRange.h"

namespace Beginner
{

	//DescriptorRange”z—ñ‚Ì’·‚³
	size_t DescriptorRange::GetSize()const
	{
		return descRange.size();
	}

	//Getter
	D3D12_DESCRIPTOR_RANGE* DescriptorRange::GetDescRange(const size_t index)
	{
		return &descRange[index];
	}

	//”z—ñ‚É’Ç‰Á
	void DescriptorRange::PushBack(const D3D12_DESCRIPTOR_RANGE pushDesc)
	{
		descRange.push_back(pushDesc);
	}

	//”z—ñ‚É’Ç‰Á
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