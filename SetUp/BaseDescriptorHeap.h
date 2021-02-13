#pragma once

#include"..\Base\Pch.h"

class BaseDescriptorHeap
{
protected:
	virtual bool CreateHeap(const Microsoft::WRL::ComPtr<ID3D12Device>&) = 0;
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> heap;

public:
	BaseDescriptorHeap();
	~BaseDescriptorHeap();

	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> GetDescriptorHeap();
};
