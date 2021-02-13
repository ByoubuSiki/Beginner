#pragma once

#include"..\Base\Pch.h"

namespace Beginner
{

	class RootParametor
	{
	private:
		std::vector<D3D12_ROOT_PARAMETER> rootParametor;

	public:
		RootParametor() :rootParametor() {}
		~RootParametor() {}

		size_t GetSize()const;//配列サイズを取得
		D3D12_ROOT_PARAMETER* GetRootParameter(const size_t);//Getter
		void PushBack(const D3D12_ROOT_PARAMETER);//配列に追加
		void PushBack(const D3D12_DESCRIPTOR_RANGE*, const UINT);//配列に追加
	};
}