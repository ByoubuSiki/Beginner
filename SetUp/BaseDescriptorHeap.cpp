#include"BaseDescriptorHeap.h"

BaseDescriptorHeap::BaseDescriptorHeap()
	:heap(nullptr)
{

}

BaseDescriptorHeap::~BaseDescriptorHeap()
{

}

Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> BaseDescriptorHeap::GetDescriptorHeap()
{
	return heap;
}