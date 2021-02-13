#include"RootParametor.h"

namespace Beginner
{

	//�z��T�C�Y���擾
	size_t RootParametor::GetSize()const
	{
		return rootParametor.size();
	}

	//Getter
	D3D12_ROOT_PARAMETER* RootParametor::GetRootParameter(const size_t index)
	{
		return &rootParametor[index];
	}

	//�z��ɒǉ�
	void RootParametor::PushBack(const D3D12_ROOT_PARAMETER pushRoot)
	{
		rootParametor.push_back(pushRoot);
	}

	//�z��ɒǉ�
	void RootParametor::PushBack(const D3D12_DESCRIPTOR_RANGE* pushRange, const UINT pushNum)
	{
		D3D12_ROOT_PARAMETER pushRoot = {};
		{
			pushRoot.ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
			pushRoot.ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;
			pushRoot.DescriptorTable.pDescriptorRanges = pushRange;
			pushRoot.DescriptorTable.NumDescriptorRanges = pushNum;
		}

		PushBack(pushRoot);
	}
}